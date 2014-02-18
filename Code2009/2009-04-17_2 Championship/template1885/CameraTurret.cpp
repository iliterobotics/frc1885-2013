/*
 * CameraTurret.cpp
 *
 *  Created on: Jan 22, 2009
 *      Author: ITRobo
 */

#include "CameraTurret.h"
#include "RobotGlobalData.h"

/*
 * Default constructor
 */
CameraTurret::CameraTurret()
{
  
}

/*
 * Default destructor
 */
CameraTurret::~CameraTurret()
{
  
}

void CameraTurret::setTiltAngle(RobotGlobalData* apData, float anCameraTiltInput)
{
  bhs_CameraTurretData lcCameraTurretData;
  
  apData->getCameraTurretData(lcCameraTurretData);
  
  if (TiltOffSet>=0)
  {
    lcCameraTurretData.mnCameraTurretTiltDegree = anCameraTiltInput-TiltOffSet;
  }
  else
  {
    lcCameraTurretData.mnCameraTurretTiltDegree = anCameraTiltInput+TiltOffSet;
  }
  
  apData->setCameraTurretData(lcCameraTurretData);
}

void CameraTurret::setPanAngle(RobotGlobalData* apData, float anCameraPanInput)
{
  bhs_CameraTurretData lcCameraTurretData;
  
  apData->getCameraTurretData(lcCameraTurretData);
  
  if (PanOffSet>=0)
  {
    lcCameraTurretData.mnCameraTurretPanDegree = anCameraPanInput-PanOffSet;
  }
  else
  {
    lcCameraTurretData.mnCameraTurretPanDegree = anCameraPanInput+PanOffSet;
  }
  
  apData->setCameraTurretData(lcCameraTurretData);
}
