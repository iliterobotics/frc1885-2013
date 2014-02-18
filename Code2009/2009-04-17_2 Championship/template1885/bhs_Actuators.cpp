/*
 * bhs_Actuators.cpp
 *
 *  Created on: Dec 18, 2008
 *      Author: ITRobo
 */

#include "bhs_Actuators.h"
#include "RobotGlobalData.h"
#include "bhs_Constants.h"
#include "Servo.h"
#include "Relay.h"

/*
 * Default constructor
 */
bhs_Actuators::bhs_Actuators()
{
	printf("Begin bhs_Actuators constructor\n");
	mpMotorAJaguar = new Jaguar(bhs_Constants::cnDRIVE_TRAIN_MOTOR_A_CHANNEL);
	mpMotorBJaguar = new Jaguar(bhs_Constants::cnDRIVE_TRAIN_MOTOR_B_CHANNEL);
	mpMotorCJaguar = new Jaguar(bhs_Constants::cnDRIVE_TRAIN_MOTOR_C_CHANNEL);
	mpMotorDJaguar = new Jaguar(bhs_Constants::cnDRIVE_TRAIN_MOTOR_D_CHANNEL);
	
	for(int i=0; i<4; i++)
  {
    mpTwitchPistons[i] = new Solenoid(i+cnFIRST_TWITCH_PISTON);
  }
	
  mpPanServo = new Servo(bhs_Constants::cnPAN_SERVO_CHANNEL);
  mpTiltServo = new Servo(bhs_Constants::cnTILT_SERVO_CHANNEL);
  
  mpPneumaticPump = new Relay(bhs_Constants::cnCOMPRESSOR_RELAY_CHANNEL);
  /*mpConveyor1 = new Relay(bhs_Constants::cnCONVEYOR_1_RELAY_CHANNEL);
  mpConveyor2 = new Relay(bhs_Constants::cnCONVEYOR_2_RELAY_CHANNEL);*/
  mpCellFeeder = new Relay(bhs_Constants::cnCELL_FEEDER_RELAY_CHANNEL);
  mpCellHolder = new Relay(bhs_Constants::cnCELL_HOLDER_RELAY_CHANNEL);
  
  //mpTurret = new Victor(bhs_Constants::cnTURRET_MOTOR_CHANNEL);
  mpCellLoader1 = new Victor(bhs_Constants::cnEMPTY_CELL_LOADER_MOTOR_1_CHANNEL);
  mpCellLoader2 = new Victor(bhs_Constants::cnEMPTY_CELL_LOADER_MOTOR_2_CHANNEL);
  
  printf("Begin MotorAcceleration constructors\n");
  for (int i=0; i<4; i++)
  {
    mpAccelerationControls[i] = new MotorAcceleration();
  }
  printf("Completed MotorAcceleration constructor\n");
  
  printf("Completed bhs_Actuators constructor\n");
}

/*
 * Default destructor
 */
bhs_Actuators::~bhs_Actuators()
{
	delete mpMotorAJaguar;
	delete mpMotorBJaguar;
	delete mpMotorCJaguar;
	delete mpMotorDJaguar;
	for(int i=0; i<4; i++)
  {
    delete mpTwitchPistons[i];
  }
	for(int i=0; i<4; i++)
	{
	  delete mpAccelerationControls[i];
	}
	delete mpPanServo;
	delete mpTiltServo;
	delete mpPneumaticPump;
	//delete mpConveyor1;
	//delete mpConveyor2;
	delete mpCellFeeder;  
	//delete mpTurret;
	delete mpCellLoader1;
	delete mpCellLoader2;
}

/*void bhs_Actuators::controlDriveTrain(bhs_DriveData aDriveData)
{
	controlDriveTrain(aDriveData.mLeftMotor, aDriveData.mRightMotor);
}*/

void bhs_Actuators::controlDriveTrain(RobotGlobalData *apData)
{
  //Enforce an acceleration curve on each motor
  /*apData->mnMotorAPower
      = mpAccelerationControls[0]->accelerateMotor(apData->mnMotorAPower);
  apData->mnMotorBPower
      = mpAccelerationControls[1]->accelerateMotor(apData->mnMotorBPower);
  apData->mnMotorCPower
      = mpAccelerationControls[2]->accelerateMotor(apData->mnMotorCPower);
  apData->mnMotorDPower
      = mpAccelerationControls[3]->accelerateMotor(apData->mnMotorDPower);*/
  
	mpMotorAJaguar->Set(apData->mnMotorAPower);
	mpMotorBJaguar->Set(apData->mnMotorBPower);
	mpMotorCJaguar->Set(apData->mnMotorCPower);
	mpMotorDJaguar->Set(apData->mnMotorDPower);
  
  //printf("Motor power. %f, %f, %f, %f\n", apData->mnLeftMotorFront, apData->mnLeftMotorBack, apData->mnRightMotorFront, apData->mnRightMotorBack);
}

void bhs_Actuators::controlManipulator(RobotGlobalData *apData)
{
  //mpTurret->Set(apData->mnTurretPower);
  mpCellLoader1->Set(apData->mnCellLoaderPower * bhs_Constants::cnCELL_LOADER_MOTOR_1_DIRECTION);
  mpCellLoader2->Set(apData->mnCellLoaderPower * bhs_Constants::cnCELL_LOADER_MOTOR_2_DIRECTION);
  
  
  /*if(apData->meConveyorState == bhs_Constants::cnCONVEYOR_FORWARD_VALUE)
  {
    mpConveyor1->Set(bhs_Constants::cnCONVEYOR_FORWARD_VALUE);
    mpConveyor2->Set(bhs_Constants::cnCONVEYOR_2_FORWARD_VALUE);
  }
  else if(apData->meConveyorState == bhs_Constants::cnCONVEYOR_REVERSE_VALUE)
  {
    mpConveyor1->Set(bhs_Constants::cnCONVEYOR_REVERSE_VALUE);
    mpConveyor2->Set(bhs_Constants::cnCONVEYOR_2_REVERSE_VALUE);
  }
  else
  {
    mpConveyor1->Set(bhs_Constants::cnCONVEYOR_OFF_VALUE);
    mpConveyor2->Set(bhs_Constants::cnCONVEYOR_2_OFF_VALUE);
  }*/
  
  
  mpCellFeeder->Set(apData->meCellFeederState);
  mpCellHolder->Set(apData->meCellHolderState);
}

void bhs_Actuators::controlPumpAndTwitch(RobotGlobalData *apData)
{
  if(apData->mbRunCompressor && apData->mnAirPressure == 0)
  {
    mpPneumaticPump->Set(Relay::kForward);
  }
  else
  {
    mpPneumaticPump->Set(Relay::kOff);
  }
  
  //set the piston controlling the motor positions
  mpTwitchPistons[eeTwitchPistonA]->Set(apData->mbPhysicalMotorAFacing);
  mpTwitchPistons[eeTwitchPistonB]->Set(apData->mbPhysicalMotorBFacing);
  mpTwitchPistons[eeTwitchPistonC]->Set(apData->mbPhysicalMotorCFacing);
  mpTwitchPistons[eeTwitchPistonD]->Set(apData->mbPhysicalMotorDFacing);
}

void bhs_Actuators::controlCameraServos(RobotGlobalData *apData)
{
  bhs_CameraTurretData lcCameraTurretData;
  
  apData->getCameraTurretData(lcCameraTurretData);
  
  mpPanServo->SetAngle(lcCameraTurretData.mnCameraTurretPanDegree);
  mpTiltServo->SetAngle(lcCameraTurretData.mnCameraTurretTiltDegree);
}

Servo* bhs_Actuators::getPanServo()
{
	return mpPanServo;
}

Servo* bhs_Actuators::getTiltServo()
{
	return mpTiltServo;
}

void bhs_Actuators::resetAcceleration()
{
  for (int i=0; i<4; i++)
  {
    mpAccelerationControls[i]->reset();
  }
}
