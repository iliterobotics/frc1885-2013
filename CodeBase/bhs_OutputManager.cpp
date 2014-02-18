#include "bhs_OutputManager.h"

bhs_OutputManager::bhs_OutputManager(bhs_GlobalData* a_gd) 
	: m_driveRightB(K_DRIVE_RIGHT_B)
	, m_driveLeftB(K_DRIVE_LEFT_B)
	, m_driveRightF(K_DRIVE_RIGHT_F)
	, m_driveLeftF(K_DRIVE_LEFT_F)
{
	m_gd = a_gd;

	// Actuator declarations go here
}

bhs_OutputManager::~bhs_OutputManager() {
	delete m_gd;
}


void bhs_OutputManager::init() {
	m_driveRightB.Set(0);
	m_driveLeftB.Set(0);
	m_driveRightF.Set(0);
	m_driveLeftF.Set(0);

	// Set all actuators to initial position
}

void bhs_OutputManager::run() {
	safety();
	runMotors();
	runPneumatics();
}


void bhs_OutputManager::safety() {
	// Make sure all output values are within bounds
}

void bhs_OutputManager::runMotors() {
	// Run all motors
}

void bhs_OutputManager::runPneumatics() {
	// Actuate all pneumatics
}
