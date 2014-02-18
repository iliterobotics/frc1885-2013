#ifndef BHS_AUTONOMOUS_BASIC_WITH_BRIDGE_H
#define BHS_AUTONOMOUS_BASIC_WITH_BRIDGE_H

#define DEBUG_AUTONOMOUS_STATES 1
#define DEBUG_AUTONOMOUS_SHOOTER 1

#include "DriverStation.h"
#include "DriverStationLCD.h"
#include "Timer.h"

#include "bhs_GlobalData.h"
#include "bhs_Autonomous.h"

/**
 * The robot starts up the shooter and waits for a certain amount of time.
 * Then, the robot shoots in pulses.
 * Afterwards, if the drivers choose to, the robot will move back towards the bridge
 * and lower the bridge. It stops completely when the arm limit switch is set.
 */
class bhs_AutonomousBasicWithBridge : public bhs_Autonomous {
public:
	static const UINT32 MOVE_TO_BRIDGE_DIGITAL_INPUT;
	static const UINT32 DELAY_ANALOG_INPUT;
	static const UINT32 LESS_DISTANCE_ANALOG_INPUT;
	
	static const float SHOOTER_POWER;
	static const float DRIVE_TRAIN_POWER;
	
	static const int NUM_BALLS_TO_SHOOT;
	
	static const double MIN_DELAY_DURATION;
	static const double PULSE_SHOOT_DURATION;
	static const double PULSE_STOP_DURATION;
	static const double MAX_MOVE_TO_BRIDGE_DURATION;
	
	static const double DT_ENCODER_TICKS_PER_INCH;
	static const double MOVE_TO_BRIDGE_DISTANCE_IN_INCHES;
	static const double LESS_DISTANCE_PER_ANALOG_TICK_IN_INCHES;
	
	static const float DES_ENC_RATE_HARD_BALL;
	static const float DES_ENC_RATE_SOFT_BALL;
	static const float DES_ENC_RATE_HARD_BALL_SIDE;
	static const float DES_ENC_RATE_SOFT_BALL_SIDE;

	bhs_AutonomousBasicWithBridge(bhs_GlobalData*);
	~bhs_AutonomousBasicWithBridge();

	virtual void init();
	virtual void run();

private:
	typedef enum {
		k_startShooter = 0,
		k_waitForShooter,
		k_moveConveyor,
		k_stopConveyor,
		k_moveToBridge,
		k_lowerArm,
		k_halt
	} State;

	bhs_GlobalData* m_globalData;
	State m_state;

	Timer m_timer;
	int m_numBallsShot;
	
	int m_initEncoder; // the initial encoder count
	bool m_moveToBridgeSet; // if the button for moving to the bridge is set on the driver station
	double m_lessEncoderCounts; // instead of going the full 112 inches, we go a little bit less depending on this value
	int m_encoderTarget;
	
	bool* m_softBalls;
	bool m_shootingFromSide;

};

#endif
