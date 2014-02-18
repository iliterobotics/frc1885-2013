#ifndef BHS_GD_DRIVETRAIN_H_
#define BHS_GD_DRIVETRAIN_H_

#include "bhs_Constants.h"

class bhs_GDDrivetrain {
	public:
		bhs_GDDrivetrain();

		bool mdd_buttonUseArcadeDrive;
		float mdd_joystick1X;
		float mdd_joystick1Y;
		float mdd_joystick2X;
		float mdd_joystick2Y;

		float mdd_gyroAngle;
		float mdd_gyroBiasVoltage;
		int mdd_encoderCounts;
		double mdd_encoderDistance;

		bool mdd_impulseNudgeLeft;
		bool mdd_impulseNudgeRight;
		bool mdd_impulseNudgeForward;
		bool mdd_impulseNudgeBackward;
		int mdd_extraTenthsOfSecToNudge;

		bool mdd_startDriveAngle;
		bool mdd_stopDriveAngle;
		float mdd_desiredAngle;
		float mdd_driveAnglePIDAddition;
		double mdd_driveAngleTimer;

		float mdd_driveLeftPower;
		float mdd_driveRightPower;


#if TEST_DRIVE_ANGLE_PID
		bool mdd_buttonIncP;
		bool mdd_buttonDecP;
		bool mdd_buttonIncD;
		bool mdd_buttonDecD;
		bool mdd_resetPIDButton;

		double mdd_gyroP;
		double mdd_gyroD;
#endif
		
#if TEST_DRIVE_ANGLE_MIN_SPEEDS
		float mdd_minSpeedKinetic;
		float mdd_minSpeedStatic;
		
		bool mdd_incMinKinetic;
		bool mdd_decMinKinetic;
		bool mdd_incMinStatic;
		bool mdd_decMinStatic;
#endif
	
};

#endif //BHS_GD_DRIVETRAIN_H_
