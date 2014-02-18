#ifndef BHS_CAMERA_H_
#define BHS_CAMERA_H_

/*----------------------------------------------------------------------------*/
/*        Copyright (c) FIRST 2008.  All Rights Reserved.                     */
/*  Open Source Software - may be modified and shared by FRC teams. The code  */
/*  must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*----------------------------------------------------------------------------*/

/********************************************************************************
*  Project   	: bhs_Camera
*  Description	: This class is designed to acquire images from a FIRST AXIS 206 
* 				  camera. 
* 
* USAGE			: 	Once initCamera has been called, GetDecodedImageBlocking can
* 					be used ti get the latest image. 
* 					All config functions of the Camera1998 class need to be 
* 					called before initCamera in order to change the camera
* 					parameters.  config functions do not need to be call if the
* 					default parameters are acceptable.
* 					CAMERA DEFAULT PARAMETERS:
* 						mCameraServerIP("192.168.0.90"),
* 						mCameraFramesPerSecond(30),
* 						mCameraCompressionRatio(50),
* 						mCameraResolution(k640x480),
* 						mCameraRotation(ROT_0),
* 						meDecodedImageType(IMAQ_IMAGE_HSL)
* 
*  API: Because nivision.h uses C++ style comments, any file including this
*  must be a .cpp instead of .c.
*/


#include "nivision.h"
#include "Task.h"
#include "string.h"
#include "bhs_CameraInf.h"
#include "RobotGlobalData.h"


//forward  declaration
class PCVideoServer1885;



class bhs_Camera : public bhs_CameraInf
{
	static int initCameraNotClass(int camera);
public:
	/*  Enumerated Types */
	/** @brief enumeration describing the different status parameters that are logged*/
	enum FrcvCameraMetric {
		CAM_STARTS, 					CAM_STOPS, 				CAM_NUM_IMAGE,
		CAM_BUFFERS_WRITTEN, 			CAM_BLOCKING_COUNT,		CAM_SOCKET_OPEN, 	
		CAM_SOCKET_INIT_ATTEMPTS,		CAM_BLOCKING_TIMEOUT,	CAM_GETIMAGE_SUCCESS, 
		CAM_GETIMAGE_FAILURE, 			CAM_STALE_IMAGE, 		CAM_GETIMAGE_BEFORE_INIT, 
		CAM_GETIMAGE_BEFORE_AVAILABLE,	CAM_READ_JPEG_FAILURE, 	CAM_PID_SIGNAL_ERR, 
		CAM_BAD_IMAGE_SIZE, 			CAM_HEADER_ERROR, 		CAM_NUM_METRICS};
	enum ImageRotation { ROT_0 = 0, ROT_90 = 90, ROT_180 = 180, ROT_270 = 270 };
	enum ImageSize { k640x480, k320x240, k160x120 };
	
	static const ImageSize cnCAMERA_IMAGE_SIZE = k320x240;
	
	
	/*NAME       : Camera1885
	 *DESCRIPTION: constructor
	 *PARAMETERS : abSupportPCVideo - if Camera1885 instance should create a
	 * 		           TCP server for an application to receive JPEG images 
	 *RETURN     : NA
	 */
	bhs_Camera (bool abSupportPCVideo = true);
	virtual ~bhs_Camera(){}
	
	/*NAME       : initCamera
	 *DESCRIPTION: Function to be called to start class instance working
	 *PARAMETERS : 
	 * NA
	 *RETURN     : NA
	 */
	int initCamera(RobotGlobalData *apData);
	
	/*NAME       : configVideoSupported
	 *DESCRIPTION: configures class instance to support/not support JPEG
	 * 			   Video streaming
	 *PARAMETERS : 
	 * abVideo - Flag - support TCP JPEG Video streaming
	 *RETURN     : NA
	 */
	void configVideoSupported(bool abVideo);
	
	/*NAME       : configVideoSupported
	 *DESCRIPTION: configures TCP JPEG Server port number
	 *PARAMETERS : 
	 * anPort - TCp Port number
	 *RETURN     : NA
	 */
	void configVideoPort(int anPort);
	
	/*NAME       : configIP
	 *DESCRIPTION: Changes the IP of the FIRST AXIS camera.
	 *PARAMETERS : 
	 * acServerName - New IP address of camera
	 *RETURN     : NA
	 */
	void configIP(string acServerName);
	
	/*NAME       : configDecodedImageType
	 *DESCRIPTION: Changes type of image the class will decode the JPEG image
	 * 			   into.
	 *PARAMETERS : 
	 * aeImageType - New decoded Image type
	 *RETURN     : NA
	 */
	void configDecodedImageType(ImageType_enum aeImageType);

	/*NAME       : configFrames
	 *DESCRIPTION: Set the (maximum) number of frames that camera should 
	 * 			   acquire each second.
	 *PARAMETERS : 
	 * 	anFrames - Number of Frames per second (currently limited to a max of 60)
	 *RETURN     : 0 - failure or anFrame if successful
	 */
	int configFrames(int anFrames);

	/*NAME       : configCompression
	 *DESCRIPTION: configures the compression rate the AXIS camera should use
	 *PARAMETERS : 
	 * 	anCompression - [ 0 ...100] compression rate
	 *RETURN     : 0 on failure or anCompression
	 */
	int configCompression(int anCompression);

	/*NAME       : configImgSize
	 *DESCRIPTION: Image resolution the AXIS camera should encode for
	 *PARAMETERS : 
	 * 	aeResolution - image resolution
	 *RETURN     : NA
	 */
	void configImgSize(ImageSize aeResolution);

	/*NAME       : configImgRotation
	 *DESCRIPTION: Image rotation AXIS camera should encode for
	 *PARAMETERS : 
	 * 	aeRotation - image rotation
	 *RETURN     : NA
	 */
	void configImgRotation(ImageRotation aeRotation);

	/*NAME       : isCameraInitialized
	 *DESCRIPTION: Returns if camera is runing and has image(s) available
	 *PARAMETERS : 
	 * 	NA 
	 *RETURN     : true -YES false - NO
	 */
	bool isCameraInitialized();

	/*NAME       : GetDecodedImageBlocking
	 *DESCRIPTION: Blocking function that waits until a (HSL or RGP)image is
	 * 			   available.  cameraImage needs to be free using frcDispose
	 * 			   when the user/application is done with the data (or a memory
	 *  		   will occur).
	 *PARAMETERS : 
	 * 	cameraImage - pointer NI Image object
	 *	timestamp - timestamp of when the image was received from the camera
	 *RETURN     : [OK,ERROR]
	 */
	virtual int GetDecodedImageBlocking(Image*& cameraImage, double *timestamp);

	/*NAME       : GetJPEGImageBlocking
	 *DESCRIPTION: Blocking function that waits until a JPEG image is
	 * 			   available.  imageData needs to be free using delete
	 * 			   when the user/application is done with the data (or a memory
	 *  		   will occur).
	 *PARAMETERS :
	 * imageData - pointer to a char pointer that will hold the new image
	 * numbytes - size of image that is returns in imageData
	 * timestamp - timestamp of when the image was received from the camera
	 *RETURN     : [OK, ERROR]
	 */
	virtual int GetJPEGImageBlocking(char** imageData, int* numBytes, double* timestamp);
	
	virtual void ThrowAwayFrames(int anFrames);

	

	/*NAME       :
	 *DESCRIPTION: 
	 *PARAMETERS :
	 *RETURN     : 
	 */
	int GetCameraMetric(FrcvCameraMetric metric);
	
private:
	
	/* start the camera server */
	/*NAME       : cameraJPEGServer
	 *DESCRIPTION: Function designed to be called by a TASK function.  This 
	 * 			   functions connects to the AXIS camera and collects and
	 * 			   stores JPEG images for other TASK to use.
	 *PARAMETERS :
	 * NA
	 *RETURN     : OK or error code
	 */
	int cameraJPEGServer();
	
	//close socket used to connect to the camera and logs the error
	/*NAME       : cameraCloseSocket
	 *DESCRIPTION: closes a socket and logs and error message
	 *PARAMETERS :
	 * errstring - Error message
	 * socket - Socket ID
	 *RETURN     : NA
	 */
	void cameraCloseSocket(char *errstring, int socket);
	

	/*NAME		 : mLatestValidIndex
	 *TYPE       : int
	 *DESCRIPTION: Describes which array element of mCameraImagedata contains
	 * 			   the latest image (-1 if no data is available).
	 */
	int mLatestValidIndex; /* -1,0,....(cnNumStoreImages-1) */
	
	/*NAME		 : mCameraReady
	 *TYPE       : bool
	 *DESCRIPTION: If the camera class instance is ready to provide images to other
	 * 			   task/applications
	 */
	bool mCameraReady;  
	
	/*NAME		 : decode
	 *TYPE       : bool
	 *DESCRIPTION: if the class instance should decode the JPEG images		   
	 */
	//bool	mDecodeImage;

	/*NAME		 : mCameraServerIP
	 *TYPE       : string
	 *DESCRIPTION: IP address of the AXIS camera		   
	 */
	string mCameraServerIP; //serverName;
	
	/*NAME		 : mCameraFramesPerSecond
	 *TYPE       : int
	 *DESCRIPTION: Number of Frames per second the AXIS camera will attempt to
	 * 			   transmit
	 */
	int mCameraFramesPerSecond;
	
	/*NAME		 : mCameraCompressionRatio
	 *TYPE       : int
	 *DESCRIPTION: Compression RATIO the AXIS camera will use to compress the
	 * 			   JPEG image.  Bound[0......100]
	 */
	int mCameraCompressionRatio;
	
	/*NAME		 : mCameraResolution
	 *TYPE       : enumeration(ImageSize)
	 *DESCRIPTION: image resolution		   
	 */
	ImageSize mCameraResolution;
	
	/*NAME		 : mCameraRotation
	 *TYPE       : enumeration(ImageRotation)
	 *DESCRIPTION: Rotational Orientation the AXIS camera will encode the image 		   
	 */	
	ImageRotation mCameraRotation;
	
	/*NAME		 :mJPEGImageSem
	 *TYPE       : SEM_ID
	 *DESCRIPTION: Semaphore used to control when a JPEG image is available
	 */
	SEM_ID mJPEGImageSem;
	
	/*NAME		 :mDecodedImageSem
	 *TYPE       : SEM_ID
	 *DESCRIPTION: Semaphore used to control when a decoded image is available
	 */	
	SEM_ID mDecodedImageSem;
	
	/*NAME		 : pcVideoServer
	 *TYPE       : PCVideoServer1885
	 *DESCRIPTION: pointer to class that will be the JPEG TCP image server for
	 * 			   class instance
	 */
	PCVideoServer1885 *pcVideoServer;
	
	
	/*NAME		 : mnVideoServerTCPPortNum
	 *TYPE       : int
	 *DESCRIPTION: Port number for TCP JPEG Video server
	 */
	int mnVideoServerTCPPortNum;
	
	/*NAME		 : mbSupportPCVideo
	 *TYPE       : bool
	 *DESCRIPTION: flag if class instance should create a JPEG Video server
	 */
	bool mbSupportPCVideo;
	
	/*NAME		 : cnNumStoreImages
	 *TYPE       : static int
	 *DESCRIPTION: Descibes how many of the most recent mCameraImagedata is kept 		   
	 */	
	static const int cnNumStoreImages = 20;
	
	/*NAME		 : meDecodedImageType
	 *TYPE       : ImageType_enum
	 *DESCRIPTION: What type of image should the JPEG image be decoded into		   
	 */
	ImageType_enum meDecodedImageType;
	
	/*NAME		 :mCameraImagedata
	 *TYPE       : struct
	 *DESCRIPTION: Structure containing the data that descibes a received and
	 * 			   process image from the AXIS camera.
	 */
	struct {
		//
		// To find the latest image timestamp, access:
		// globalCamera.data[globalCamera.index].timestamp
		//
		double timestamp;
		char*	cameraImage;
		int cameraImageSize;
		//Image* decodedImage; //decode image if they ask for it - this change will save on memory and unnecessary processing
		//int decodedImageSize;
	}mCameraImagedata[cnNumStoreImages];
	
	/*NAME		 : cameraMetrics
	 *TYPE       : int array
	 *DESCRIPTION: Array of Camera status counters
	 * 			   
	 */
	int cameraMetrics[CAM_NUM_METRICS];
	
};
#endif
