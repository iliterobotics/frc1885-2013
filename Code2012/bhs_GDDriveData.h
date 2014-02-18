#ifndef BHS_GD_DRIVE_DATA_H
#define BHS_GD_DRIVE_DATA_H

#include "bhs_Constants.h"

class bhs_GDDriveData {
public:
	bhs_GDDriveData();

	bool mdd_buttonUseArcadeDrive;
	float mdd_joystick1X;
	float mdd_joystick1Y;
	float mdd_joystick2X;
	float mdd_joystick2Y;
#ifdef NEED_TANK_DRIVE	
	float mdd_joy2X;
	float mdd_joy2Y;
#endif	

	float mdd_gyroAngle;
	float mdd_gyroBiasVoltage;
	int mdd_encoderCounts;
	double mdd_encoderDistance;

	float mdd_driveLeftPower;
	float mdd_driveRightPower;
	
	bool mdd_startDriveAngle;
	bool mdd_stopDriveAngle;
	float mdd_desiredAngle;

	float mdd_driveAnglePIDAddition;
	double mdd_driveAngleTimer;

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
	float m_minSpeedKinetic;
	float m_minSpeedStatic;
	
	bool m_incMinKinetic;
	bool m_decMinKinetic;
	bool m_incMinStatic;
	bool m_decMinStatic;
#endif
	
};

#endif
