#include "DistanceCalculator.h"
#include "bhs_Constants.h"
#include "bhs_Camera.h"
#include "math.h"

const float CameraDistances::maCalculatedDistance[] =
{ 59.056377, 61.93058, 65.727425, 70.724785, 73.085236,
  101.90889, 110.085846, 154.053528, 245.268707, 445.303802 };

const float CameraDistances::maActualDistance[] =
{ 37, 45, 30, 56, 57, 76, 84, 106, 132, 180 };

DistanceCalculator::DistanceCalculator()
{
  switch(bhs_Camera::cnCAMERA_IMAGE_SIZE)
  {
    case bhs_Camera::k160x120:
      mnResolutionX = 160;
      mnResolutionY = 120;
      break;
    case bhs_Camera::k320x240:
      mnResolutionX = 320;
      mnResolutionY = 240;
      break;
    case bhs_Camera::k640x480:
      mnResolutionX = 640;
      mnResolutionY = 480;
      break;
  }
}

DistanceData DistanceCalculator::calcDistance(ParticleAnalysisReport *apReport, float anPan, float anTilt)
{
  mnHorizDegreesPerPixel = bhs_Constants::cnNUM_DEGREES_HORIZ/mnResolutionX;
  mnVertDegreesPerPixel = bhs_Constants::cnNUM_DEGREES_VERT/mnResolutionY;
  mnHeightDifference = fabs(bhs_Constants::cnHEIGHT_OF_TRAILER
                            - bhs_Constants::cnHEIGHT_OF_CAMERA);
  
  mnYCenterOfTarget = apReport->center_mass_y;
  mnXCenterOfTarget = apReport->center_mass_x;
  
  mnAbsoluteVertAngle = anTilt + (mnResolutionY - mnYCenterOfTarget)
                        * mnVertDegreesPerPixel - bhs_Constants::cnNUM_DEGREES_VERT/2;
  
  mnTargetDistance = mnHeightDifference/tan(mnAbsoluteVertAngle * bhs_Constants::PI/180);
  
  mnHorizDirection = anPan + (mnResolutionX - mnXCenterOfTarget)
                     * mnHorizDegreesPerPixel - bhs_Constants::cnNUM_DEGREES_HORIZ/2;
  DistanceData lsData;
  lsData.mnAngle=mnHorizDirection;
  lsData.mnDistance=mnTargetDistance;
  
  for (int i=0; i<CameraDistances::numSpecified-1; i++)
  {
    if (lsData.mnDistance == CameraDistances::maCalculatedDistance[i])
    {
      lsData.mnDistance = CameraDistances::maActualDistance[i];
    }
    else if (lsData.mnDistance > CameraDistances::maCalculatedDistance[i] && lsData.mnDistance < CameraDistances::maCalculatedDistance[i+1])
    {
      double lnCalculatedPerActualDistanceInRange = ((double)CameraDistances::maCalculatedDistance[i+1] - CameraDistances::maCalculatedDistance[i])
                                                    /((double)CameraDistances::maActualDistance[i+1] - CameraDistances::maActualDistance[i]);
      double lnDistanceInRange = (lsData.mnDistance-CameraDistances::maCalculatedDistance[i])/lnCalculatedPerActualDistanceInRange;
      lsData.mnDistance = lnDistanceInRange + CameraDistances::maActualDistance[i];
    }
    else if (lsData.mnDistance == CameraDistances::maCalculatedDistance[i+1])
    {
      lsData.mnDistance = CameraDistances::maActualDistance[i+1];
    }
  }
  if(lsData.mnDistance < CameraDistances::maCalculatedDistance[0])
  {
    lsData.mnDistance = CameraDistances::maActualDistance[0];
  }
  else if(lsData.mnDistance > CameraDistances::maCalculatedDistance[CameraDistances::numSpecified-1])
  {
    lsData.mnDistance = CameraDistances::maActualDistance[CameraDistances::numSpecified-1]+1;
  }
  else
  {
    printf("ERROR: This should never be printed.  Check DistanceCalculator calculations.\n");
    lsData.mnDistance = 0;
  }
  
  return lsData;
}

float DistanceCalculator::getDistance()
{
  return mnTargetDistance;
}

float DistanceCalculator::getXDirection()
{
  return mnHorizDirection;
}
