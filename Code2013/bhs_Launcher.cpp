
#include "GlobalData.h"
#include "bhs_Launcher.h"
#include "SpeedBuffer.h"
#include "Output.h"
#include "Constants.h"
#include "Processing.h"


//const float POSITION[] = { PRESET1_ANGLE, PRESET2_ANGLE };

bhs_Launcher :: bhs_Launcher ()
{
	m_globals = GlobalData::GetInstance();
	
	m_tiltPID = new PID();
	m_tiltPID->setConstants( m_globals->c_tiltP, m_globals->c_tiltI, m_globals->c_tiltD );

	m_kickerState = false;
	m_timer = new Timer();
	m_timer->Reset();
}

bhs_Launcher :: ~bhs_Launcher ()
{

}


void bhs_Launcher :: moveToAngle ( double setpoint, double current )
{
	if (setpoint < MIN_ANGLE)
		setpoint = MIN_ANGLE;
	else if (setpoint > MAX_ANGLE)
		setpoint = MAX_ANGLE;
	
	double pidOutput = m_tiltPID->getPID( current, setpoint, 1 );

	m_globals->o_launcherTilt = m_globals->m_speedBuffer->BufferedSpeed( m_globals->g_launcherTilt, pidOutput );
}

double bhs_Launcher :: getAngle ()
{
	return Processing::launcherPotToAngle(m_globals->i_launcherTiltPotentiometer);
}

bool bhs_Launcher :: isAtDesiredAngle ()
{
	return fabs(getAngle()-m_globals->i_launcherDesiredTilt) < TILT_ANGLE_TOLERANCE;
}

void bhs_Launcher :: moveToSpeed ( double speed )
{
	if ( speed < SHOOTER_SPEED_TOLERANCE )
		m_globals->o_launcherSpeed = 0;
	else
		m_globals->o_launcherSpeed = speed * m_globals->i_powerFactor;
}


double bhs_Launcher :: getSpeed ()
{
	return m_globals->g_launcherFire->Get();
}

bool bhs_Launcher :: isAtDesiredSpeed ()
{
	return true;
	//return fabs(getSpeed()-m_globals->i_launcherSpeed) < SHOOTER_SPEED_TOLERANCE;
}

void bhs_Launcher :: launch( bool doLaunch )
{	
	if ( !doLaunch )
	{
		m_timer->Stop();
		m_timer->Reset();
		m_kickerState = false;
	}
	else
	{
		if (!m_timer->HasPeriodPassed(0))
		{
			m_timer->Start();
		}
		else if ( m_timer->Get() > KICK_DELAY )
		{	
			m_timer->Reset();
			m_kickerState = !m_kickerState;
		}
		else
		{
			/* time passes. */
		}
	}

	if ( m_kickerState )
	{
		kickerOut();
	}
	else
	{
		kickerIn();
	}

}

void bhs_Launcher :: forceStopLaunch()
{
	kickerIn();
}

void bhs_Launcher :: kickerOut ()
{
	m_globals->o_launcherKicker = true;
}

void bhs_Launcher :: kickerIn ()
{
	m_globals->o_launcherKicker = false;
}

// true if out. false if in.
bool bhs_Launcher :: getKickerState ()
{
	//return m_kickerState.get();
	return m_kickerState;
}

void bhs_Launcher :: moveToVoltage( double voltage )
{
	// This is speed for going from / to _.
	double motorVal = 0.0;
	double potVal = m_globals->i_launcherCurrentTilt;

	if(fabs(voltage - potVal) < LAUNCH_POT_TOLERANCE)
	{
		motorVal = 0.0;
	}
	else if(voltage > m_globals->i_launcherCurrentTilt)
	{
		// If we want to go from _ to /
		motorVal = -LAUNCH_DEFAULT_SPEED;
	}
	else
	{
		motorVal = LAUNCH_DEFAULT_SPEED;
	}

	m_globals->o_launcherTilt = motorVal;
}
