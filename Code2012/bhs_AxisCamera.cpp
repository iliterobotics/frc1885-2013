/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/
#include "bhs_AxisCamera.h"

#define IVA_MAX_BUFFERS 10
#define VisionErrChk(Function) {if (!(Function)) {success = 0; goto Error;}}

//

/** Private NI function to decode JPEG */
IMAQ_FUNC int Priv_ReadJPEGString_C(Image* _image, const unsigned char* _string, UINT32 _stringLength);

// Max packet without jumbo frames is 1500... add 36 because??
#define kMaxPacketSize 1536
#define kImageBufferAllocationIncrement 1000

bhs_AxisCamera *bhs_AxisCamera::_instance = NULL;

/**
 * bhs_AxisCamera constructor
 */

bhs_AxisCamera::bhs_AxisCamera(const char *ipAddress = "10.18.85.11")
	: AxisCameraParams(ipAddress)
	, m_cameraSocket(ERROR)
	, m_protectedImageBuffer(NULL)
	, m_protectedImageBufferLength(0)
	, m_protectedImageSize(0)
	, m_protectedImageSem(NULL)
	, m_freshImage(false)
	, mDataServer(NULL)
	, mCameraData(NULL)
	//, mImageProcessor()
	//, mVision()
{
	mbProcessImage = false;
	mcPrcessImage = frcCreateImage(IMAQ_IMAGE_HSL);
	proImage2 = frcCreateImage(IMAQ_IMAGE_HSL);
}

void bhs_AxisCamera::init(bhs_CameraData* aCameraData, bhs_PCDataServer* apDataServer) {
	m_protectedImageSem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
	m_dataSem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
	
	mDataServer =apDataServer;
	//	printf("Camera Constructed\n");
	
		WriteMaxFPS(2);
		taskSpawn ("cameraTask", 101, 0x8, 20000, (FUNCPTR)bhs_AxisCamera::s_ImageStreamTaskFunction,(int)this,0,0,0,0,0,0,0,0,0);
		//taskSpawn ("BHS_CameraProcessing", 102, 0x8, 200000, (FUNCPTR)bhs_AxisCamera::s_ImageProcessTaskFunction,(int)this,0,0,0,0,0,0,0,0,0);
		m_dataSem = semBCreate(0,SEM_FULL);
		m_processSem = semBCreate(0,SEM_EMPTY);
		m_proImageSize = 200000;
		proImage = new char [m_proImageSize];
		proImage2 = frcCreateImage(IMAQ_IMAGE_HSL);
		mcPrcessImage = frcCreateImage(IMAQ_IMAGE_HSL);
		timer = new Timer();
		
		
		if(aCameraData == NULL) {
			printf("Warning: Camera Data is null.");
		}
		else {
			mCameraData = aCameraData;
			mCameraData->initActualHoops();
		}
}


typedef enum IVA_ResultType_Enum {IVA_NUMERIC, IVA_BOOLEAN, IVA_STRING} IVA_ResultType;

	typedef union IVA_ResultValue_Struct    // A result in Vision Assistant can be of type double, BOOL or string.
	{
	    double numVal;
	    bool   boolVal;
	    char*  strVal;
	} IVA_ResultValue;

	typedef struct IVA_Result_Struct
	{
	    IVA_ResultType  type;           // Result type
	    IVA_ResultValue resultVal;     // Result value
	} IVA_Result;

	typedef struct IVA_StepResultsStruct
	{
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


static IVA_Data* IVA_InitData(int numSteps, int numCoordSys);
	
int bhs_AxisCamera::IVA_ProcessImage(Image *image, bhs_AxisCamera* camera)
{
	int success = 1;
	IVA_Data *ivaData;

	    int pKernel[49] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	    StructuringElement structElem;
	    int pPixelMeasurements[10] = {0,1,4,5,6,7,85,86,87,88};
	    int *pCalibratedMeasurements = 0;

	    // Initializes internal data (buffers and array of points for caliper measurements)
	    ivaData = (bhs_AxisCamera::IVA_Data_Struct*)IVA_InitData(8, 0);
	    Image* l_ThresholdImage = frcCreateImage(IMAQ_IMAGE_HSL);
		l_ThresholdImage = IVA_CLRThreshold(image, 49, 137, 33, 255, 110, 255, IMAQ_HSL, this);
		if(l_ThresholdImage == NULL)
		{
	    	frcDispose(l_ThresholdImage);
	    	IVA_DisposeData(ivaData);
	    	ivaData =  NULL;
	    	printf("Error thresholding the image\n");
	    	return 0;
	    }
		frcDispose(image);
		image = NULL;

	    //-------------------------------------------------------------------//
	    //                          Basic Morphology                         //
	    //-------------------------------------------------------------------//

	    // Sets the structuring element.
	    structElem.matrixCols = 3;
	    structElem.matrixRows = 3;
	    structElem.hexa = FALSE;
	    structElem.kernel = pKernel;

	    // Applies a morphological transformation to the binary image.
	    Image* l_ClosedImage = frcCreateImage(IMAQ_IMAGE_U8);
	    if(imaqMorphology(l_ClosedImage, l_ThresholdImage, IMAQ_CLOSE, &structElem))
	    {
	    frcDispose(l_ThresholdImage);
	    l_ThresholdImage = NULL;
	    if(l_ClosedImage == NULL)
	    {
	    	frcDispose(l_ClosedImage);
	    	IVA_DisposeData(ivaData);
	    	ivaData =  NULL;
 	    	printf("Error thresholding the image\n");
 	    	return 0;
  	    }
	    //Wait until the template image is ftp'ed onto the cRIO before calling MatchShape
	    IVA_MatchShape(l_ClosedImage,"C:/vision/RectangleTemplate.png", 600, TRUE,ivaData, 2);
	IVA_Particle(l_ClosedImage, TRUE, pPixelMeasurements, 10, pCalibratedMeasurements, 0, ivaData, 5);
	
	frcDispose(l_ClosedImage);
	l_ClosedImage = NULL;
	
    // Releases the memory allocated in the IVA_Data structure.
    IVA_DisposeData(ivaData);
    ivaData =  NULL;
 //   mCameraData->lineUpDataUpdate();

	return success;
	
	}
	    printf("Failed to process image");
	    IVA_DisposeData(ivaData);
	    ivaData =  NULL;
	    frcDispose(l_ClosedImage);
	    l_ClosedImage = NULL;
	    frcDispose(l_ThresholdImage);
	    l_ThresholdImage = NULL;
	    return 0;
}

void bhs_AxisCamera::IVA_MatchShape(Image* image,
                                   char* templatePath,
                                   double minimumScore,
                                   int scaleInvariance,
                                   IVA_Data* ivaData,
                                   int stepIndex)
{
    Image* shapeImage;
    Image* imageTemplate;
    int i;
    short lookupTable[256];
    ShapeReport* shapeReport = NULL;
    int numMatchesFound;



    //-------------------------------------------------------------------//
    //                           Shape Matching                          //
    //-------------------------------------------------------------------//

    // Creates a temporary image that will be used to perform the search.
    shapeImage = imaqCreateImage(IMAQ_IMAGE_U8, 7);

    // Applies a lookup table to the image because the input image for the
    // imaqMatchShape fucntion must be a binary image that contains only
    // pixel values of 0 or 1

    lookupTable[0] = 0;
    for (i = 1 ; i < 256 ; i++)
        lookupTable[i] = 1;

    if(!imaqLookup(shapeImage, image, lookupTable, NULL))
    	{
    		printf("ImaqLookUp Error\n");
    	}
    // Creates and read the image template containing the shape to match
    imageTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 7);
    if(!imaqReadFile(imageTemplate, templatePath, NULL, NULL))
    	{
    		printf("Template read incorrectly\n");
    	}

    // Applies the same lookup table to obtain an image containing only pixel
    // values of 0 and 1.
    if(!imaqLookup(imageTemplate, imageTemplate, lookupTable, NULL))
	{
    		printf("ImaqLookUp Error\n");
    }
    // Finds the shape in the binary image.
    shapeReport = imaqMatchShape(shapeImage, shapeImage, imageTemplate, scaleInvariance, 1, 0.5, &numMatchesFound);
    
    mCenterPoints.clear();
    int shapes = 0;
    for (i = 0 ; i < numMatchesFound ; i++)
    {
        if (shapeReport[i].score >= minimumScore)
        {
        	mCenterPoints.push_back(Point(shapeReport[i].centroid));
        	shapes++;
        }
    }
    printf("Shapes: %d\n",shapes);
    frcDispose(shapeImage);
    frcDispose(imageTemplate);
    shapeImage = NULL;
    imageTemplate = NULL;
    free(shapeReport);
    shapeReport = NULL;
    IVA_DisposeStepResults(ivaData, stepIndex); 
}

////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_CLRThreshold
//
// Description  : Thresholds a color image.
//
// Parameters   : image      -  Input image
//                min1       -  Minimum range for the first plane
//                max1       -  Maximum range for the first plane
//                min2       -  Minimum range for the second plane
//                max2       -  Maximum range for the second plane
//                min3       -  Minimum range for the third plane
//                max3       -  Maximum range for the third plane
//                colorMode  -  Color space in which to perform the threshold
//
// Return Value : thresholdImage
//
////////////////////////////////////////////////////////////////////////////////
Image* bhs_AxisCamera::IVA_CLRThreshold(Image* image, int min1, int max1, int min2, int max2, int min3, int max3, int colorMode, bhs_AxisCamera* camera)
{
	bool lbProcessingGood = true;
    Image* thresholdImage = NULL;
    Range plane1Range;
    Range plane2Range;
    Range plane3Range;


    //-------------------------------------------------------------------//
    //                          Color Threshold                          //
    //-------------------------------------------------------------------//

    // Creates an 8 bit image for the thresholded image.
//    printf("Threshhold Image creation:%d\n",VisionErrChk(thresholdImage = imaqCreateImage(IMAQ_IMAGE_U8, 7)));
    thresholdImage = imaqCreateImage(IMAQ_IMAGE_U8, 7);
    if(thresholdImage == NULL)
    {
    	printf("IVA_CLRThreshold: Error imaqCreateImage\n");
    	lbProcessingGood = false;
    }
    else
    {
    // Set the threshold range for the 3 planes.
    plane1Range.minValue = min1;
    plane1Range.maxValue = max1;
    plane2Range.minValue = min2;
    plane2Range.maxValue = max2;
    plane3Range.minValue = min3;
    plane3Range.maxValue = max3;

    // Thresholds the color image.
    int lntCtStatus;
     if( (lntCtStatus = frcColorThreshold(thresholdImage,image,1,IMAQ_HSL,&plane1Range, &plane2Range, &plane3Range)) == 0)
     {
    	 lbProcessingGood = false;
    	 int lnLastErrorNum = imaqGetLastError();
    	 printf("IVA_CLRThreshold: Error frcColorThreshold status: %d 0x%X %s\n",lntCtStatus,lnLastErrorNum,imaqGetErrorText(lnLastErrorNum));
     }
     
 //    imaqColorThreshold(thresholdImage, image, 1, IMAQ_HSL, &plane1Range, &plane2Range, &plane3Range);
// Copies the threshold image in the souce image.
    //imaqDuplicate(image, thresholdImage);
 //   printf("Threshold Complete\n");

    }
    
	if(lbProcessingGood == false)
	{
		printf("Error finding the image threshold\n");
		if(thresholdImage == NULL)
		{
			imaqDispose(thresholdImage);
			thresholdImage = NULL;
		}
	}

    return thresholdImage;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_Particle
//
// Description  : Computes the number of particles detected in a binary image and
//                a 2D array of requested measurements about the particle.
//
// Parameters   : image                      -  Input image
//                connectivity               -  Set this parameter to 1 to use
//                                              connectivity-8 to determine
//                                              whether particles are touching.
//                                              Set this parameter to 0 to use
//                                              connectivity-4 to determine
//                                              whether particles are touching.
//                pixelMeasurements          -  Array of measuremnets parameters
//                numPixelMeasurements       -  Number of elements in the array
//                calibratedMeasurements     -  Array of measuremnets parameters
//                numCalibratedMeasurements  -  Number of elements in the array
//                ivaData                    -  Internal Data structure
//                stepIndex                  -  Step index (index at which to store
//                                              the results in the resuts array)
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int bhs_AxisCamera::IVA_Particle(Image* image,
                                 int connectivity,
                                 int pPixelMeasurements[],
                                 int numPixelMeasurements,
                                 int pCalibratedMeasurements[],
                                 int numCalibratedMeasurements,
                                 IVA_Data* ivaData,
                                 int stepIndex)
{
    int success = 1;
    int numParticles;//, tempNumParticles;
    double* pixelMeasurements = NULL;
    double* calibratedMeasurements = NULL;
    IVA_Result* particleResults;
    int i;


    //-------------------------------------------------------------------//
    //                         Particle Analysis                         //
    //-------------------------------------------------------------------//

    // Counts the number of particles in the image.
    if(!imaqCountParticles(image, connectivity, &numParticles))
    	printf("ImaqCountParticles has failed.\n");
    
    // Allocate the arrays for the measurements.
 //   printf("%d\n",numParticles);
    pixelMeasurements = (double*)malloc(numParticles * numPixelMeasurements * sizeof(double));
    calibratedMeasurements = (double*)malloc(numParticles * numCalibratedMeasurements * sizeof(double));
    // Delete all the results of this step (from a previous iteration)
    IVA_DisposeStepResults(ivaData, stepIndex);

	if(!success)
		printf("Error with IMAQ functions while processing image.\n");
	
	if(numParticles>0)
	{
		vector<Point>::iterator it;
		bool isRectangle;
		double xVal = 0;
		double yVal = 0;
		
		for (i = 0 ; i < numParticles ; i++)
		{
			imaqMeasureParticle(image, i, FALSE, IMAQ_MT_CENTER_OF_MASS_X, &xVal);
			imaqMeasureParticle(image, i, FALSE, IMAQ_MT_CENTER_OF_MASS_Y, &yVal);
			isRectangle = false;
			for(it = mCenterPoints.begin();it!=mCenterPoints.end();it++)
			{
				if((int)(*it).x == (int)xVal && (int) (*it).y == (int)yVal)
				{
//					printf("Rectangle confirmed\n");
					isRectangle = true;
					break;
				}
			}
			if(isRectangle)
			{
				bhs_CameraData::camData lcData;
				double tempValue = 0;
				lcData.centerX = (int) xVal;
				lcData.centerY = (int) yVal;
				imaqMeasureParticle(image, i, FALSE, IMAQ_MT_BOUNDING_RECT_LEFT, &tempValue);
				lcData.leftBound = (int) tempValue;
				imaqMeasureParticle(image, i, FALSE, IMAQ_MT_BOUNDING_RECT_RIGHT, &tempValue);
				lcData.rightBound = (int) tempValue;
				imaqMeasureParticle(image, i, FALSE, IMAQ_MT_BOUNDING_RECT_TOP, &tempValue);
				lcData.topBound = (int) tempValue;
				imaqMeasureParticle(image, i, FALSE, IMAQ_MT_BOUNDING_RECT_BOTTOM, &tempValue);
				lcData.bottomBound = (int) tempValue;
				lcData.rectHeight = lcData.bottomBound - lcData.topBound;
				lcData.rectWidth = lcData.rightBound - lcData.leftBound;
				
				lcData.filled = true;
				if(lcData.rectWidth > 30 && lcData.rectHeight > 30)
				{
					printf("X: %d, Y: %d, Height: %d, Width: %d\n",lcData.centerX,lcData.centerY,lcData.rectHeight,lcData.rectWidth);
					mHoop.push_back(lcData);
				}
			}
			
		}
		
	}
	
	bhs_HoopInformation::fixImageData(mHoop,mCameraData);
	
	if(pixelMeasurements != NULL)
	{
		free(pixelMeasurements);
		pixelMeasurements = NULL;
	}
	if(calibratedMeasurements!=NULL)
	{
		free(calibratedMeasurements);
		calibratedMeasurements = NULL;
	}
	if(particleResults != NULL)
	{
		free(particleResults);
		particleResults = NULL;
	}
    mHoop.clear();

    return success;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_InitData
//
// Description  : Initializes data for buffer management and results.
//
// Parameters   : # of steps
//                # of coordinate systems
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
IVA_Data_Struct* IVA_InitData(int numSteps, int numCoordSys)
{
    int success = 1;
    IVA_Data* ivaData = NULL;
    int i;


    // Allocate the data structure.
    ivaData = (IVA_Data*)malloc(sizeof (IVA_Data));

    // Initializes the image pointers to NULL.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        ivaData->buffers[i] = NULL;

    // Initializes the steo results array to numSteps elements.
    ivaData->numSteps = numSteps;

    ivaData->stepResults = (IVA_StepResults*)malloc(ivaData->numSteps * sizeof(IVA_StepResults));
    for (i = 0 ; i < numSteps ; i++)
    {

        ivaData->stepResults[i].numResults = 0;
        ivaData->stepResults[i].results = NULL;
    }

    // Create the coordinate systems
	ivaData->baseCoordinateSystems = NULL;
	ivaData->MeasurementSystems = NULL;
	if (numCoordSys)
	{
		ivaData->baseCoordinateSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
		ivaData->MeasurementSystems = (CoordinateSystem*)malloc(sizeof(CoordinateSystem) * numCoordSys);
	}

    ivaData->numCoordSys = numCoordSys;

	if(!success)
		printf("Initialization Error\n");
    return ivaData;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeData
//
// Description  : Releases the memory allocated in the IVA_Data structure
//
// Parameters   : ivaData  -  Internal data structure
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int bhs_AxisCamera::IVA_DisposeData(IVA_Data* ivaData)
{
    int i;


    // Releases the memory allocated for the image buffers.
    for (i = 0 ; i < IVA_MAX_BUFFERS ; i++)
        imaqDispose(ivaData->buffers[i]);

    // Releases the memory allocated for the array of measurements.
    for (i = 0 ; i < ivaData->numSteps ; i++)
        IVA_DisposeStepResults(ivaData, i);

    free(ivaData->stepResults);

    // Dispose of coordinate systems
    if (ivaData->numCoordSys)
    {
        free(ivaData->baseCoordinateSystems);
        free(ivaData->MeasurementSystems);
    }

    free(ivaData);

    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function Name: IVA_DisposeStepResults
//
// Description  : Dispose of the results of a specific step.
//
// Parameters   : ivaData    -  Internal data structure
//                stepIndex  -  step index
//
// Return Value : success
//
////////////////////////////////////////////////////////////////////////////////
int bhs_AxisCamera::IVA_DisposeStepResults(IVA_Data* ivaData, int stepIndex)
{
    int i;

    
    for (i = 0 ; i < ivaData->stepResults[stepIndex].numResults ; i++)
    {
        if (ivaData->stepResults[stepIndex].results[i].type == IVA_STRING)
            free(ivaData->stepResults[stepIndex].results[i].resultVal.strVal);
    }

    free(ivaData->stepResults[stepIndex].results);

    return TRUE;
}


int MatchShape(Image* camImage, char* templatePath, int minimumScore, int scaleInvariance, int* coordinates)
{
//		printf("Shape matching\n");
		Image* imageTemplate;
		short lookupTable[256];
		int i;
		ShapeReport* shapeReport;
		int* numMatchesFound = 0;
		int numMatches = 0;
		int coord = 0;
		int n = 0;
		double highest = 0;
		Image* tempIm;
		
	
		lookupTable[0] = 0;
	    for (i = 1 ; i < 256 ; i++)
	        lookupTable[i] = 1;
	    imageTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 7);
	    imaqReadFile(imageTemplate, templatePath, NULL, NULL);

	    if(camImage==NULL)
	    	printf("Camera Image Not Correct\n");
	    if(imageTemplate==NULL)
	    	printf("Template Image not correct\n");
	//  if(scale)

	    // Finds the shape in the binary image.
//	    shapeReport = imaqMatchShape(tempIm, camImage, imageTemplate, scaleInvariance, 1, 0.5, &numMatchesFound);
	    if(imaqMatchShape(tempIm,camImage,imageTemplate,scaleInvariance,1,0.01,numMatchesFound)==NULL)
	    	printf("\n\n\n\n\n");

	    // Log the results in the points array for future caliper operations.
	//    printf("Number of matches found: %d\n", *numMatchesFound);
	    for (i = 0 ; i < *numMatchesFound ; i++)
	    {
	        if (shapeReport[i].score >= minimumScore)
	        {    
	        	numMatches++;
	        	coordinates[coord] = shapeReport[n].coordinates.width;
	        	coord++;
	        	coordinates[coord] = shapeReport[n].centroid.x;
	        	coord++;
	        	coordinates[coord] = shapeReport[n].centroid.y;
	        	coord++;
	        }
	        if(shapeReport[i].score > highest)
	        	highest = shapeReport[i].score;
	    }
	 //   printf("Highest:%f\n",highest);
	return numMatches;
}
//Function modified from the NI Vision Assistant


/**
 * Destructor
 */
bhs_AxisCamera::~bhs_AxisCamera()
{	

//	m_imageStreamTask.Stop();
	close(m_cameraSocket);

	SemSet_t::iterator it = m_newImageSemSet.begin();
	SemSet_t::iterator end = m_newImageSemSet.end();
	for (;it != end; it++)
	{
		semDelete(*it);
	}
	m_newImageSemSet.clear();

	semDelete(m_protectedImageSem);
}

/**
 * Get a pointer to the bhs_AxisCamera object, if the object does not exist, create it
 * To use the camera on port 2 of a cRIO-FRC, pass "192.168.0.90" to the first GetInstance call.
 * @return reference to bhs_AxisCamera object
 */
bhs_AxisCamera* bhs_AxisCamera::GetInstance(const char *cameraIP)
{
	if (NULL == _instance)
	{
		_instance = new bhs_AxisCamera();
		
	}

	return _instance;
}

/**
 * Called by Java to delete the camera... how thoughtful
 */
void bhs_AxisCamera::DeleteInstance()
{
	delete _instance;
	_instance = NULL;
}

/**
 * Return true if the latest image from the camera has not been retrieved by calling GetImage() yet.
 * @return true if the image has not been retrieved yet.
 */
bool bhs_AxisCamera::IsFreshImage()
{
	return m_freshImage;
}

/**
 * Get the semaphore to be used to synchronize image access with camera acquisition
 *
 * Call semTake on the returned semaphore to block until a new image is acquired.
 *
 * The semaphore is owned by the bhs_AxisCamera class and will be deleted when the class is destroyed.
 * @return A semaphore to notify when new image is received
 */
SEM_ID bhs_AxisCamera::GetNewImageSem()
{
	SEM_ID sem = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
	m_newImageSemSet.insert(sem);
	return sem;
}

/**
 * Get an image from the camera and store it in the provided image.
 * @param image The imaq image to store the result in. This must be an HSL or RGB image
 * This function is called by Java.
 * @return 1 upon success, zero on a failure
 */
int bhs_AxisCamera::GetImage(Image* imaqImage)
{
	if (m_protectedImageBuffer == NULL)
		return 0;
//	printf("Getting the Image\n");
/*	int lnStatus =*/ Priv_ReadJPEGString_C(imaqImage,
		(unsigned char*)m_protectedImageBuffer, m_protectedImageSize);
//	printf("bhs_AxisCamera::GetImage lnStatus = %d\n",lnStatus);
	m_freshImage = false;
	return 1;
}

#if JAVA_CAMERA_LIB != 1
/**
 * Get an image from the camera and store it in the provided image.
 * @param image The image to store the result in. This must be an HSL or RGB image
 * @return 1 upon success, zero on a failure
 */
int bhs_AxisCamera::GetImage(ColorImage* image)
{
	return GetImage(image->GetImaqImage());
}

/**
 * Instantiate a new image object and fill it with the latest image from the camera.
 *
 * The returned pointer is owned by the caller and is their responsibility to delete.
 * @return a pointer to an HSLImage object
 */
HSLImage* bhs_AxisCamera::GetImage()
{
	HSLImage *image = new HSLImage();
	GetImage(image);
	return image;
}
#endif

/**
 * Copy an image into an existing buffer.
 * This copies an image into an existing buffer rather than creating a new image
 * in memory. That way a new image is only allocated when the image being copied is
 * larger than the destination.
 * This method is called by the PCVideoServer class.
 * @param imageData The destination image.
 * @param numBytes The size of the destination image.
 * @return 0 if failed (no source image or no memory), 1 if success.
 */
int bhs_AxisCamera::CopyJPEG(char **destImage, int &destImageSize, int &destImageBufferSize)
{
	Synchronized sync(m_protectedImageSem);
	if (destImage == NULL)
		wpi_setWPIErrorWithContext(NullParameter, "destImage must not be NULL");

	if (m_protectedImageBuffer == NULL || m_protectedImageSize <= 0)
		return 0; // if no source image

	if (destImageBufferSize < m_protectedImageSize) // if current destination buffer too small
	{
		if (*destImage != NULL) delete [] *destImage;
		destImageBufferSize = m_protectedImageSize + kImageBufferAllocationIncrement;
		*destImage = new char[destImageBufferSize];
		if (*destImage == NULL) return 0;
	}
	// copy this image into destination buffer
	if (*destImage == NULL)
	{
		wpi_setWPIErrorWithContext(NullParameter, "*destImage must not be NULL");
	}
	// TODO: Is this copy realy necessary... perhaps we can simply transmit while holding the protected buffer
	memcpy(*destImage, m_protectedImageBuffer, m_protectedImageSize);
	destImageSize = m_protectedImageSize;
	return 1;
}

/**
 * Static interface that will cause an instantiation if necessary.
 * This static stub is directly spawned as a task to read images from the camera.
 */
void bhs_AxisCamera::s_ImageStreamTaskFunction(bhs_AxisCamera *thisPtr)
{
//	printf("bhs_AxisCamera Task Beginning\n");
	thisPtr->ImageStreamTaskFunction();
}

/**
 * Task spawned by bhs_AxisCamera constructor to receive images from cam
 * If setNewImageSem has been called, this function does a semGive on each new image
 * Images can be accessed by calling getImage()
 */
int bhs_AxisCamera::ImageStreamTaskFunction()
{
	//printf("About to enter loop\n");
	// Loop on trying to setup the camera connection. This happens in a background
	// thread so it shouldn't effect the operation of user programs.
	while (1)
	{
		const char *requestString = "GET /mjpg/video.mjpg HTTP/1.1\n\
User-Agent: HTTPStreamClient\n\
Connection: Keep-Alive\n\
Cache-Control: no-cache\n\
Authorization: Basic RlJDOkZSQw==\n\n";
//		printf("About to take the Sem");
		semTake(m_socketPossessionSem, WAIT_FOREVER);
		m_cameraSocket = CreateCameraSocket(requestString);
		if (m_cameraSocket == ERROR)
		{
			// Don't hammer the camera if it isn't ready.
			semGive(m_socketPossessionSem);
			taskDelay(1000);
		}
		else
		{
//			printf("Call ReadImages\n");
			ReadImagesFromCamera();
		}
	}
}

/**
 * This function actually reads the images from the camera.
 */
int bhs_AxisCamera::ReadImagesFromCamera()
{
	char *imgBuffer = NULL;
	int imgBufferLength = 0;
	//Infinite loop, task deletion handled by taskDeleteHook
	// Socket cleanup handled by destructor

	// TODO: these recv calls must be non-blocking. Otherwise if the camera
	// fails during a read, the code hangs and never retries when the camera comes
	// back up.

	int counter = 2;
	while (1)
	{
		char initialReadBuffer[kMaxPacketSize];
		char intermediateBuffer[1];
		char *trailingPtr = initialReadBuffer;
		int trailingCounter = 0;
		initialReadBuffer[0] = 0;
		while (counter)
		{
			// TODO: fix me... this cannot be the most efficient way to approach this, reading one byte at a time.
			if(recv(m_cameraSocket, intermediateBuffer, 1, 0) == ERROR)
			{
				wpi_setErrnoErrorWithContext("Failed to read image header");
				close (m_cameraSocket);
				return ERROR;
			}
			strncat(initialReadBuffer, intermediateBuffer, 1);
			// trailingCounter ensures that we start looking for the 4 byte string after
			// there is at least 4 bytes total. Kind of obscure.
			// look for 2 blank lines (\r\n)
			if (NULL != strstr(trailingPtr, "\r\n\r\n"))
			{
				--counter;
			}
			if (++trailingCounter >= 4)
			{
				trailingPtr++;
			}
		}
		counter = 1;
		char *contentLength = strstr(initialReadBuffer, "Content-Length: ");
		if (contentLength == NULL)
		{
			wpi_setWPIErrorWithContext(IncompatibleMode, "No content-length token found in packet");
			close(m_cameraSocket);
			return ERROR;
		}
		contentLength = contentLength + 16; // skip past "content length"
		int readLength = atol(contentLength); // get the image byte count

		// Make sure buffer is large enough
		if (imgBufferLength < readLength)
		{
			if (imgBuffer) delete[] imgBuffer;
			imgBufferLength = readLength + kImageBufferAllocationIncrement;
			imgBuffer = new char[imgBufferLength];
			if (imgBuffer == NULL)
			{
				imgBufferLength = 0;
				continue;
			}
		}

		// Read the image data for "Content-Length" bytes
		int bytesRead = 0;
		int remaining = readLength;
		while(bytesRead < readLength)
		{
			int bytesThisRecv = recv(m_cameraSocket, &imgBuffer[bytesRead], remaining, 0);
			bytesRead += bytesThisRecv;
			remaining -= bytesThisRecv;
		}
		// Update image
//		printf("ReadImageFromCamera: UpdatePublicImage\n");
		UpdatePublicImageFromCamera(imgBuffer, readLength);
		if (semTake(m_paramChangedSem, NO_WAIT) == OK)
		{
			// params need to be updated: close the video stream; release the camera.
			close(m_cameraSocket);
			semGive(m_socketPossessionSem);
			return 0;
		}
	}
}

/**
 * Copy the image from private buffer to shared buffer.
 * @param imgBuffer The buffer containing the image
 * @param bufLength The length of the image
 */
void bhs_AxisCamera::UpdatePublicImageFromCamera(char *imgBuffer, int imgSize)
{
	/*{
		Synchronized sync(m_protectedImageSem);

		// Adjust the buffer size if current destination buffer is too small.
		if (m_protectedImageBufferLength < imgSize)
		{
			if (m_protectedImageBuffer != NULL) delete [] m_protectedImageBuffer;
			m_protectedImageBufferLength = imgSize + kImageBufferAllocationIncrement;
			m_protectedImageBuffer = new char[m_protectedImageBufferLength];
			if (m_protectedImageBuffer == NULL)
			{
				m_protectedImageBufferLength = 0;
				return;
			}
		}

		memcpy(m_protectedImageBuffer, imgBuffer, imgSize);
		m_protectedImageSize = imgSize;
	}*/
//	printf("Ready to sendImage");
	m_freshImage = true;
	SendImage(imgBuffer, imgSize);
	//GetImage(proImage2);
//	SendImage(proImage2, imgSize);
	if(mbProcessImage)
	{
		//timer->Reset();
		//timer->Start();
		if(mCameraData->m_runCameraProcessing)
		{
			ProcessImage(imgBuffer, imgSize);
		}
		//printf("Image Processing Time:%f\n",timer->Get());
		//mbProcessImage = false;
	}
	// Notify everyone who is interested.
	SemSet_t::iterator it = m_newImageSemSet.begin();
	SemSet_t::iterator end = m_newImageSemSet.end();
	for (;it != end; it++)
	{
		semGive(*it);
	}
}
/*void bhs_AxisCamera::SendImage(Image* image)
{
//	Priv_ReadJPEGString_C(image,(unsigned char*)m_protectedImageBuffer, m_protectedImageSize);
//	if(mDataServer != NULL)
//		mDataServer->xmitImage(m_protectedImageBuffer, m_protectedImageSize);
//	printf("Sending Image\n");
}*/
void bhs_AxisCamera::SendImage(char* image, int imageSize)
{
//	printf("Image Send Attempted\n");
	if(mDataServer != NULL)
	{
		mDataServer->xmitImage(image, imageSize);
//		printf("Image sent!\n");
	}
	AttemptGiveImageSem();
}
/**
 * Implement the pure virtual interface so that when parameter changes require a restart, the image task can be bounced.
 */
void bhs_AxisCamera::RestartCameraTask()
{
//	m_imageStreamTask.Stop();
//	m_imageStreamTask.Start((int)this);
}

void bhs_AxisCamera::CameraProcessingTask(bhs_AxisCamera* tempThis)
{
	tempThis->CameraSemManage();
}
void bhs_AxisCamera::CameraSemManage()
{/*
//	printf("Camera entering the processing loop:\n");
	while(1)
	{
		semTake(cameraProcessingSem, WAIT_FOREVER);	
//		printf("Semaphore taken\n");
		isProcessBusy=true;
		
//		printf("Image processing done\n");
		isProcessBusy=false;
	}*/
}


//This function is from the NI Vision Assistant.
//Matches a template shapre to objects within the image

void bhs_AxisCamera::AttemptGiveImageSem()
{
//	if(m_proImageSize < m_protectedImageSize)
//	{
//		proImage = new char[m_protectedImageSize];
//	}
//	printf("Attempting to give the sem \n");
/*	if(!isProcessBusy)
	{
		GetImage(proImage2);
//		printf("Giving up the Semaphore\n");
		semGive(cameraProcessingSem);
	}*/
}

void bhs_AxisCamera::ProcessImage(char* imgBuffer, int imgSize)
{
	giveData2ProcessTask(imgBuffer, imgSize);
}
void bhs_AxisCamera::giveData2ProcessTask(char* imgBuffer, int imgSize)
{
//	printf("Preparing to give an Image\n");
	semTake(m_dataSem,WAIT_FOREVER);

	if (m_protectedImageBufferLength < imgSize)
	{
		if (m_protectedImageBuffer != NULL) delete [] m_protectedImageBuffer;
		m_protectedImageBufferLength = imgSize + kImageBufferAllocationIncrement;
		m_protectedImageBuffer = new char[m_protectedImageBufferLength];
		if (m_protectedImageBuffer == NULL)
		{
			m_protectedImageBufferLength = 0;
			return;
		}
	}

	memcpy(m_protectedImageBuffer, imgBuffer, imgSize);
	m_protectedImageSize = imgSize;	

	semGive(m_dataSem);
	semGive(m_processSem);
	//printf("Image Given\n");
}

Image* bhs_AxisCamera::getData4ProcessTask()
{
	Image* lpReturnImage = NULL;
	semTake(m_dataSem,WAIT_FOREVER);
	if(m_protectedImageSize > 0)
	{
	lpReturnImage = frcCreateImage(IMAQ_IMAGE_HSL);
	Priv_ReadJPEGString_C(lpReturnImage,
		(unsigned char*)m_protectedImageBuffer, m_protectedImageSize);
	m_protectedImageSize = 0;
	}
	semGive(m_dataSem);
	
	return lpReturnImage;
}


void bhs_AxisCamera::s_ImageProcessTaskFunction(bhs_AxisCamera *thisPtr)
{
	thisPtr->ProcessImageTask();
}
void bhs_AxisCamera::ProcessImageTask()
{
	Image* lcTMP = frcCreateImage(IMAQ_IMAGE_HSL);
	
	while(semTake(m_processSem,WAIT_FOREVER) == OK)//waiting on semaphore to process data
	{
		lcTMP = getData4ProcessTask();
		if(lcTMP != NULL)
		{
			IVA_ProcessImage(lcTMP, this);
			mCameraData->m_cameraProcessingIteration++;
			if(lcTMP!=NULL)	
			{
				frcDispose(lcTMP);
				lcTMP = NULL;
			}
		}
	}
}


