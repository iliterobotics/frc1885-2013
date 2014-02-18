#include "bhs_Robot.h"

bhs_Robot::bhs_Robot()
	: m_globalData()

	, m_sensorInput(&m_globalData)
	, m_joystickInput(&m_globalData)

	, m_driveTrain(&m_globalData)
	, m_driveAngle(&m_globalData)
	, m_conveyor(&m_globalData)
	, m_shooter(&m_globalData)
	, m_bridgeLowering(&m_globalData)
	, m_nudge(&m_globalData)

	, m_outputManager(&m_globalData)
	, m_diagnostics(&m_globalData)
	, m_dataServer()
{
	Watchdog w;
	w.Kill();
	
	m_autonomousInput = new bhs_AutonomousBasicWithBridge(&m_globalData);

#if USE_HOOP_CAMERA
	m_axisCamera = bhs_AxisCamera::GetInstance();
	m_axisCamera->init(dynamic_cast<bhs_CameraData*>(&m_globalData), &m_dataServer);
#endif
	
#if USE_GROUND_CAMERA
	m_axisCamera2 = bhs_AxisCamera2::GetInstance("10.18.85.12");
	m_axisCamera2->setDataServer(&m_dataServer);
#endif
}

bhs_Robot::~bhs_Robot() {
	delete m_autonomousInput;
}

void bhs_Robot::disabledInit() {

}

void bhs_Robot::autonomousInit() {
	m_sensorInput.init();
	m_autonomousInput->init();
}

void bhs_Robot::autonomous() {
	m_sensorInput.run();
	m_autonomousInput->run();
	normalRobotProcessing();
}

void bhs_Robot::teleopInit() {
	m_sensorInput.init();
	m_joystickInput.init();
}

void bhs_Robot::teleop() {
	m_sensorInput.run();
	m_joystickInput.run();
	normalRobotProcessing();
}

void bhs_Robot::normalRobotProcessing() {
	m_driveTrain.run();
	m_driveAngle.run();
	m_conveyor.run();
	m_shooter.run();
	m_bridgeLowering.run();
	m_nudge.run();
	
	m_outputManager.run();
	
	m_diagnostics.run();
	m_dataServer.xmitData((char*) &(m_globalData.mtd_diagnosticData), (int) sizeof(bhs_GlobalData::DiagnosticData));

	//	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "PD: %f/%f", m_globalData.mdd_gyroP, m_globalData.mdd_gyroD);
	//	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "timer: %5.4f", m_globalData.mdd_driveAngleTimer);
	//	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "min(s/k): %4.3f/%4.3f", m_globalData.m_minSpeedStatic, m_globalData.m_minSpeedKinetic);

	//	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "ls: %d%d%d%d", m_globalData.mbd_backDownLimitSwitch,
	//			m_globalData.mbd_backUpLimitSwitch,
	//			m_globalData.mbd_frontDownLimitSwitch,
	//			m_globalData.mbd_frontUpLimitSwitch);

	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "min: %f", m_globalData.msd_flavMinEncRate);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "max: %f", m_globalData.msd_flavMaxEncRate);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line3, "avg: %f", m_globalData.msd_flavAvgEncRate);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line4, "pow: %f", m_globalData.msd_shooterPower);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line5, "dsr: %f", m_globalData.msd_desiredRate);
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line6, "it: %d", m_globalData.m_cameraProcessingIteration);

	//	static int it = 0;
	//	if (it%25 == 0) {
	DriverStationLCD::GetInstance()->UpdateLCD();
	//		it = 0;
	//	}
	//	it++;
}
