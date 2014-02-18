#include "bhs_Diagnostics.h"

bhs_Diagnostics::bhs_Diagnostics(bhs_GlobalData* a_globalData)
	: m_globalData(a_globalData)
{
}

bhs_Diagnostics::~bhs_Diagnostics() {
	m_globalData = 0;
}

void bhs_Diagnostics::run() {
	m_globalData->mtd_diagnosticData.m_joystickX = m_globalData->mdd_joystick1X;
	m_globalData->mtd_diagnosticData.m_joystickY = m_globalData->mdd_joystick1Y;
	
	m_globalData->mtd_diagnosticData.m_gyroAngle = m_globalData->mdd_gyroAngle;
	m_globalData->mtd_diagnosticData.m_encoderDistance = m_globalData->mdd_encoderDistance;
	
	m_globalData->mtd_diagnosticData.m_driveLeftPower = m_globalData->mdd_driveLeftPower;
	m_globalData->mtd_diagnosticData.m_driveRightPower = m_globalData->mdd_driveRightPower;
	m_globalData->mtd_diagnosticData.m_shooterPower = m_globalData->msd_shooterPower;
	
	m_globalData->getHoopData(0, m_topHoopData);
	m_globalData->mtd_diagnosticData.m_topHoopDetected = m_topHoopData.filled;
	m_globalData->mtd_diagnosticData.m_topHoopTargetImageX = m_topHoopData.centerX;
	m_globalData->mtd_diagnosticData.m_topHoopTargetImageY = m_topHoopData.centerY;
	m_globalData->mtd_diagnosticData.m_topHoopBoxWidth = m_topHoopData.rectWidth;
	m_globalData->mtd_diagnosticData.m_topHoopBoxHeight = m_topHoopData.rectHeight;

	m_globalData->getHoopData(2, m_leftHoopData);
	m_globalData->mtd_diagnosticData.m_leftHoopDetected = m_leftHoopData.filled;
	m_globalData->mtd_diagnosticData.m_leftHoopTargetImageX = m_leftHoopData.centerX;
	m_globalData->mtd_diagnosticData.m_leftHoopTargetImageY = m_leftHoopData.centerY;
	m_globalData->mtd_diagnosticData.m_leftHoopBoxWidth = m_leftHoopData.rectWidth;
	m_globalData->mtd_diagnosticData.m_leftHoopBoxHeight = m_leftHoopData.rectHeight;

	m_globalData->getHoopData(1, m_rightHoopData);
	m_globalData->mtd_diagnosticData.m_rightHoopDetected = m_rightHoopData.filled;
	m_globalData->mtd_diagnosticData.m_rightHoopTargetImageX = m_rightHoopData.centerX;
	m_globalData->mtd_diagnosticData.m_rightHoopTargetImageY = m_rightHoopData.centerY;
	m_globalData->mtd_diagnosticData.m_rightHoopBoxWidth = m_rightHoopData.rectWidth;
	m_globalData->mtd_diagnosticData.m_rightHoopBoxHeight = m_rightHoopData.rectHeight;

	m_globalData->getHoopData(3, m_bottomHoopData);
	m_globalData->mtd_diagnosticData.m_bottomHoopDetected = m_bottomHoopData.filled;
	m_globalData->mtd_diagnosticData.m_bottomHoopTargetImageX = m_bottomHoopData.centerX;
	m_globalData->mtd_diagnosticData.m_bottomHoopTargetImageY = m_bottomHoopData.centerY;
	m_globalData->mtd_diagnosticData.m_bottomHoopBoxWidth = m_bottomHoopData.rectWidth;
	m_globalData->mtd_diagnosticData.m_bottomHoopBoxHeight = m_bottomHoopData.rectHeight;
	
	double l_robotX;
	double l_robotZ;
	double l_robotTheta;
	double l_dummy;
	m_globalData->getCameraData(l_robotX, l_robotZ, l_robotTheta, l_dummy);

	m_globalData->mtd_diagnosticData.m_computedRobotX = l_robotX;
	m_globalData->mtd_diagnosticData.m_computedRobotZ = l_robotZ;
	m_globalData->mtd_diagnosticData.m_computedRobotOrientation = l_robotTheta;
	
	m_globalData->mtd_diagnosticData.m_actualShooterEncRate = m_globalData->msd_encoderRate;
	m_globalData->mtd_diagnosticData.m_desiredShooterEncRate = m_globalData->msd_desiredRate;
	m_globalData->mtd_diagnosticData.m_shooterAngle = bhs_Constants::SHOOTER_ANGLE;
	m_globalData->mtd_diagnosticData.m_computedBallVelocity = 0;
	
	m_globalData->mtd_diagnosticData.m_gyroBiasVoltage = m_globalData->mdd_gyroBiasVoltage;
	m_globalData->mtd_diagnosticData.m_deadzoneValue = bhs_Constants::JOYSTICK_DEAD_ZONE;
	m_globalData->mtd_diagnosticData.m_verticalGyroAngle = m_globalData->mbd_bridgeGyroAngle;
	
	m_globalData->mtd_diagnosticData.m_pidAddition = m_globalData->mdd_driveAnglePIDAddition;
	
	m_globalData->mtd_diagnosticData.m_shooterSpeedFromCamera = 0;
	m_globalData->mtd_diagnosticData.m_shooterSpeedFromUser = m_globalData->msd_shooterTarget;
	m_globalData->mtd_diagnosticData.m_shooterSpeedOverride = m_globalData->msd_switchSpeedOverride;
	
	m_globalData->mtd_diagnosticData.m_limitSwitchBackDown = m_globalData->mbd_backDownLimitSwitch;
	m_globalData->mtd_diagnosticData.m_limitSwitchBackUp = m_globalData->mbd_backUpLimitSwitch;
	m_globalData->mtd_diagnosticData.m_limitSwitchFrontDown = m_globalData->mbd_frontDownLimitSwitch;
	m_globalData->mtd_diagnosticData.m_limitSwitchFrontUp = m_globalData->mbd_frontUpLimitSwitch;
	
	m_globalData->mtd_diagnosticData.m_avgShooterEncRate = m_globalData->msd_flavAvgEncRate;
	
	m_globalData->mtd_diagnosticData.m_distToTarget = m_globalData->msd_distanceToHoopFromCamera;
	
	m_globalData->mtd_diagnosticData.m_cameraIterations = m_globalData->m_cameraProcessingIteration; // TODO
}
