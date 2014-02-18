/*
 * bhs_Actuators.h
 *
 *  Created on: Dec 18, 2008
 *      Author: ITRobo
 */
#ifndef __BHS_ACTUATORS_H__
#define __BHS_ACTUATORS_H__

#include "WPILib.h"
#include "RobotGlobalData.h"
#include "MotorAcceleration.h"

class bhs_Actuators
{
	public:
		bhs_Actuators();
		~bhs_Actuators();
		
		void controlDriveTrain(RobotGlobalData *apData);
		void controlPumpAndTwitch(RobotGlobalData *apData);
		void controlCameraServos(RobotGlobalData *apData);
		void controlManipulator(RobotGlobalData *apData);
		//void controlDriveTrain(float aLeftMotor, float aRightMotor);
		
		Servo* getPanServo();
		Servo* getTiltServo();
		
		void resetAcceleration();
		
	private:
	  enum tePistonPosition {eeTwitchPistonA, eeTwitchPistonB, eeTwitchPistonC, eeTwitchPistonD, eeNumTwitchPositions};
		static const int cnFIRST_TWITCH_PISTON = 1;
	  
	  Jaguar *mpMotorAJaguar;
		Jaguar *mpMotorBJaguar;
		Jaguar *mpMotorCJaguar;
		Jaguar *mpMotorDJaguar;
		
		Solenoid *mpTwitchPistons[4];
		Relay *mpCellFeeder;
		Relay *mpCellHolder;
		
		MotorAcceleration *mpAccelerationControls[4];
		
		Servo *mpPanServo;
		Servo *mpTiltServo;
		
		Relay *mpPneumaticPump;
		//Relay *mpConveyor1;
		//Relay *mpConveyor2;
		
		//Victor *mpTurret;
		Victor *mpCellLoader1;
		Victor *mpCellLoader2;
};

#endif
