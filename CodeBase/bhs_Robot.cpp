#include "bhs_Robot.h"

bhs_Robot::bhs_Robot() 
	: m_gd()
	, m_outputManager(&m_gd)
{
	Watchdog w;
	w.Kill();

	m_sensorInput = new bhs_SensorInput(&m_gd);
	m_autonomousInput = new bhs_Autonomous(&m_gd);
	m_driverInput = new bhs_DriverInput(&m_gd);

	m_drivetrain = new bhs_DriveTrain(&m_gd);
	m_driveAngle = new bhs_DriveAngle(&m_gd);
	m_nudge = new bhs_Nudge(&m_gd);
	m_subsys1 = new bhs_Subsys1(&m_gd);
	m_subsys2 = new bhs_Subsys2(&m_gd);
	m_subsys3 = new bhs_Subsys3(&m_gd);
}

bhs_Robot::~bhs_Robot() {
	delete m_sensorInput;
	delete m_autonomousInput;
	delete m_driverInput;
	
	delete m_drivetrain;
	delete m_driveAngle;
	delete m_nudge;
	delete m_subsys1;
	delete m_subsys1;
	delete m_subsys3;
	
}

void bhs_Robot::disabledInit() {
}

void bhs_Robot::autonomousInit() {
	m_sensorInput->init();
	m_autonomousInput->init();
}

void bhs_Robot::autonomous() {
	m_sensorInput->run();
	m_autonomousInput->run();
	normalRobotProcessing();
}

void bhs_Robot::teleopInit() {
	m_sensorInput->init();
	m_driverInput->init();
}

void bhs_Robot::teleop() {
	m_sensorInput->run();
	m_driverInput->run();
	normalRobotProcessing();
}

void bhs_Robot::normalRobotProcessing() {
	m_drivetrain->run();
	m_driveAngle->run();
	m_nudge->run();
	m_subsys1->run();
	m_subsys2->run();
	m_subsys3->run();
	
	m_outputManager.run();
}
