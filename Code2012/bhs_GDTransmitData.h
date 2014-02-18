#ifndef BHS_GD_TRANSMIT_DATA_H
#define BHS_GD_TRANSMIT_DATA_H

#include "string.h"

class bhs_GDTransmitData {
public:
	bhs_GDTransmitData();

	typedef struct {
		float m_joystickX;
		float m_joystickY;
		
		float m_gyroAngle;
		float m_encoderDistance;
		
		float m_driveLeftPower;
		float m_driveRightPower;
		float m_shooterPower;
		
		int m_topHoopDetected; // boolean
		int m_topHoopTargetImageX;
		int m_topHoopTargetImageY;
		
		int m_leftHoopDetected; // boolean
		int m_leftHoopTargetImageX;
		int m_leftHoopTargetImageY;
		
		int m_rightHoopDetected; // boolean
		int m_rightHoopTargetImageX;
		int m_rightHoopTargetImageY;
		
		int m_bottomHoopDetected; // boolean
		int m_bottomHoopTargetImageX;
		int m_bottomHoopTargetImageY;
		
		float m_computedRobotX;
		float m_computedRobotZ;
		
		float m_actualShooterEncRate;
		float m_desiredShooterEncRate;
		float m_shooterAngle;
		float m_computedBallVelocity;
		
		float m_gyroBiasVoltage;
		float m_deadzoneValue;
		float m_verticalGyroAngle;
		
		float m_pidAddition;
		
		float m_shooterSpeedFromCamera;
		float m_shooterSpeedFromUser;
		int m_shooterSpeedOverride; // boolean
		
		int m_limitSwitchBackDown; // boolean
		int m_limitSwitchBackUp; // boolean
		int m_limitSwitchFrontDown; // boolean
		int m_limitSwitchFrontUp; // boolean
		
		// 36 variables * 4 = 144 bytes
		
		// diagnostic variables that have not been added to the IDD sheet yet
		
		int m_topHoopBoxWidth;
		int m_topHoopBoxHeight;
		
		int m_leftHoopBoxWidth;
		int m_leftHoopBoxHeight;
		
		int m_rightHoopBoxWidth;
		int m_rightHoopBoxHeight;
		
		int m_bottomHoopBoxWidth;
		int m_bottomHoopBoxHeight;
		
		float m_computedRobotOrientation;
		int m_computedFrameNumber;
		
		float m_lockedInRobotX;
		float m_lockedInRobotZ;
		float m_lockedInRobotOrientation;
		int m_lockedInFrameNumber;
		
		int m_iddRevNumber;
		
		float m_avgShooterEncRate;
		float m_distToTarget;
		
		int m_cameraIterations;
	} DiagnosticData;
	
	DiagnosticData mtd_diagnosticData;

};

#endif // BHS_GD_TRANSMIT_DATA_H
