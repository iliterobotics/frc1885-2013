#ifndef BHS_CAMERAINF_H_
#define BHS_CAMERAINF_H_

#include "nivision.h"



/** packet size */
#define DEFAULT_PACKET_SIZE 512
/** port for communicating with camera */
#define CAMERA_PORT 80
/** port for sending video to laptop */
#define VIDEO_TO_PC_PORT 1180

class bhs_CameraInf
{
public:
	bhs_CameraInf(){}
	
	virtual ~bhs_CameraInf(){}

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
	virtual int GetDecodedImageBlocking(Image*& cameraImage, double *timestamp) = 0;

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
	virtual int GetJPEGImageBlocking(char** imageData, int* numBytes, double* timestamp) = 0;
	
	virtual void ThrowAwayFrames(int anFrames) = 0;
};

#endif
