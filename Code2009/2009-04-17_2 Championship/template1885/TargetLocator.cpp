#include "CameraTurret.h"
#include "TargetLocator.h"
#include <math.h>

static int StartTargetingTask(int apTargeting)
{
  ((TargetLocator*)apTargeting)->targetMainLoop();
  return 0;
}

TargetLocator::TargetLocator()
{
	printf("Begin TargetLocator constructor\n");
  mnTiltAngle = MIN_TILT_ANGLE;
  mnPanAngle = MIN_PAN_ANGLE;
  meCurrentState = eeSCAN;
  mnPanDirection = 1;
  mnTiltDirection = 1;
  
  printf("Completed TargetLocator constructor\n");
}

void TargetLocator::Initialize(RobotGlobalData* apData, CameraTurret *apTurret, SEM_ID apSem)
{
  mpData = apData;
  mpTurret = apTurret;
  mpSem = apSem;
  
  //Start the target tracking task
  mnTargetingTaskId = taskSpawn("Target Tracking",
                                 101,           //task pri.
                                 VX_FP_TASK,       // options
                                 64000,          // stack size
                                 (FUNCPTR)StartTargetingTask, // function to start
                                 (int)this,       // targeting class to start
                                  0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void TargetLocator::targetMainLoop()
{
	int lnDelayCounter=0;
  while (true)
  {
    semTake(mpSem, WAIT_FOREVER);
    switch (meCurrentState)
    {
      case eeSCAN:
        nextAngle();
        meCurrentState = eeSERVO_WAIT;
        break;
      case eeSERVO_WAIT:
        bhs_CameraTurretData lcCameraTurretData;
        
        mpData->getCameraTurretData(lcCameraTurretData);
        
        if (fabs(lcCameraTurretData.mnCameraTurretTiltDegree - lcCameraTurretData.mnCameraTurretActualTiltDegree) < 2 &&
            fabs(lcCameraTurretData.mnCameraTurretPanDegree - lcCameraTurretData.mnCameraTurretActualPanDegree) < 2)
        {
          meCurrentState = eeFIND;
        }
        break;
      case eeFIND:
        //Pattern matching target thingy
        if (0/*found*/)
          meCurrentState = eeTRACK;
        else
        {
        	lnDelayCounter++;
        	if(lnDelayCounter>=50)
        	{
        		lnDelayCounter=0;
        		meCurrentState = eeSCAN;
        	}
        }
        break;
      case eeTRACK:
        //Track target
        meCurrentState = eeSCAN;
        break;
      default:
        printf("Fail... %d\n", meCurrentState);
    }
  }
}

void TargetLocator::nextAngle()
{
  //Panning has reached an edge. Change tilt angle and reverse pan direction.
  if (mnPanAngle <= MIN_PAN_ANGLE || mnPanAngle >= MAX_PAN_ANGLE)
  {
    //Tilting has reached an edge.  Change tilt direction.
    if (mnTiltAngle <= MIN_TILT_ANGLE || mnTiltAngle >= MAX_TILT_ANGLE)
    {
      mnTiltDirection *= -1;
    }
    
    mnTiltAngle += TILT_ANGLE_CHANGE * mnTiltDirection;
    if(mnTiltAngle > MAX_TILT_ANGLE)
    {
      mnTiltAngle = MAX_TILT_ANGLE;
    }
    if(mnTiltAngle < MIN_TILT_ANGLE)
    {
      mnTiltAngle = MIN_TILT_ANGLE;
    }
    
    mnPanDirection *= -1;
  }
  
  mnPanAngle += PAN_ANGLE_CHANGE * mnPanDirection;
  if(mnPanAngle > MAX_PAN_ANGLE)
  {
    mnPanAngle = MAX_PAN_ANGLE;
  }
  if(mnPanAngle < MIN_PAN_ANGLE)
  {
    mnPanAngle = MIN_PAN_ANGLE;
  }
  
  mpTurret->setPanAngle(mpData, mnPanAngle);
  mpTurret->setTiltAngle(mpData, mnTiltAngle);
}
