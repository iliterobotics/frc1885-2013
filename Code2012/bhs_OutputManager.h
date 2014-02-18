#ifndef BHS_OUTPUT_MANAGER_H
#define BHS_OUTPUT_MANAGER_H

#include "bhs_Constants.h"

#if USE_CONVEYOR_MOTORS || USE_BRIDGE_LOWERING_MOTORS
#include "Relay.h"
#endif

#include "bhs_GlobalData.h"
#if USE_DRIVE_TRAIN_MOTORS || USE_CONVEYOR_MOTORS || USE_SHOOTER_MOTORS
#include "bhs_CANJaguar.h"
#endif

class bhs_OutputManager {
public:
	static const UINT8 CJAG_RIGHT_DRIVE_BACK_NUMBER;
	static const UINT8 CJAG_RIGHT_DRIVE_FRONT_NUMBER;
	static const UINT8 CJAG_LEFT_DRIVE_BACK_NUMBER;
	static const UINT8 CJAG_LEFT_DRIVE_FRONT_NUMBER;
	static const UINT8 CJAG_CONVEYOR_LEFT_NUMBER;
	static const UINT8 CJAG_SHOOTER_TOP_NUMBER;
	static const UINT8 CJAG_CONVEYOR_RIGHT_NUMBER;
	static const UINT8 CJAG_SHOOTER_BOTTOM_NUMBER;
	static const UINT8 CJAG_CONVEYOR_BACK_NUMBER;
	static const UINT32 RELAY_CONVEYOR_VERT_CHANNEL;
	static const UINT32 RELAY_BRIDGE_LOWERING_FRONT_CHANNEL;
	
	typedef enum {
		k_null = 0,

#if USE_DRIVE_TRAIN_MOTORS
		k_driveRightB,
		k_driveLeftB,
#if USE_FOUR_DRIVE_TRAIN_MOTORS
		k_driveRightF,
		k_driveLeftF,
#endif
#endif

#if USE_CONVEYOR_MOTORS
		k_conveyorLeft,
		k_conveyorRight,
		k_conveyorVert,
		k_conveyorBack,
#endif

#if USE_SHOOTER_MOTORS
		k_shooterTop,
#if USE_TWO_SHOOTER_MOTORS
		k_shooterBottom,
#endif
#endif

#if USE_BRIDGE_LOWERING_MOTORS
		k_bridgeLoweringFront,
#endif

		k_numMotors
	} MotorID;

	bhs_OutputManager(bhs_GlobalData*);
	~bhs_OutputManager();

	void run();

private:
	void safety();

#if USE_DRIVE_TRAIN_MOTORS || USE_CONVEYOR_MOTORS || USE_SHOOTER_MOTORS
	void runJaguars();
#endif
#if USE_CONVEYOR_MOTORS || USE_BRIDGE_LOWERING_MOTORS
	void runRelays();
#endif

#if USE_DRIVE_TRAIN_MOTORS || USE_CONVEYOR_MOTORS || USE_SHOOTER_MOTORS
	float processReversedJagMotor(MotorID, float);
#endif
#if USE_CONVEYOR_MOTORS || USE_BRIDGE_LOWERING_MOTORS
	Relay::Value processReversedRelayMotor(MotorID, Relay::Value);
#endif

	bhs_GlobalData* m_globalData;

#if USE_DRIVE_TRAIN_MOTORS
	bhs_CANJaguar m_driveRightB;
	bhs_CANJaguar m_driveLeftB;
#if USE_FOUR_DRIVE_TRAIN_MOTORS
	bhs_CANJaguar m_driveRightF;
	bhs_CANJaguar m_driveLeftF;
#endif
#endif

#if USE_CONVEYOR_MOTORS
	bhs_CANJaguar m_conveyorLeft;
	bhs_CANJaguar m_conveyorRight;
	Relay m_conveyorVert;
	bhs_CANJaguar m_conveyorBack;
#endif

#if USE_SHOOTER_MOTORS
	bhs_CANJaguar m_shooterTop;
#if USE_TWO_SHOOTER_MOTORS
	bhs_CANJaguar m_shooterBottom;
#endif
#endif

#if USE_BRIDGE_LOWERING_MOTORS
	Relay m_bridgeLoweringFront;
#endif

	bool* m_reversedMotors;

};

#endif // BHS_OUTPUT_MANAGER_H
