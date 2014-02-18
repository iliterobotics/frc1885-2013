/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef __BHS_AXIS_CAMERA_H__
#define __BHS_AXIS_CAMERA_H__

#include <taskLib.h>
#include <vxWorks.h> 
#include <sockLib.h> 
#include <inetLib.h>

#include "bhs_PCDataServer.h"
#include "Vision/AxisCameraParams.h"
#include "Vision2009/VisionAPI.h"
#if JAVA_CAMERA_LIB != 1
#include "Vision/ColorImage.h"
#include "Vision/HSLImage.h"
#endif
#include "nivision.h"
#include <set>
#include "Task.h"
#include "bhs_CameraData.h"
#include "Synchronized.h"
#include "WPIErrors.h"
#include "Timer.h"
#include "bhs_HoopInformation.cpp"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <nivision.h>
#include <vector>
using std::vector;
/**
 * bhs_AxisCamera class.
 * This class handles everything about the Axis 206 FRC Camera.
 * It starts up 2 tasks each using a different connection to the camera:
 * - image reading task that reads images repeatedly from the camera
 * - parameter handler task in the base class that monitors for changes to
 *     parameters and updates the camera
 */
class bhs_AxisCamera : public AxisCameraParams
{
private:
	explicit bhs_AxisCamera(const char *cameraIP);
public:
	virtual ~bhs_AxisCamera();
	static bhs_AxisCamera* GetInstance(const char *cameraIP = NULL);
	static void DeleteInstance();
	void init(bhs_CameraData* aCameraData, bhs_PCDataServer* apDataServer);
	
	
	static void CameraProcessingTask(bhs_AxisCamera* selfReference);
	static void s_ImageStreamTaskFunction(bhs_AxisCamera *thisPtr);
	static void s_ImageProcessTaskFunction(bhs_AxisCamera *thisPtr);

	bool IsFreshImage();
	SEM_ID GetNewImageSem();


	int GetImage(Image *imaqImage);
#if JAVA_CAMERA_LIB != 1
	int GetImage(ColorImage *image);
	HSLImage *GetImage();
#endif

	int CopyJPEG(char **destImage, int &destImageSize, int &destImageBufferSize);


private:

	Timer* timer;
	typedef enum IVA_ResultType_Enum {IVA_NUMERIC, IVA_BOOLEAN, IVA_STRING} IVA_ResultType;

	typedef union IVA_ResultValue_Struct    // A result in Vision Assistant can be of type double, BOOL or string.
	{
	    double numVal;
	    bool   boolVal;
	    char*  strVal;
	} IVA_ResultValue;

	typedef struct IVA_Result_Struct
	{
	#if defined (IVA_STORE_RESULT_NAMES)
	    char resultName[256];           // Result name
	#endif
	    IVA_ResultType  type;           // Result type
	    IVA_ResultValue resultVal;     // Result value
	} IVA_Result;

	typedef struct IVA_StepResultsStruct
	{
	#if defined (IVA_STORE_RESULT_NAMES)
	    char stepName[256];             // Step name
	#endif
	    int         numResults;         // number of results created by the step
	    IVA_Result* results;            // array of results
	} IVA_StepResults;

	typedef struct IVA_Data_Struct
	{
	    Image* buffers[10];            // Vision Assistant Image Buffers
	    IVA_StepResults* stepResults;              // Array of step results
	    int numSteps;                               // Number of steps allocated in the stepResults array
	    CoordinateSystem *baseCoordinateSystems;    // Base Coordinate Systems
	    CoordinateSystem *MeasurementSystems;       // Measurement Coordinate Systems
	    int numCoordSys;                            // Number of coordinate systems
	} IVA_Data;
	
	int IVA_ProcessImage(Image *image, bhs_AxisCamera* camera);
	Image* IVA_ParticleFilter(Image* image,int pParameter[],float plower[],float pUpper[],int pCalibrated[],int pExclude[],int criteriaCount,int rejectMatches,int connectivity);
	int IVA_DisposeData(IVA_Data* ivaData);
	int IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex);
	Image* IVA_CLRThreshold(Image* image, int min1, int max1, int min2, int max2, int min3, int max3, int colorMode, bhs_AxisCamera* camera);
	int IVA_Particle(Image* image,
	                                 int connectivity,
	                                 int pPixelMeasurements[],
	                                 int numPixelMeasurements,
	                                 int pCalibratedMeasurements[],
	                                 int numCalibratedMeasurements,
	                                 IVA_Data* ivaData,
	                                 int stepIndex);
	void IVA_MatchShape(Image* image,
	                                   char* templatePath,
	                                   double minimumScore,
	                                   int scaleInvariance,
	                                   IVA_Data* ivaData,
	                                   int stepIndex);
	
	
	int ImageStreamTaskFunction();
	
	void CameraSemManage();
	void ProcessImage(char* imgBuffer,int imgSize);
	void AttemptGiveImageSem();

	int ReadImagesFromCamera();
	void UpdatePublicImageFromCamera(char *imgBuffer, int imgSize);
	void SendImage(char* image, int imageSize);
	void giveData2ProcessTask(char* imgBuffer, int imgSize);
	Image* getData4ProcessTask();
	void ProcessImageTask();
	

	virtual void RestartCameraTask();

	static bhs_AxisCamera *_instance;
	int m_cameraSocket;
	typedef std::set<SEM_ID> SemSet_t;
	SemSet_t m_newImageSemSet;

	char* m_protectedImageBuffer;
	char* proImage;
	Image* proImage2;
	int m_protectedImageBufferLength;
	int m_protectedImageSize;
	SEM_ID m_protectedImageSem;
	SEM_ID m_dataSem;
	SEM_ID m_processSem;
	bool m_freshImage, isProcessBusy;
	int m_proImageSize;
	bool mbProcessImage;
	Image* mcPrcessImage;
	vector<Point> mCenterPoints;
	

	

//	Task m_imageStreamTask;
	
	bhs_PCDataServer* mDataServer;
	bhs_CameraData* mCameraData;
	vector<bhs_CameraData::camData> mHoop;
//	newerImageProcessing* mImageProcessor;
//	VisionAPI mVision;
};

#if JAVA_CAMERA_LIB == 1
#ifdef __cplusplus
extern "C" {
#endif
	void bhs_AxisCameraStart(const char *IPAddress);
	int bhs_AxisCameraGetImage(Image *image);
	void bhs_AxisCameraDeleteInstance();
	int bhs_AxisCameraFreshImage();

	// Mid-stream gets & writes
	void bhs_AxisCameraWriteBrightness(int brightness);
	int bhs_AxisCameraGetBrightness();
	void bhs_AxisCameraWriteWhiteBalance(bhs_AxisCameraParams::WhiteBalance_t whiteBalance);
	bhs_AxisCameraParams::WhiteBalance_t bhs_AxisCameraGetWhiteBalance();
	void bhs_AxisCameraWriteColorLevel(int colorLevel);
	int bhs_AxisCameraGetColorLevel();
	void bhs_AxisCameraWriteExposureControl(bhs_AxisCameraParams::Exposure_t exposure);
	bhs_AxisCameraParams::Exposure_t bhs_AxisCameraGetExposureControl();
	void bhs_AxisCameraWriteExposurePriority(int exposurePriority);
	int bhs_AxisCameraGetExposurePriority();
	void bhs_AxisCameraWriteMaxFPS(int maxFPS);
	int bhs_AxisCameraGetMaxFPS();

	// New-Stream gets & writes
	void bhs_AxisCameraWriteResolution(bhs_AxisCameraParams::Resolution_t resolution);
	bhs_AxisCameraParams::Resolution_t bhs_AxisCameraGetResolution();
	void bhs_AxisCameraWriteCompression(int compression);
	int bhs_AxisCameraGetCompression();
	void bhs_AxisCameraWriteRotation(bhs_AxisCameraParams::Rotation_t rotation);
	bhs_AxisCameraParams::Rotation_t bhs_AxisCameraGetRotation();
#ifdef __cplusplus
}
#endif
#endif // JAVA_CAMERA_LIB == 1

#endif
