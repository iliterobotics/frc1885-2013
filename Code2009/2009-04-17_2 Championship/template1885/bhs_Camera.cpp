
/********************************************************************************
*  Project   		: FIRST Motor Controller
*  File Name  		: AxisCamera.cpp        
*  Contributors 	: TD, ELF, JDG, SVK
*  Creation Date 	: July 29, 2008
*  Revision History	: Source code & revision history maintained at sourceforge.WPI.edu    
*  File Description	: Axis camera access for the FIRST Vision API
*      The camera task runs as an independent thread 
*/    
/*----------------------------------------------------------------------------*/
/*        Copyright (c) FIRST 2008.  All Rights Reserved.                     */
/*  Open Source Software - may be modified and shared by FRC teams. The code  */
/*  must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*----------------------------------------------------------------------------*/                   


#include "sockLib.h" 
#include "vxWorks.h" 

#include "errno.h"
#include "fioLib.h"
#include "hostLib.h" 
#include "inetLib.h" 
#include "signal.h"
//#include "sigLib.h"		// for signal
#include "sysLib.h"
#include <string>
#include "time.h"
#include "usrLib.h"

#include "BaeUtilities.h"
#include "FrcError.h"
#include "Task.h"
#include "Timer.h"
#include "Utility.h"
#include "VisionAPI.h" 

#include "WPILib.h"
#include "bhs_Camera.h"

#include "taskLibCommon.h"
#include "PCVideoServer1885.h"


// To locally enable debug printing: set AxisCamera_debugFlag to a 1, to disable set to 0
static int AxisCamera_debugFlag = 0;
#define DPRINTF if(AxisCamera_debugFlag)dprintf


/**
 * serverName is the IPAddress that you want to use for the camera
 */
bhs_Camera::bhs_Camera(bool abSupportPCVideo)
:bhs_CameraInf(),
mLatestValidIndex(-1),
mCameraReady(false),
//mDecodeImage(true),
mCameraServerIP("192.168.0.90"),
mCameraFramesPerSecond(30),
mCameraCompressionRatio(20),
mCameraResolution(cnCAMERA_IMAGE_SIZE),
mCameraRotation(ROT_0),
mnVideoServerTCPPortNum(VIDEO_TO_PC_PORT),
mbSupportPCVideo(abSupportPCVideo),
meDecodedImageType(IMAQ_IMAGE_HSL)
{	
	printf("Begin bhs_Camera constructor\n");
	mJPEGImageSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	mDecodedImageSem = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY);
	for (int i = 0; i < cnNumStoreImages; i++) {
		mCameraImagedata[i].cameraImage = NULL;
		//mCameraImagedata[i].decodedImage = NULL;
	}
	printf("Completed bhs_Camera constructor\n");
}

/**
 * @brief This is the routine that is run when the task is spawned
 */
int bhs_Camera::initCamera(RobotGlobalData *apData) {
	
	//char funcName[]="initCamera";
	//let user code determine debug level
	//SetDebugFlag ( DEBUG_SCREEN_AND_FILE  ) ;
	
	DPRINTF(LOG_DEBUG, "\n+++++ camera task starting: rotation = %i", (int)mCameraResolution);
	
	if(mbSupportPCVideo){
		pcVideoServer = new PCVideoServer1885(static_cast<bhs_CameraInf*>(this),
				mnVideoServerTCPPortNum);
		if(pcVideoServer != NULL){
			pcVideoServer->initVideoServer();
		}
	}
	else{
		pcVideoServer = NULL;
	}

	taskSpawn("bhs_Camera",
		  101,						//task pri.
		  VX_FP_TASK,				// options
		  640000,					// stack size
		  (FUNCPTR)initCameraNotClass,	// function to start
		  (int)this,				// parameter 1 - Pointer to this camera object
		  0, 0, 0, 0, 0, 0, 0, 0, 0);
	return (OK);
}

void bhs_Camera::configDecodedImageType(ImageType_enum aeImageType)
{
	meDecodedImageType = aeImageType;
}

/*NAME       : configVideoSupported
 *DESCRIPTION: configures class instance to support/not support JPEG
 * 			   Video streaming
 *PARAMETERS : 
 * abVideo - Flag - support TCP JPEG Video streaming
 *RETURN     : NA
 */
void bhs_Camera::configVideoSupported(bool abVideo)
{
	mbSupportPCVideo = abVideo;
}

/*NAME       : configVideoSupported
 *DESCRIPTION: configures TCP JPEG Server port number
 *PARAMETERS : 
 * anPort - TCp Port number
 *RETURN     : NA
 */
void bhs_Camera::configVideoPort(int anPort)
{
	mnVideoServerTCPPortNum = anPort;
}


/* Method Name:
 * Description: Implements
 * Parameters: 
 *   image - This is the pointer to 
 *   anMotorDirection - 
 * Reurn:
 * 	NA - 
 * @brief Get the most recent camera image.
 * Supports IMAQ_IMAGE_RGB and IMAQ_IMAGE_HSL.
 * @param image Image to return to; image must have been first created using frcCreateImage. 
 * When you are done, use frcDispose.
 * @param timestamp Timestamp to return; will record the time at which the image was stored.
 * @param lastImageTimestamp Input - timestamp of last image; prevents serving of stale images
 * @return 0 is failure, 1 is success
 * @sa frcCreateImage(), frcDispose()
 */
int bhs_Camera::GetDecodedImageBlocking(Image*& image, double *timestamp)
{
	int success = OK;
	int readIndex;

	cameraMetrics[CAM_BLOCKING_COUNT]++;
	/* check to see if camera is initialized */

	semTake(mDecodedImageSem, WAIT_FOREVER);
	readIndex = mLatestValidIndex;

	if ( (image = frcCreateImage(meDecodedImageType)) != NULL) {
	  //printf(" bhs_Camera::GetDecodedImageBlocking %p\n",image);
		if (!Priv_ReadJPEGString_C(image,
				(const unsigned char *)mCameraImagedata[readIndex].cameraImage,
				mCameraImagedata[readIndex].cameraImageSize) ) {
			DPRINTF (LOG_DEBUG, "failure creating Image");
			success = ERROR;
			cameraMetrics[CAM_GETIMAGE_FAILURE]++;
		}else{
			cameraMetrics[CAM_GETIMAGE_SUCCESS]++;
			// save the timestamp to check on a success
			*timestamp = mCameraImagedata[readIndex].timestamp;
		}
	} else {
		success = ERROR;
		cameraMetrics[CAM_GETIMAGE_FAILURE]++;
	}
	
	return success;	
}

/** mutator functions*/
void bhs_Camera::configIP(string acServerName)
{
	mCameraServerIP = acServerName;
}
int bhs_Camera::configFrames(int anFrames)
{
	int lnReturnSet = 0;
	if(anFrames > 0 && anFrames <= 60){
		lnReturnSet = mCameraFramesPerSecond = anFrames;
	}
	return lnReturnSet;
}
int bhs_Camera::configCompression(int anCompression)
{
	int lnReturnSet = 0;
	if(anCompression >= 0 && anCompression <= 100)
		lnReturnSet = mCameraCompressionRatio = anCompression;
	
	return lnReturnSet;
}
void bhs_Camera::configImgSize(ImageSize aeResolution)
{
	mCameraResolution = aeResolution;
}
void bhs_Camera::configImgRotation(ImageRotation aeRotation)
{
	mCameraRotation = aeRotation;
}

/**
 * @brief Verifies that the camera is initialized
 * @return 0 for failure, 1 for success
 */
bool bhs_Camera::isCameraInitialized()
{
  char funcName[] = "isCameraInitialized";
	bool success = true;
	/* check to see if camera is initialized */
	if (!mCameraReady)  {
		imaqSetError(ERR_CAMERA_NOT_INITIALIZED, funcName);
		DPRINTF (LOG_DEBUG, "Camera request before camera is initialized");
		cameraMetrics[CAM_GETIMAGE_BEFORE_INIT]++;
		cameraMetrics[CAM_GETIMAGE_FAILURE]++;
		success = false;
	}
	
	if (mLatestValidIndex == -1){
		imaqSetError(ERR_CAMERA_NO_BUFFER_AVAILABLE, funcName);
		DPRINTF (LOG_DEBUG, "No camera image available");
		cameraMetrics[CAM_GETIMAGE_BEFORE_AVAILABLE]++;
		cameraMetrics[CAM_GETIMAGE_FAILURE]++;
		success = false;
	}
	return 1;
}





/**
 * @brief Blocking call to get images for PC.
 * This should be called from a separate task to maintain camera read performance. 
 * It is intended to be used for sending raw (undecoded) image data to the PC.
 * @param imageData image data to return
 * @param numBytes number of bytes in buffer
 * @param timestamp timestamp of buffer returned
 * @param lastImageTimestamp buffer time of last image data sent to PC
 */
int bhs_Camera::GetJPEGImageBlocking(char** imageData, int* numBytes, double* currentImageTimestamp)
{
	
	//char funcName[]="GetImageData";
	int success= OK;
	int readIndex;
	int cameraImageSize;
	char *cameraImageString;

	cameraMetrics[CAM_BLOCKING_COUNT]++;
	/* check to see if camera is initialized */

	semTake(mJPEGImageSem, WAIT_FOREVER);
	/* try readCount times to get an image */

	readIndex = mLatestValidIndex;
	cameraImageSize = mCameraImagedata[readIndex].cameraImageSize;
	//cameraImageString = (Image *) malloc(cameraImageSize);
	cameraImageString = new char[cameraImageSize];
	if (NULL == cameraImageString) {
		DPRINTF (LOG_DEBUG, "Unable to allocate cameraImage");
		cameraMetrics[CAM_GETIMAGE_FAILURE]++;
		success = ERROR;
	} else {
		memcpy(cameraImageString, mCameraImagedata[readIndex].cameraImage, cameraImageSize);
		*currentImageTimestamp = mCameraImagedata[readIndex].timestamp;

		*imageData = cameraImageString;
		*numBytes = cameraImageSize;
	}
	return success;

}


/**
 * @brief Accessor for camera instrumentation data
 * @param the counter queried
 * @return the counter value
 */
int bhs_Camera::GetCameraMetric(FrcvCameraMetric metric)
{	return cameraMetrics[metric];  }

/**
 * @brief Close socket & report error
 * @param errstring String to print
 * @param socket Socket to close
 * @return error
 */
void bhs_Camera::cameraCloseSocket(char *errstring, int socket)
{
	//char funcName[]="cameraCloseSocket";
	DPRINTF (LOG_CRITICAL, "CAMERA ERROR: %s", errstring );
	close (socket);
}

#define MEASURE_SOCKET_TIME 1   

/**
 * @brief Manage access to the camera. Sets up sockets and reads images
 * @return error
 */
int bhs_Camera::cameraJPEGServer()
{
	//Increments if an image is received and prints out after every 30 images.
	int imageCounter = 0;
	
	char funcName[]="cameraJPEGServer";
	//Passing in IPAddress as serverName
	//char *serverName = "192.168.0.90";		/* camera @ */
	//char *server = serverName;
	short cont = 1;
	int errorCode = 0;
	int printCounter = 0;
	int	writeIndex;
	int authorizeCount = 0;
	int authorizeConfirmed = 0;
	static const int authenticationStringsCount = 3;
    static const char* authenticationStrings[] = {
    		"cm9vdDphZG1pbg==", /* root, admin*/
    		"RlJDOkZSQw==",     /* FRC, FRC */
    		"cm9vdDpwYXNz=="    /* root, pass*/
    };

	DPRINTF (LOG_DEBUG, "cameraJPEGServer" ); 
	
	struct sockaddr_in cameraAddr;
	int sockAddrSize;  
	int camSock = 0;    

	char resStr[10];
	switch (mCameraResolution) {
		case k640x480: { sprintf(resStr,"640x480"); break; }
		case k320x240: { sprintf(resStr,"320x240"); break; }
		case k160x120: { sprintf(resStr,"160x120"); break; }
		default: {DPRINTF (LOG_DEBUG, "code error - resolution input" ); break; }
	}
	
	/* Generate camera initialization string */
	/* changed resolution to 160x120 from 320x240 */
	/* supported resolutions are: 640x480, 640x360, 320x240, 160x120 */	
	char * getStr1 = 
	"GET /axis-cgi/mjpg/video.cgi?showlength=1&camera=1&";	
		
	char insertStr[100];
	sprintf (insertStr, "des_fps=%i&compression=%i&resolution=%s&rotation=%i", 
			mCameraFramesPerSecond, mCameraCompressionRatio, resStr, (int)mCameraResolution);	
	
	char * getStr2 = " HTTP/1.1\n\
User-Agent: HTTPStreamClient\n\
Host: 192.150.1.100\n\
Connection: Keep-Alive\n\
Cache-Control: no-cache\n\
Authorization: Basic %s;\n\n";

	char getStr[strlen(getStr1) + strlen(insertStr) + strlen(getStr2)];      
    sprintf (getStr, "%s:%s%s",	getStr1, insertStr, getStr2);

	DPRINTF(LOG_DEBUG, "revised camera string: \n%s", getStr);
	/* Allocation */
	char tempBuffer[1024];
    
	RETRY:
	//while (acquire == 0) Wait(0.1);

	if (!authorizeConfirmed){
	  if (authorizeCount < authenticationStringsCount){
	    sprintf (tempBuffer, getStr, authenticationStrings[authorizeCount]);
	  } else {
		imaqSetError(ERR_CAMERA_AUTHORIZATION_FAILED, funcName);
		perror ("Camera authorization failed ... Incorrect password on camera!!");
		return (ERROR);
	  }
	}

	while (1)
	{
	  cameraMetrics[CAM_SOCKET_INIT_ATTEMPTS]++;	  

	  /* create camera socket */
	  DPRINTF (LOG_DEBUG, "creating camSock" ); 
	  if ((camSock = socket (AF_INET, SOCK_STREAM, 0)) == ERROR) {	
		imaqSetError(ERR_CAMERA_SOCKET_CREATE_FAILED, funcName);
		perror ("Failed to create socket");
		cont = 0;
		return (ERROR);
	  }

	  sockAddrSize = sizeof (struct sockaddr_in);
	  bzero ((char *) &cameraAddr, sockAddrSize);
	  cameraAddr.sin_family = AF_INET;
	  cameraAddr.sin_len = (u_char) sockAddrSize;
	  cameraAddr.sin_port = htons (CAMERA_PORT);

	  DPRINTF (LOG_DEBUG, "getting IP" );
	  if (( (int)(cameraAddr.sin_addr.s_addr = inet_addr ((char*)(mCameraServerIP.c_str())) ) == ERROR) &&
		( (int)(cameraAddr.sin_addr.s_addr = hostGetByName ((char*)(mCameraServerIP.c_str())) ) == ERROR)) 
	  {	
		cameraCloseSocket("Failed to get IP, check hostname or IP", camSock);
		continue;
	  }
	  
	  DPRINTF (LOG_INFO, "Attempting to connect to camSock" ); 
	  if (connect (camSock, (struct sockaddr *) &cameraAddr, sockAddrSize) == ERROR) 	{
		imaqSetError(ERR_CAMERA_CONNECT_FAILED, funcName);
		cameraCloseSocket("Failed to connect to camera - check network", camSock);
		continue;
	  }	    
	  
	  cameraMetrics[CAM_SOCKET_OPEN]++;
	  break;
	} // end while (trying to connect to camera)

	DPRINTF (LOG_DEBUG, "writing GET request to camSock" ); 
	if (write (camSock, tempBuffer , strlen(tempBuffer) ) == ERROR) {
		cameraCloseSocket("Failed to send GET request", camSock);
		return ERROR;
	}

	//DPRINTF (LOG_DEBUG, "reading header" ); 
	/* Find content-length, then read that many bytes */
	int counter = 2;
	char* contentString = "Content-Length: ";
	char* authorizeString = "200 OK";
	

	
	while (cont) {
	  
		// If camera has been turned OFF, jump to RETRY
		//if (acquire == 0) goto RETRY;
		
		/* Determine writer index */
		if (mLatestValidIndex >= cnNumStoreImages-1)
		{
		  writeIndex = 0;
		}
		else
		{
		  writeIndex = mLatestValidIndex+1;
		}
		
		
		/* read header */
		//TODO: check for error in header, increment ERR_CAMERA_HEADER_ERROR
		char initialReadBuffer[DEFAULT_PACKET_SIZE] = "";
		char intermediateBuffer[1];
		char *trailingPtr = initialReadBuffer;
		int trailingCounter = 0;	

		while (counter) {
			if (read (camSock, intermediateBuffer, 1) <= 0) {
				cameraCloseSocket("Failed to read image header", camSock);
				cameraMetrics[ERR_CAMERA_HEADER_ERROR]++;
				goto RETRY;
			}

			strncat(initialReadBuffer, intermediateBuffer, 1);
			if (NULL != strstr(trailingPtr, "\r\n\r\n")) {

				  if (!authorizeConfirmed){

					  if (strstr(initialReadBuffer, authorizeString))
					  {
						  authorizeConfirmed = 1;
						  /* set camera to initialized */
						  mCameraReady = true; 
					  }
					  else
					  {
						  cameraCloseSocket("Not authorized to connect to camera", camSock);
						  authorizeCount++;
				  goto RETRY;
					  }
				}
				--counter;
			}
			if (++trailingCounter >= 4) {
				trailingPtr++;
			}
		}
	
		counter = 1;
		char *contentLength = strstr(initialReadBuffer, contentString);
		if (contentLength == NULL) {
			cameraMetrics[ERR_CAMERA_HEADER_ERROR]++;
			cameraCloseSocket("No content-length token found in packet", camSock);
			goto RETRY;
		}
		/* get length of image content */
		contentLength = contentLength + strlen(contentString);
		mCameraImagedata[writeIndex].cameraImageSize = atol (contentLength);
		
		if(mCameraImagedata[writeIndex].cameraImage != NULL){
			delete mCameraImagedata[writeIndex].cameraImage;
		}
		mCameraImagedata[writeIndex].cameraImage = new char[mCameraImagedata[writeIndex].cameraImageSize];
		if (NULL == mCameraImagedata[writeIndex].cameraImage) {
			cameraCloseSocket("Failed to allocate space for imageString", camSock);
			return ERROR;
		}
		cameraMetrics[CAM_BUFFERS_WRITTEN]++;
		
		//
		// This is a blocking camera read function, and will block if the camera
		// has been disconnected from the cRIO.  If however the camera is
		// POWERED OFF while connected to the cRIO, this function NEVER RETURNS
		//
		int bytesRead = fioRead (camSock, (char *)mCameraImagedata[writeIndex].cameraImage,
				mCameraImagedata[writeIndex].cameraImageSize);
		
		//DPRINTF (LOG_DEBUG, "Completed fioRead function - bytes read:%d", bytesRead);
		if (bytesRead <= 0) {
			cameraCloseSocket("Failed to read image data", camSock);
			goto RETRY;
		} else if (bytesRead != mCameraImagedata[writeIndex].cameraImageSize){
			perror ("ERROR: Failed to read entire image: readLength does not match bytes read");
			cameraMetrics[CAM_BAD_IMAGE_SIZE]++;
		}
		
		imageCounter++;
		
		//New Scheme - decoding is done when GetDecodedImageBlocking() call is made
		//This will save on unneeded processing and memory
		// if decoding the JPEG to an HSL Image, do it here
		/*if (mDecodeImage) 
		{
			
			if(mCameraImagedata[writeIndex].decodedImage != NULL)
				frcDispose(mCameraImagedata[writeIndex].decodedImage);
			mCameraImagedata[writeIndex].decodedImage = frcCreateImage(meDecodedImageType);
			if (! Priv_ReadJPEGString_C(mCameraImagedata[writeIndex].decodedImage, 
					(const unsigned char *)mCameraImagedata[writeIndex].cameraImage, 
					mCameraImagedata[writeIndex].cameraImageSize) ) {
				DPRINTF (LOG_DEBUG, "failure creating Image");			
			}
		}*/
		
		/*if(imageCounter % 30 == 0)
			printf("got %d images\n", imageCounter);*/
		// TODO: React to partial image
		mCameraImagedata[writeIndex].timestamp = GetTime();
		mLatestValidIndex = writeIndex;
		semGive(mJPEGImageSem);
		semGive(mDecodedImageSem);

		cameraMetrics[CAM_NUM_IMAGE]++;	
		printCounter ++;
		if (printCounter == 1000) { 
			DPRINTF (LOG_DEBUG, "imageCounter = %i", cameraMetrics[CAM_NUM_IMAGE]); 
			printCounter=0; 
		}
		
		taskDelay(1);  
			
	}  /* end while (cont) */

	/* clean up */
	close (camSock);
	cont = 0;
	DPRINTF (LOG_INFO, "\nJPEG SERVER ENDING  errorCode = %i\n", errorCode );
	
	return (OK);
}

void bhs_Camera::ThrowAwayFrames(int anFrames)
{
  //It is likely that the semaphore will already be given
  semTake(mDecodedImageSem, WAIT_FOREVER);
  //Now wait for the next frame to be received
  for(int i=0; i<anFrames; i++)
  {
    semTake(mDecodedImageSem, WAIT_FOREVER);
  }
}

int bhs_Camera::initCameraNotClass(int camera)
{
	return ((bhs_Camera*)camera)->cameraJPEGServer();
}
