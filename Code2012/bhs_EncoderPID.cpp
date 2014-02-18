#include "bhs_EncoderPID.h"

const double bhs_EncoderPID::DEFAULT_MAX_PID_ADDITION = 0.2;

bhs_EncoderPID::bhs_EncoderPID()
	: m_pid()
	, m_maxPIDAddition(DEFAULT_MAX_PID_ADDITION)
{
}

void bhs_EncoderPID::setMaxPIDAddition(double a_maxPIDAddition) {
	m_maxPIDAddition = a_maxPIDAddition;
}

double bhs_EncoderPID::getMaxPIDAddition() {
	return m_maxPIDAddition;
}

void bhs_EncoderPID::setPID(double a_p, double a_i, double a_d) {
	m_pid.init(a_p, a_i, a_d);
}

float bhs_EncoderPID::getMotorPowFromEncRate(float a_steadyStatePower, double a_currentRate, double a_desiredRate) {
#if 1
	double l_pidAddition = m_pid.apply(a_desiredRate - a_currentRate);
#else
	double l_newPower = m_pid.apply(a_desiredRate - a_currentRate);
	double l_pidAddition = l_newPower - a_steadyStatePower;
#endif
	if (l_pidAddition > m_maxPIDAddition) {
		l_pidAddition = m_maxPIDAddition;
	} else if (l_pidAddition < -m_maxPIDAddition) {
		l_pidAddition = -m_maxPIDAddition;
	}
	return a_steadyStatePower + l_pidAddition;
}
