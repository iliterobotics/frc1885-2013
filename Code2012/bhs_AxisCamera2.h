#ifndef BHS_AXIS_CAMERA_2_H
#define BHS_AXIS_CAMERA_2_H

#include "kernelLib.h"
#include "drv/timer/timerDev.h"

#include <taskLib.h>
#include <vxWorks.h> 
#include <sockLib.h> 
#include <inetLib.h>

#include "bhs_AxisCameraParams2.h"
#if JAVA_CAMERA_LIB != 1
#include "Vision/ColorImage.h"
#include "Vision/HSLImage.h"
#endif
#include "nivision.h"
#include <set>
#include "Task.h"

#include "bhs_PCDataServer.h"

/**
 * bhs_AxisCamera2 class.
 * This class handles everything about the Axis 206 FRC Camera.
 * It starts up 2 tasks each using a different connection to the camera:
 * - image reading task that reads images repeatedly from the camera
 * - parameter handler task in the base class that monitors for changes to
 *     parameters and updates the camera
 */
class bhs_AxisCamera2 : public bhs_AxisCameraParams2
{
private:
	explicit bhs_AxisCamera2(const char *cameraIP);
public:
	static const int FRAMES_PER_SECOND;
	
	virtual ~bhs_AxisCamera2();
	static bhs_AxisCamera2* GetInstance(const char *cameraIP = NULL);
	static void DeleteInstance();

	bool IsFreshImage();
	SEM_ID GetNewImageSem();

	int GetImage(Image *imaqImage);
#if JAVA_CAMERA_LIB != 1
	int GetImage(ColorImage *image);
	HSLImage *GetImage();
#endif

	int CopyJPEG(char **destImage, int &destImageSize, int &destImageBufferSize);
	void setDataServer(bhs_PCDataServer*);

private:
	static int s_ImageStreamTaskFunction(bhs_AxisCamera2 *thisPtr);
	int ImageStreamTaskFunction();

	int ReadImagesFromCamera();
	void UpdatePublicImageFromCamera(char *imgBuffer, int imgSize);

	virtual void RestartCameraTask();

	static bhs_AxisCamera2 *_instance;
	int m_cameraSocket;
	typedef std::set<SEM_ID> SemSet_t;
	SemSet_t m_newImageSemSet;

	char* m_protectedImageBuffer;
	int m_protectedImageBufferLength;
	int m_protectedImageSize;
	SEM_ID m_protectedImageSem;
	bool m_freshImage;

	Task m_imageStreamTask;

	bhs_PCDataServer* m_dataServer;
};

#if JAVA_CAMERA_LIB == 1
#ifdef __cplusplus
extern "C" {
#endif
	void AxisCameraStart(const char *IPAddress);
	int AxisCameraGetImage(Image *image);
	void AxisCameraDeleteInstance();
	int AxisCameraFreshImage();

	// Mid-stream gets & writes
	void AxisCameraWriteBrightness(int brightness);
	int AxisCameraGetBrightness();
	void AxisCameraWriteWhiteBalance(bhs_AxisCameraParams2::WhiteBalance_t whiteBalance);
	bhs_AxisCameraParams2::WhiteBalance_t AxisCameraGetWhiteBalance();
	void AxisCameraWriteColorLevel(int colorLevel);
	int AxisCameraGetColorLevel();
	void AxisCameraWriteExposureControl(bhs_AxisCameraParams2::Exposure_t exposure);
	bhs_AxisCameraParams2::Exposure_t AxisCameraGetExposureControl();
	void AxisCameraWriteExposurePriority(int exposurePriority);
	int AxisCameraGetExposurePriority();
	void AxisCameraWriteMaxFPS(int maxFPS);
	int AxisCameraGetMaxFPS();

	// New-Stream gets & writes
	void AxisCameraWriteResolution(bhs_AxisCameraParams2::Resolution_t resolution);
	bhs_AxisCameraParams2::Resolution_t AxisCameraGetResolution();
	void AxisCameraWriteCompression(int compression);
	int AxisCameraGetCompression();
	void AxisCameraWriteRotation(bhs_AxisCameraParams2::Rotation_t rotation);
	bhs_AxisCameraParams2::Rotation_t AxisCameraGetRotation();
#ifdef __cplusplus
}
#endif
#endif // JAVA_CAMERA_LIB == 1

#endif
