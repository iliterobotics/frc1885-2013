/********************************************************************************
*  Project   		: FIRST Motor Controller
*  File Name  		: PCVideoServer1885.cpp
*  Contributors   	: SVK, ELF
*  Creation Date 	: November 30, 2008
*  Revision History	: Source code & revision history maintained at sourceforge.WPI.edu
*  File Description	: C++ Axis camera control for the FIRST Vision API
*/
/*----------------------------------------------------------------------------*/
/*        Copyright (c) FIRST 2008.  All Rights Reserved.                     */
/*  Open Source Software - may be modified and shared by FRC teams. The code  */
/*  must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*----------------------------------------------------------------------------*/

#include <vxWorks.h>

#include "sockLib.h"
#include "inetLib.h"
#include "hostLib.h"

#include "BaeUtilities.h"
#include "chipobject/NiRioStatus.h"
#include "Task.h"
#include "Timer.h"
#include "Utility.h"
#include "semLibCommon.h"

#include "PCVideoServer1885.h"
#include "bhs_CameraInf.h"

//bool mbStopImageToPCServer=false;

/**
@brief Implements an object that automatically does a close on a
camera socket on destruction.
*/
class ScopedSocket {
public:
    ScopedSocket(int camSock, const ErrorBase* errorBase) :
        m_camSock(camSock)
    {
        if (errorBase->StatusIsFatal()) {
            return;
        }
        if (m_camSock == ERROR) {
           // wpi_setError(*errorBase, ERR_CAMERA_SOCKET_CREATE_FAILED);
            return;
        }
    }

    ~ScopedSocket() {
        if (m_camSock != ERROR) {
            close(m_camSock);
        }
    }
    //  Cast to int allows you to pass this to any function that
    //  takes the socket as an int.
    operator int() const {
        return m_camSock;
    }

private:
    int m_camSock;
};

//============================================================================
//  PCVideoServer1885
//============================================================================

/**
@brief Constructor.
 port default = VIDEO_TO_PC_PORT
*/
static void PCServerTask(int anThis)
{
	PCVideoServer1885* lpThis = (PCVideoServer1885*)anThis;
	
	lpThis->ImageToPCServer();
}

PCVideoServer1885::PCVideoServer1885(bhs_CameraInf *camera) 
{
	mnVideoPCPort = VIDEO_TO_PC_PORT;
	mbStopImageToPCServer=false;
    try {
    	printf ("START PCVideoServer constructor\n");
        // Start the image communication task.
        if (StatusIsFatal()) {
            throw GetError().GetCode();
        }
    }
    catch(Error::Code error) {
        wpi_setError(*this, error);
        return;
    }
    
    mpCameraServer = camera;
}


/**
@Constuctor with specific port
Created by: Jacob Cohen - 1/10/09
*/
PCVideoServer1885::PCVideoServer1885(bhs_CameraInf *camera, int port)
{
	mnVideoPCPort = port;
	mbStopImageToPCServer=false;
    try {
    	printf ("START PCVideoServer1885 constructor\n");
        // Start the image communication task.
        if (StatusIsFatal()) {
            throw GetError().GetCode();
        }
    }
    catch(Error::Code error) {
        wpi_setError(*this, error);
        return;
    }
    
    mpCameraServer = camera;
}

/**
@brief Destructor.
Stop serving images and destroy this class.
*/
PCVideoServer1885::~PCVideoServer1885() {
    printf ("START PCVideoServer1885 DESTRUCTOR **********************\n");
    
    //StopImageToPCServer();
    //  Clear the error so that you can use this object to make a connection to
    //  the mnVideoPCPort to stop the ImageToPCServer if it is waiting to
    //  accept connections from a PC.
    ClearError();
    //  Open a socket.
    ScopedSocket camSock(socket (AF_INET, SOCK_STREAM, 0), this);
    //  If successful
    if (!StatusIsFatal()) {
        //  Create a connection to the localhost.
        struct sockaddr_in selfAddr;
        int sockAddrSize = sizeof(selfAddr);
        bzero ((char *) &selfAddr, sockAddrSize);
        selfAddr.sin_family = AF_INET;
        selfAddr.sin_len = (u_char) sockAddrSize;
        selfAddr.sin_port = htons (mnVideoPCPort);

        if (( (int)(selfAddr.sin_addr.s_addr = inet_addr (const_cast<char*>("localhost")) ) != ERROR) ||
            ( (int)(selfAddr.sin_addr.s_addr = hostGetByName (const_cast<char*>("localhost")) ) != ERROR))
        {
            connect (camSock, (struct sockaddr *) &selfAddr, sockAddrSize);
        }
    }
}

int PCVideoServer1885::initVideoServer()
{
	if (StatusIsFatal()) 
	    {
	        return -1;
	    }
		int id = 0;
		mbStopImageToPCServer = false;

		// spawn video server task
		// this is done to ensure that the task is spawned with the
		// floating point context save parameter.
		id = taskSpawn("ImageToPCServer/n",
						101,
						VX_FP_TASK,								// options
						64000,									// stack size
						(FUNCPTR)PCServerTask,							// function to start
						(int)this, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// parameter 1 - pointer to this class



	    /**if (!mbStarted) 
	    {
	        wpi_setError(*this, ERR_CAMERA_TASK_SPAWN_FAILED);
	        return id;
	    }*/
		taskDelay(1);
		return id;
}

/**
@brief Stop sending images to the PC.
*/
void PCVideoServer1885::StopImageToPCServer() {
	printf("StopImageToPCServer");
	mbStopImageToPCServer = true;
}

/**
@brief Check is server is shutting down.
@return bool true if time to stop
*/
bool PCVideoServer1885::ShouldStopImageToPCServer()  {
    //  Return the current state of mbStopImageToPCServer
    return mbStopImageToPCServer;
}

/**
@brief Initialize the socket and serve images to the PC.
*/
int PCVideoServer1885::ImageToPCServer() {
    printf("ImageToPCServer Started\n");

    /* Setup to PC sockets */
    struct sockaddr_in serverAddr;
    int sockAddrSize = sizeof(serverAddr);
    int pcSock = ERROR;
    bzero ((char *) &serverAddr, sockAddrSize);
    serverAddr.sin_len = (u_char) sockAddrSize;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons (mnVideoPCPort);
    serverAddr.sin_addr.s_addr = htonl (INADDR_ANY);

    int success;
    while (!ShouldStopImageToPCServer()) {
        double lastImageTimestamp = 0;
        //  Create the socket.
        if ((pcSock = socket (AF_INET, SOCK_STREAM, 0)) == ERROR) {
            perror ("socket");
            continue;
        }
        //  Set the TCP socket so that it can be reused if it is in the wait state.
        int reuseAddr = 1;
        setsockopt(pcSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&reuseAddr), sizeof(reuseAddr));
        //  Bind socket to local address.
        // printf ("Binding to socket\n");
        if (bind (pcSock, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR)
        {
            perror ("bind");
            close (pcSock);
            continue;
        }
        //  Create queue for client connection requests.
        printf ("Listening on socket (port:%d)\n",mnVideoPCPort);
        if (listen (pcSock, 1) == ERROR)
        {
            perror ("listen");
            close (pcSock);
            continue;
        }
        //  Accept a new connect request
        struct sockaddr_in clientAddr;
        int clientAddrSize;
        printf ("accept socket\n");
        int newPCSock = accept (pcSock, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
        if (newPCSock  == ERROR) {
            perror ("accept");
            close(pcSock);
            continue;
        }
        printf ("CONNECTED TO PC socket:%d\n",newPCSock);
        //  Got a connection.
        //TODO: check camera error
        //if (!StatusIsFatal()) {
        if (1) {
        	printf ("Everyting is OKAY...starting to get images from camera\n");
            while(!ShouldStopImageToPCServer()) {
                double newTimestamp;
                int numBytes = 0;
                char* imageData = NULL;
                //printf ("Getting image from camera\n");
                //This function does not return unless an Image has been gotten
                success = mpCameraServer->GetJPEGImageBlocking(&imageData, &numBytes, &newTimestamp);
                //TODO:  only continue process if mpCameraServer->GetImageDataBlocking does not return 0
                if(success == OK)
                {
		            lastImageTimestamp = newTimestamp;
		
		            //printf ("Writing image to PC\n");
		            /* Write header to PC */
		            static const char header[4]={1,0,0,0};
		            int headerSend = write(newPCSock, const_cast<char*>(header), 4);
		
		            /* Write image length to PC */
		            int presend = write(newPCSock, reinterpret_cast<char*>(&numBytes), 4);
		
		            /* Write image to PC */
		            int sent = write (newPCSock, imageData, numBytes);
		
		            //  Cleanup memory allocated for the image.
		            delete imageData;
		            imageData = NULL;
		            numBytes = 0;
		
		            //  The PC probably closed connection. Get out of here
		            //  and try listening again.
		            if (headerSend == ERROR || sent == ERROR || presend == ERROR) {
		                break;
		            }
                }
                //no point in running too fast -
                //max camera frame rate is 30 fps
                //Wait(0.01);no need to wait - Image function 
                //mpCameraServer->GetImageDataBlocking blocks until camera has an image
            }
        }
        printf ("CLEANING UP...\n");
        //  Clean up
        close (newPCSock);
        newPCSock = ERROR;
        close (pcSock);
        pcSock = ERROR;
    }
    return (OK);
}

