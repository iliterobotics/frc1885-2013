#include "RobotGlobalData.h"
#include "VisionAPI.h"

/*
 * Add to bhs_Constants.h
 *   // *********Camera Constants*******
 static const float cnNUM_DEGREES_VERT = 40.5;
 static const float cnNUM_DEGREES_HORIZ = 54;
 static const float cnHEIGHT_OF_TRAILER = 71;
 static const float cnHEIGHT_OF_CAMERA = 0;*/

struct DistanceData
{
	float mnDistance;
	float mnAngle;
};

struct CameraDistances
{
  static const int numSpecified = 10;
  static const float maCalculatedDistance[numSpecified];
  static const float maActualDistance[numSpecified];
};

class DistanceCalculator
{
  private:
    //Resolution of the image from the camera
    int mnResolutionX;
    int mnResolutionY;

    float mnHorizDegreesPerPixel;
    float mnVertDegreesPerPixel;
    float mnHeightDifference;
    float mnYCenterOfTarget;
    float mnXCenterOfTarget;
    float mnAbsoluteVertAngle;
    float mnTargetDistance;
    float mnHorizDirection;

    

  public:
    DistanceCalculator();
    float getDistance();
    float getXDirection();
    void test();
    
    DistanceData calcDistance(ParticleAnalysisReport *apReport, float anPan, float anTilt);

};

