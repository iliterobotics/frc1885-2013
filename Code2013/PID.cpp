#include "PID.h"

PID :: PID ()
{
	m_globals= GlobalData::GetInstance();
	m_error = m_integral = 0.0;
}

PID :: ~PID ()
{

}

void PID :: setConstants (const double P, const double I, const double D )
{
	m_globals->c_turnP = P;
	m_globals->c_turnI = I;
	m_globals->c_turnD = D;
}

double PID :: getPID (const double pv, const double sp, const double dt )
{
	m_previousError = m_error;
	m_error = sp - pv;

	//return dt == 0 ? 0 : m_globals->m_P * getP() + m_globals->m_I * getI(dt) + m_globals->m_D/dt * getD();
	return (m_globals->c_turnP * getP()) + (m_globals->c_turnI * getI(1.0)) +
		(m_globals->c_turnD * getD());
}

double PID :: getP () const
{
	return m_error;
}

double PID :: getI (const double dt )
{
	return m_integral+= m_error * dt;
}

double PID :: getD () const
{
	return m_error - m_previousError;
}
