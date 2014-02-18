#include "bhs_Shooter.h"

#define abs(x) ((x) >= 0 ? (x) : -(x))

const float bhs_Shooter::LAY_UP_POWER = 0.8;
const float bhs_Shooter::POWER_SHOT_POWER = 1.0;

// any power between 0 and POW_RAMP_A: jump
// any power between POW_RAMP_A and POW_RAMP_B: ramp by POW_RAMP_INC
// any power between POW_RAMP_B and 1: jump
const float bhs_Shooter::POW_RAMP_A = 0.16;
const float bhs_Shooter::POW_RAMP_B = 0.32;
const float bhs_Shooter::POW_RAMP_INC = 0.02;

bhs_Shooter::bhs_Shooter(bhs_GDShooterData* a_shooterData)
: m_shooterData(a_shooterData)
, m_shooterProcess()
, m_encoderPID()
, m_constantOverridePower(false)
, m_desiredConstantPower(0.0)
{
	m_encoderPID.setPID(0.0001666, 0, 0);
	//m_encoderPID.setPID(0, 0.00004, 0);
	//m_encoderPID.setPID(0, 0.00000128, 0);
}

bhs_Shooter::~bhs_Shooter() {
	m_shooterData = 0;
}

void bhs_Shooter::run() {
#if TEST_SHOOTER_PID
	m_encoderPID.setPID(m_shooterData->msd_p, m_shooterData->msd_i, m_shooterData->msd_d);
#endif

#if TEST_SHOOTER_CONST_POWER
	m_shooterData->msd_shooterTarget = 0.7;
#else // !TEST_SHOOTER_CONST_POWER
	if (m_shooterData->msd_switchSpeedOverride) {
		if (m_shooterData->msd_buttonLayUp) {
			m_constantOverridePower = true;
			m_desiredConstantPower = LAY_UP_POWER;
		} else if (m_shooterData->msd_buttonPowerShot) {
			m_constantOverridePower = true;
			m_desiredConstantPower = POWER_SHOT_POWER;
		}

		if (m_constantOverridePower) {
			//shooterData->msd_shooterTarget = m_desiredConstantPower;
			m_shooterData->msd_desiredRate = m_desiredConstantPower * 15200;
		} else {
			//m_shooterData->msd_shooterTarget = m_shooterData->msd_potentiometerReading;
			m_shooterData->msd_desiredRate = m_shooterData->msd_potentiometerReading * 15200;
		}
	} else {
		m_constantOverridePower = false;

		if (m_shooterData->msd_impulseLockDistanceForShooter) {
			double l_distance = m_shooterData->msd_distanceToHoopFromCamera;
			if (m_shooterData->msd_buttonHoopOverrideTop) {
				l_distance += 36;
			}
			m_shooterData->msd_desiredRate = m_shooterProcess.getRateFromDistance(l_distance);
		}
	}

#if 1
	m_shooterData->msd_shooterTarget = m_shooterProcess.getPowerFromRate(m_shooterData->msd_desiredRate);
#endif

	float l_old = m_shooterData->msd_shooterTarget;
	m_shooterData->msd_shooterTarget = m_encoderPID.getMotorPowFromEncRate(
			m_shooterData->msd_shooterTarget,
			m_shooterData->msd_encoderRate,
			m_shooterData->msd_desiredRate);
	m_shooterData->msd_pidAddition = m_shooterData->msd_shooterTarget - l_old;

#if 0
	m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
#endif

#if 1
	if (m_shooterData->msd_shooterPower >= POW_RAMP_A && m_shooterData->msd_shooterTarget >= POW_RAMP_A) {
		if (m_shooterData->msd_shooterPower >= POW_RAMP_B && m_shooterData->msd_shooterTarget >= POW_RAMP_B) {
			m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
		} else if (m_shooterData->msd_shooterPower >= POW_RAMP_B && m_shooterData->msd_shooterTarget < POW_RAMP_B) {
			m_shooterData->msd_shooterPower = POW_RAMP_B;
		} else {
			if (m_shooterData->msd_shooterPower < m_shooterData->msd_shooterTarget) {
				if (m_shooterData->msd_shooterTarget - m_shooterData->msd_shooterPower < POW_RAMP_INC) {
					m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
				} else {
					m_shooterData->msd_shooterPower += POW_RAMP_INC;
				}
			} else if (m_shooterData->msd_shooterPower > m_shooterData->msd_shooterTarget) {
				if (m_shooterData->msd_shooterPower - m_shooterData->msd_shooterTarget < POW_RAMP_INC) {
					m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
				} else {
					m_shooterData->msd_shooterPower -= POW_RAMP_INC;
				}
			}
		}
	} else if (m_shooterData->msd_shooterPower < POW_RAMP_A && m_shooterData->msd_shooterTarget >= POW_RAMP_A) {
		m_shooterData->msd_shooterPower = POW_RAMP_A;
	} else {
		if (m_shooterData->msd_shooterPower > m_shooterData->msd_shooterTarget) { // if old power is greater than new power (if ramping down)
			if (m_shooterData->msd_shooterPower < m_shooterData->msd_shooterTarget) {
				if (m_shooterData->msd_shooterTarget - m_shooterData->msd_shooterPower < POW_RAMP_INC) {
					m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
				} else {
					m_shooterData->msd_shooterPower += POW_RAMP_INC;
				}
			} else if (m_shooterData->msd_shooterPower > m_shooterData->msd_shooterTarget) {
				if (m_shooterData->msd_shooterPower - m_shooterData->msd_shooterTarget < POW_RAMP_INC) {
					m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
				} else {
					m_shooterData->msd_shooterPower -= POW_RAMP_INC;
				}
			}
		} else {
			m_shooterData->msd_shooterPower = m_shooterData->msd_shooterTarget;
		}
	}
#endif

	if (m_shooterData->msd_shooterPower > 1.0) {
		m_shooterData->msd_shooterPower = 1.0;
	} else if (m_shooterData->msd_shooterPower < -1.0) {
		m_shooterData->msd_shooterPower = -1.0;
	}
#endif // TEST_SHOOTER_CONST_POWER
}
