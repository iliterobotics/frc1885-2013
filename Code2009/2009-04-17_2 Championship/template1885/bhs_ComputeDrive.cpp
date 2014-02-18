#include "bhs_ComputeDrive.h"
#include "bhs_Constants.h"
#include <math.h>

/**Method Name: 
 *
 * Description: This method computes the 4 motor powers, applies direction to the  
 *	       power values, and  saves the computed data to the robot global data
 *             class.
 * 
 * Parameters:
 *	apData: Pointer to where this method will save its results.
 *	aXaxis: Holds the x axis value of the joystick that is passed in.
 *	aYaxis: Holds the y axis value of the joystick that is passed in.
 *	anLeftMotorFrontDirection: Provides the directional value for the left front motor.	
 *                                 The values should be either 1 or -1.
 *	anLeftMotorBackDirection: Provides the directional value for the left rear motor.
 *                                The values should be  either 1 or -1.
 *	anRightMotorFrontDirection: Provides the directional value for the right front motor.
 * 								The values should be either 1 or -1.
 *	anRightMotorBackDirection: Provides the directional value for the right rear motor.
 * 							   The values should be either 1 or -1.
 */

bhs_ComputeDrive::bhs_ComputeDrive()
{
  printf("Begin ComputeDrive constructor\n");
  maAutomaticModeMotorConfiguration[eeNORTH][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingForward;
  
  maAutomaticModeMotorConfiguration[eeNORTH_EAST][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH_EAST][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH_EAST][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH_EAST][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingSideways;
  
  maAutomaticModeMotorConfiguration[eeEAST][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeEAST][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingSideways;
  maAutomaticModeMotorConfiguration[eeEAST][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeEAST][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingSideways;
  
  maAutomaticModeMotorConfiguration[eeSOUTH_EAST][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH_EAST][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingSideways;
  maAutomaticModeMotorConfiguration[eeSOUTH_EAST][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH_EAST][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingForward;
  
  maAutomaticModeMotorConfiguration[eeSOUTH][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingForward;
  
  maAutomaticModeMotorConfiguration[eeSOUTH_WEST][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH_WEST][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeSOUTH_WEST][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingSideways;
  maAutomaticModeMotorConfiguration[eeSOUTH_WEST][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingForward;
  
  maAutomaticModeMotorConfiguration[eeWEST][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingSideways;
  maAutomaticModeMotorConfiguration[eeWEST][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeWEST][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingSideways;
  maAutomaticModeMotorConfiguration[eeWEST][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingForward;
  
  maAutomaticModeMotorConfiguration[eeNORTH_WEST][eeMOTOR_A]
      = RobotGlobalData::gbMotorFacingSideways;
  maAutomaticModeMotorConfiguration[eeNORTH_WEST][eeMOTOR_B]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH_WEST][eeMOTOR_C]
      = RobotGlobalData::gbMotorFacingForward;
  maAutomaticModeMotorConfiguration[eeNORTH_WEST][eeMOTOR_D]
      = RobotGlobalData::gbMotorFacingForward;
  
  printf("Completed ComputeDrive constructor\n");
}

void bhs_ComputeDrive::VirtualDriveMath(RobotGlobalData* apData, float anUserXaxis,
                                        float anUserYaxis)
{
  if(apData->meDriveStatus == RobotGlobalData::kVectorForwardHybrid)
  {
    SkidSteer(apData, anUserXaxis, anUserYaxis);
    apData->mnLeftMotorFront = anUserXaxis;
    apData->mnRightMotorFront = anUserXaxis;
  } 
  else if(apData->meDriveStatus == RobotGlobalData::kVectorBackwardHybrid)
  { //Added by Buddy. No idea what I was supposed to do here. xD
  //Check and approved by Wei (w/ changes)
    SkidSteer(apData, anUserXaxis, anUserYaxis);
    apData->mnLeftMotorBack = anUserXaxis;
    apData->mnRightMotorBack = anUserXaxis;
  }
  else if(apData->meDriveStatus == RobotGlobalData::kVectorLeftHybrid)
  {
    SkidSteer(apData, anUserXaxis, -1*anUserYaxis);
    apData->mnRightMotorBack = apData->mnLeftMotorBack;
    apData->mnLeftMotorFront = anUserXaxis;
    apData->mnLeftMotorBack = anUserXaxis;
  }
  else if(apData->meDriveStatus == RobotGlobalData::kVectorRightHybrid)
  {
    SkidSteer(apData, anUserXaxis, anUserYaxis);
    apData->mnLeftMotorBack = apData->mnRightMotorBack;
    apData->mnRightMotorFront = anUserXaxis;
    apData->mnRightMotorBack = anUserXaxis;
  }
  else if (apData->meDriveStatus != RobotGlobalData::kVector)
  {
    SkidSteer(apData, anUserXaxis, anUserYaxis);
  }
  else
  {
    VectorDrive(apData, anUserXaxis, anUserYaxis);
  }
}

void bhs_ComputeDrive::VectorDrive(RobotGlobalData* apData, float anUserXaxis,
                                   float anUserYaxis)
{
  apData->mnLeftMotorFront = anUserYaxis;
  apData->mnLeftMotorBack = anUserXaxis;
  apData->mnRightMotorFront = anUserXaxis;
  apData->mnRightMotorBack = anUserYaxis;
}
/**Computes the drive train motor powers.
 * The following if statement converts the yaxis value to a positive number
 * to ensure that the values add and subtract correctly. After it computes
 * it will convert the values back to the original negative values.
 */
void bhs_ComputeDrive::SkidSteer(RobotGlobalData *apData, float anUserXaxis,
                                 float anUserYaxis)
{
  
  float lnLeftMotorPower = anUserYaxis + anUserXaxis;
  float lnRightMotorPower = anUserYaxis - anUserXaxis;
  
  apData->mnLeftMotorFront = lnLeftMotorPower;
  apData->mnLeftMotorBack = lnLeftMotorPower;
  apData->mnRightMotorFront = lnRightMotorPower;
  apData->mnRightMotorBack = lnRightMotorPower;
  
}
void bhs_ComputeDrive::MapMotorDirections(RobotGlobalData* apData)
{
  switch (apData->meDriveStatus)
  {
    case RobotGlobalData::kAutomatic:
      //call kickdrive to determine wheel orientation base on
      //where joystick is at
      automaticMotorConfiguration(apData);
      break;
    case RobotGlobalData::kMotorsForward:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingForward;
      break;
    case RobotGlobalData::kMotorsSideways:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingSideways;
      break;
    case RobotGlobalData::kVector:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingForward;
      break;
    case RobotGlobalData::kVectorForwardHybrid:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingForward;
      break;
        //Added by Buddy. Backward wheels going left-right, front wheels going up-down
        //Checked and approved by Wei
    case RobotGlobalData::kVectorBackwardHybrid:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingSideways;
      break;
    case RobotGlobalData::kVectorLeftHybrid:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingSideways;
      break;
    case RobotGlobalData::kVectorRightHybrid:
      apData->mbPhysicalMotorAFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorBFacing = RobotGlobalData::gbMotorFacingSideways;
      apData->mbPhysicalMotorCFacing = RobotGlobalData::gbMotorFacingForward;
      apData->mbPhysicalMotorDFacing = RobotGlobalData::gbMotorFacingForward;
      break;
  }
}

void bhs_ComputeDrive::automaticMotorConfiguration(RobotGlobalData* apData)
{
  teDirections leRobotDirection;
  double lnUserAngle = apData->mnUserAngle;
  if (sqrt(pow(apData->mnRawXaxis, 2) + pow(-1*apData->mnRawYaxis, 2))
      > bhs_Constants::TWITCH_DEAD_ZONE)
  {
    if (((lnUserAngle>=0)&&(lnUserAngle<=7.5))||((lnUserAngle>352.5)&&(lnUserAngle
        <=360)))
    {
      //printf("East %f\n", lnUserAngle);
      leRobotDirection = eeEAST;
    }
    else if ((lnUserAngle>7.5)&&(lnUserAngle<=45))
    {
      //printf("North East %f\n", lnUserAngle);
      leRobotDirection = eeNORTH_EAST;
    }
    else if ((lnUserAngle>45)&&(lnUserAngle<=135))
    {
      //printf("North %f\n", lnUserAngle);
      leRobotDirection = eeNORTH;
    }
    else if ((lnUserAngle>135)&&(lnUserAngle<=172.5))
    {
      //printf("North West %f\n", lnUserAngle);
      leRobotDirection = eeNORTH_WEST;
    }
    else if ((lnUserAngle>172.5)&&(lnUserAngle<=187.5))
    {
      //printf("West %f\n", lnUserAngle);
      leRobotDirection = eeWEST;
    }
    else if ((lnUserAngle>187.5)&&(lnUserAngle<=225))
    {
      //printf("South West %f\n", lnUserAngle);
      leRobotDirection = eeSOUTH_WEST;
    }
    else if ((lnUserAngle>225)&&(lnUserAngle<=315))
    {
      //printf("South %f\n", lnUserAngle);
      leRobotDirection = eeSOUTH;
    }
    else if ((lnUserAngle>315)&&(lnUserAngle<=352.5))
    {
      //printf("South East %f\n", lnUserAngle);
      leRobotDirection = eeSOUTH_EAST;
    }
    else
    {
      printf("ERROR: INVALID TWITCH ZONE");
      leRobotDirection = eeNORTH;
    }
  }
  else
  {
    //printf("Neutral\n");
    leRobotDirection = eeNORTH;
  }
  
  apData->mbPhysicalMotorAFacing
      = maAutomaticModeMotorConfiguration[leRobotDirection][eeMOTOR_A];
  apData->mbPhysicalMotorBFacing
      = maAutomaticModeMotorConfiguration[leRobotDirection][eeMOTOR_B];
  apData->mbPhysicalMotorCFacing
      = maAutomaticModeMotorConfiguration[leRobotDirection][eeMOTOR_C];
  apData->mbPhysicalMotorDFacing
      = maAutomaticModeMotorConfiguration[leRobotDirection][eeMOTOR_D];
}
