#ifndef BHS_SENSOR_INPUT_H
#define BHS_SENSOR_INPUT_H

#include <list>

#include "Joystick.h"
#include "AnalogModule.h"
#include "AnalogChannel.h"
#include "Encoder.h"
#include "DigitalInput.h"
#include "DriverStation.h"
#include "DriverStationLCD.h"

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"
#include "bhs_Gyro.h"

using namespace std;

class bhs_SensorInput {
public:
	// analog inputs
	static const UINT32 GYRO_DRIVE_CHANNEL;
	static const UINT32 GYRO_BRIDGE_CHANNEL;
	static const UINT32 RANGE_FINDER_CHANNEL;
	
	// digital inputs
	static const UINT32 ENCODER_LEFT_DRIVE_CHANNEL_A;
	static const UINT32 ENCODER_LEFT_DRIVE_CHANNEL_B;
	static const UINT32 ENCODER_RIGHT_DRIVE_CHANNEL_A;
	static const UINT32 ENCODER_RIGHT_DRIVE_CHANNEL_B;
	static const UINT32 ENCODER_SHOOTER_CHANNEL_A;
	static const UINT32 ENCODER_SHOOTER_CHANNEL_B;
	static const UINT32 LIMIT_SWITCH_FRONT_UP_CHANNEL;
	static const UINT32 LIMIT_SWITCH_FRONT_DOWN_CHANNEL;
	static const UINT32 LIMIT_SWITCH_BACK_UP_CHANNEL;
	static const UINT32 LIMIT_SWITCH_BACK_DOWN_CHANNEL;
	
	bhs_SensorInput(bhs_GlobalData*);
	~bhs_SensorInput();

	void init();
	void run();

private:
	bhs_GlobalData* m_globalData;

#if USE_DRIVE_TRAIN_GYRO
	bhs_Gyro m_driveGyro;
#endif
#if USE_BRIDGE_GYRO
	bhs_Gyro m_bridgeGyro;
#endif
	AnalogChannel m_rangeFinder;

#if USE_DRIVE_TRAIN_ENCODERS	
	Encoder m_driveEncoderL;
	Encoder m_driveEncoderR;
#endif
#if USE_SHOOTER_ENCODER
	Encoder m_shooterEncoder;
#endif

#if USE_BRIDGE_LIMIT_SWITCHES
	DigitalInput m_backDownLimitSwitch;
	DigitalInput m_backUpLimitSwitch;
	DigitalInput m_frontDownLimitSwitch;
	DigitalInput m_frontUpLimitSwitch;
#endif

#if USE_AUTONOMOUS_SWITCHES
	DigitalInput m_autoSwitch1On;
	DigitalInput m_autoSwitch1Off;
	DigitalInput m_autoSwitch2On;
	DigitalInput m_autoSwitch2Off;
#endif

#if USE_DRIVE_TRAIN_GYRO
	bool m_firstDriveGyroAngle;
	float m_lastDriveGyroAngle;
#endif

};

#endif
