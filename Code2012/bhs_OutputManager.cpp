#include "bhs_OutputManager.h"

const UINT8 bhs_OutputManager::CJAG_RIGHT_DRIVE_BACK_NUMBER = 2;
const UINT8 bhs_OutputManager::CJAG_RIGHT_DRIVE_FRONT_NUMBER = 3;
const UINT8 bhs_OutputManager::CJAG_LEFT_DRIVE_BACK_NUMBER = 4;
const UINT8 bhs_OutputManager::CJAG_LEFT_DRIVE_FRONT_NUMBER = 5;
const UINT8 bhs_OutputManager::CJAG_CONVEYOR_LEFT_NUMBER = 6;
const UINT8 bhs_OutputManager::CJAG_SHOOTER_TOP_NUMBER = 7;
const UINT8 bhs_OutputManager::CJAG_CONVEYOR_RIGHT_NUMBER = 8;
const UINT8 bhs_OutputManager::CJAG_SHOOTER_BOTTOM_NUMBER = 9;
const UINT8 bhs_OutputManager::CJAG_CONVEYOR_BACK_NUMBER = 10;

const UINT32 bhs_OutputManager::RELAY_CONVEYOR_VERT_CHANNEL = 1;
const UINT32 bhs_OutputManager::RELAY_BRIDGE_LOWERING_FRONT_CHANNEL = 3;

bhs_OutputManager::bhs_OutputManager(bhs_GlobalData* a_globalData)
	: m_globalData(a_globalData)

#if USE_DRIVE_TRAIN_MOTORS
	, m_driveRightB(CJAG_RIGHT_DRIVE_BACK_NUMBER)
	, m_driveLeftB(CJAG_LEFT_DRIVE_BACK_NUMBER)
#if USE_FOUR_DRIVE_TRAIN_MOTORS
	, m_driveRightF(CJAG_RIGHT_DRIVE_FRONT_NUMBER)
	, m_driveLeftF(CJAG_LEFT_DRIVE_FRONT_NUMBER)
#endif
#endif

#if USE_CONVEYOR_MOTORS
	, m_conveyorLeft(CJAG_CONVEYOR_LEFT_NUMBER)
	, m_conveyorRight(CJAG_CONVEYOR_RIGHT_NUMBER)
	, m_conveyorVert(RELAY_CONVEYOR_VERT_CHANNEL)
	, m_conveyorBack(CJAG_CONVEYOR_BACK_NUMBER)
#endif

#if USE_SHOOTER_MOTORS
	, m_shooterTop(CJAG_SHOOTER_TOP_NUMBER)
#if USE_TWO_SHOOTER_MOTORS
	, m_shooterBottom(CJAG_SHOOTER_BOTTOM_NUMBER)
#endif
#endif

#if USE_BRIDGE_LOWERING_MOTORS
	, m_bridgeLoweringFront(RELAY_BRIDGE_LOWERING_FRONT_CHANNEL, Relay::kBothDirections)
#endif
{
	m_reversedMotors = new bool[k_numMotors];

#if USE_DRIVE_TRAIN_MOTORS
	m_reversedMotors[k_driveRightB] = false;
	m_reversedMotors[k_driveLeftB] = true;
#if USE_FOUR_DRIVE_TRAIN_MOTORS
	m_reversedMotors[k_driveRightF] = false;
	m_reversedMotors[k_driveLeftF] = true;
#endif
#endif
	
#if USE_CONVEYOR_MOTORS
	m_reversedMotors[k_conveyorLeft] = false;
	m_reversedMotors[k_conveyorRight] = false;
	m_reversedMotors[k_conveyorVert] = true;
	m_reversedMotors[k_conveyorBack] = true;
#endif
	
#if USE_SHOOTER_MOTORS
	m_reversedMotors[k_shooterTop] = false;
#if USE_TWO_SHOOTER_MOTORS
	m_reversedMotors[k_shooterBottom] = true;
#endif
#endif

#if USE_BRIDGE_LOWERING_MOTORS
	m_reversedMotors[k_bridgeLoweringFront] = false;
#endif
}

bhs_OutputManager::~bhs_OutputManager() {
	m_globalData = 0;
	delete[] m_reversedMotors;
}

void bhs_OutputManager::run() {
	safety();

#if USE_DRIVE_TRAIN_MOTORS || USE_CONVEYOR_MOTORS || USE_SHOOTER_MOTORS
	runJaguars();
#endif
#if USE_CONVEYOR_MOTORS || USE_BRIDGE_LOWERING_MOTORS
	runRelays();
#endif
}

void bhs_OutputManager::safety() {
	
}

#if USE_DRIVE_TRAIN_MOTORS || USE_CONVEYOR_MOTORS || USE_SHOOTER_MOTORS
void bhs_OutputManager::runJaguars() {
#if USE_DRIVE_TRAIN_MOTORS
	m_driveRightB.Set(processReversedJagMotor(k_driveRightB, m_globalData->mdd_driveRightPower));
	m_driveLeftB.Set(processReversedJagMotor(k_driveLeftB, m_globalData->mdd_driveLeftPower));
#if USE_FOUR_DRIVE_TRAIN_MOTORS
	m_driveRightF.Set(processReversedJagMotor(k_driveRightF, m_globalData->mdd_driveRightPower));
	m_driveLeftF.Set(processReversedJagMotor(k_driveLeftF, m_globalData->mdd_driveLeftPower));
#endif
#endif

#if USE_CONVEYOR_MOTORS
	m_conveyorLeft.Set(processReversedJagMotor(k_conveyorLeft, m_globalData->mcd_leftConveyorPower));
	m_conveyorRight.Set(processReversedJagMotor(k_conveyorRight, m_globalData->mcd_rightConveyorPower));
	m_conveyorBack.Set(processReversedJagMotor(k_conveyorBack, m_globalData->mcd_backConveyorPower));
#endif

#if USE_SHOOTER_MOTORS
	m_shooterTop.Set(processReversedJagMotor(k_shooterTop, m_globalData->msd_shooterPower));
#if USE_TWO_SHOOTER_MOTORS
	m_shooterBottom.Set(processReversedJagMotor(k_shooterBottom, m_globalData->msd_shooterPower));
#endif
#endif
}
#endif

#if USE_CONVEYOR_MOTORS || USE_BRIDGE_LOWERING_MOTORS
void bhs_OutputManager::runRelays() {
#if USE_CONVEYOR_MOTORS
	m_conveyorVert.Set(processReversedRelayMotor(k_conveyorVert, m_globalData->mcd_vertConveyorValue));
#endif

#if USE_BRIDGE_LOWERING_MOTORS
	m_bridgeLoweringFront.Set(processReversedRelayMotor(k_bridgeLoweringFront, m_globalData->mbd_bridgeFrontArmValue));
#endif
}
#endif

#if USE_DRIVE_TRAIN_MOTORS || USE_CONVEYOR_MOTORS || USE_SHOOTER_MOTORS
float bhs_OutputManager::processReversedJagMotor(MotorID a_motor, float a_motorPower) {
	if (m_reversedMotors[a_motor]) {
		return -a_motorPower;
	} else {
		return a_motorPower;
	}
}
#endif

#if USE_CONVEYOR_MOTORS || USE_BRIDGE_LOWERING_MOTORS
Relay::Value bhs_OutputManager::processReversedRelayMotor(MotorID a_motor, Relay::Value a_relayValue) {
	if (a_relayValue == Relay::kForward) {
		return (m_reversedMotors[a_motor] ? Relay::kReverse : Relay::kForward);  
	} else if (a_relayValue == Relay::kReverse) {
		return (m_reversedMotors[a_motor] ? Relay::kForward : Relay::kReverse);
	} else {
		return a_relayValue;
	}
}
#endif
