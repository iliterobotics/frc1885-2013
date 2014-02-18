#include "bhs_SensorInput.h"

const UINT32 bhs_SensorInput::GYRO_DRIVE_CHANNEL = 1;
const UINT32 bhs_SensorInput::GYRO_BRIDGE_CHANNEL = 2;
const UINT32 bhs_SensorInput::RANGE_FINDER_CHANNEL = 3;

const UINT32 bhs_SensorInput::ENCODER_LEFT_DRIVE_CHANNEL_A = 1;
const UINT32 bhs_SensorInput::ENCODER_LEFT_DRIVE_CHANNEL_B = 2;
const UINT32 bhs_SensorInput::ENCODER_RIGHT_DRIVE_CHANNEL_A = 3;
const UINT32 bhs_SensorInput::ENCODER_RIGHT_DRIVE_CHANNEL_B = 4;
const UINT32 bhs_SensorInput::ENCODER_SHOOTER_CHANNEL_A = 5;
const UINT32 bhs_SensorInput::ENCODER_SHOOTER_CHANNEL_B = 6;
const UINT32 bhs_SensorInput::LIMIT_SWITCH_FRONT_UP_CHANNEL = 7;
const UINT32 bhs_SensorInput::LIMIT_SWITCH_FRONT_DOWN_CHANNEL = 8;
const UINT32 bhs_SensorInput::LIMIT_SWITCH_BACK_UP_CHANNEL = 9;
const UINT32 bhs_SensorInput::LIMIT_SWITCH_BACK_DOWN_CHANNEL = 10;

bhs_SensorInput::bhs_SensorInput(bhs_GlobalData* a_globalData)
	: m_globalData(a_globalData)

#if USE_DRIVE_TRAIN_GYRO
	, m_driveGyro(GYRO_DRIVE_CHANNEL)
#endif
#if USE_BRIDGE_GYRO
	, m_bridgeGyro(GYRO_BRIDGE_CHANNEL)
#endif
	, m_rangeFinder(RANGE_FINDER_CHANNEL)

#if USE_DRIVE_TRAIN_ENCODERS
	, m_driveEncoderL(ENCODER_LEFT_DRIVE_CHANNEL_A, ENCODER_LEFT_DRIVE_CHANNEL_B)
	, m_driveEncoderR(ENCODER_RIGHT_DRIVE_CHANNEL_A, ENCODER_RIGHT_DRIVE_CHANNEL_B)
#endif
#if USE_SHOOTER_ENCODER
	, m_shooterEncoder(ENCODER_SHOOTER_CHANNEL_A, ENCODER_SHOOTER_CHANNEL_B)
#endif

#if USE_BRIDGE_LIMIT_SWITCHES
	, m_backDownLimitSwitch(LIMIT_SWITCH_BACK_DOWN_CHANNEL)
	, m_backUpLimitSwitch(LIMIT_SWITCH_BACK_UP_CHANNEL)
	, m_frontDownLimitSwitch(LIMIT_SWITCH_FRONT_DOWN_CHANNEL)
	, m_frontUpLimitSwitch(LIMIT_SWITCH_FRONT_UP_CHANNEL)
#endif

#if USE_AUTONOMOUS_SWITCHES
	, m_autoSwitch1On(11)
	, m_autoSwitch1Off(12)
	, m_autoSwitch2On(13)
	, m_autoSwitch2Off(14)
#endif

#if USE_DRIVE_TRAIN_GYRO
	, m_firstDriveGyroAngle(true)
	, m_lastDriveGyroAngle(0.0)
#endif
{
#if USE_DRIVE_TRAIN_ENCODERS
	m_driveEncoderL.Start();
	m_driveEncoderR.Start();
#endif
#if USE_SHOOTER_ENCODER
	m_shooterEncoder.Start();
#endif
}

bhs_SensorInput::~bhs_SensorInput() {
	m_globalData = 0;
}

void bhs_SensorInput::init() {
	m_globalData->msd_initBatteryVoltage = DriverStation::GetInstance()->GetBatteryVoltage();

#if USE_DRIVE_TRAIN_GYRO
	m_driveGyro.Reset();
	m_firstDriveGyroAngle = true;
	m_lastDriveGyroAngle = 0.0;
#endif
#if USE_BRIDGE_GYRO
	m_bridgeGyro.Reset();
#endif

#if USE_DRIVE_TRAIN_ENCODERS
	m_driveEncoderL.Reset();
	m_driveEncoderR.Reset();
#endif
#if USE_SHOOTER_ENCODER
	m_shooterEncoder.Reset();
#endif
}

void bhs_SensorInput::run() {
#if USE_HOOP_CAMERA
	float l_angle;
	float l_distance;
	int l_count;
	m_globalData->getShooterData(l_angle, l_distance, l_count);
	m_globalData->msd_distanceToHoopFromCamera = l_distance;
#endif
	
#if USE_DRIVE_TRAIN_GYRO
	m_globalData->mdd_gyroAngle = m_driveGyro.GetAngle();
	if (!m_firstDriveGyroAngle) {
		m_globalData->mdd_gyroAngle = (m_globalData->mdd_gyroAngle + m_lastDriveGyroAngle) / 2;
	} else {
		m_firstDriveGyroAngle = false;
	}
	m_globalData->mdd_gyroBiasVoltage = AnalogModule::GetInstance(1)->GetAverageVoltage(GYRO_DRIVE_CHANNEL);
#endif
#if USE_BRIDGE_GYRO
	m_globalData->mbd_bridgeGyroAngle = m_bridgeGyro.GetAngle();
#endif

#if USE_DRIVE_TRAIN_ENCODERS
	// note: left encoder is not working
	m_globalData->mdd_encoderCounts = m_driveEncoderR.Get();
#endif
#if USE_SHOOTER_ENCODER
	m_globalData->msd_encoderRate = -m_shooterEncoder.GetRate();
	
	static const unsigned int k_flavMaxSize = 50;
	static list<float> l_flavlist;
	l_flavlist.push_back(m_globalData->msd_encoderRate);
	if (l_flavlist.size() > k_flavMaxSize) {
		l_flavlist.pop_front();
	}

	m_globalData->msd_flavMaxEncRate = -9999999;
	m_globalData->msd_flavMinEncRate = 9999999;
	m_globalData->msd_flavAvgEncRate = 0;
	for (list<float>::iterator it = l_flavlist.begin(); it != l_flavlist.end(); it++) {
		if (*it < m_globalData->msd_flavMinEncRate) {
			m_globalData->msd_flavMinEncRate = *it;
		}
		if (*it > m_globalData->msd_flavMaxEncRate) {
			m_globalData->msd_flavMaxEncRate = *it;
		}
		m_globalData->msd_flavAvgEncRate += *it;
	}
	m_globalData->msd_flavAvgEncRate /= k_flavMaxSize;
#endif
	
	//float l_distance = m_rangeFinder.GetAverageVoltage(); // TODO

#if USE_BRIDGE_LIMIT_SWITCHES
	m_globalData->mbd_backDownLimitSwitch = m_backDownLimitSwitch.Get() == 0;
	m_globalData->mbd_backUpLimitSwitch = m_backUpLimitSwitch.Get() == 0;
	m_globalData->mbd_frontDownLimitSwitch = m_frontDownLimitSwitch.Get() == 0;
	m_globalData->mbd_frontUpLimitSwitch = m_frontUpLimitSwitch.Get() == 0;

#if DISPLAY_BRIDGE_LIMIT_SWITCHES
	printf("switches: %d%d%d%d\n", m_globalData->mbd_backDownLimitSwitch, m_globalData->mbd_backUpLimitSwitch,
			m_globalData->mbd_frontDownLimitSwitch, m_globalData->mbd_frontUpLimitSwitch);

	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "ls: %d%d%d%d",
			m_globalData->mbd_backDownLimitSwitch, m_globalData->mbd_backUpLimitSwitch,
			m_globalData->mbd_frontDownLimitSwitch, m_globalData->mbd_frontUpLimitSwitch);
#endif
#endif

#if DISPLAY_AUTONOMOUS_SWITCHES
	DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "as: %d%d%d%d",
			m_autoSwitch1Off.Get(), m_autoSwitch1On.Get(),
			m_autoSwitch2Off.Get(), m_autoSwitch2On.Get());
#endif
}
