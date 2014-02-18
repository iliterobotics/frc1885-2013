#include "Output.h"
#include "Processing.h"

GlobalData* Output::m_globals= GlobalData::GetInstance();

void Output :: OutputToActuators ()
{
	// Sorted from lowest priority to highest priority
#if 1
	// Drivetrain
	Processing::processDriveTrain();	//FIXME: remove when Processing.cpp is completely debugged and bhs_SSD obsolete
	m_globals->g_DTLeftA->SetSpeed( m_globals->o_drivetrainLeft );
	m_globals->g_DTLeftB->SetSpeed( m_globals->o_drivetrainLeft );
	m_globals->g_DTRightA->SetSpeed( m_globals->o_drivetrainRight );
	m_globals->g_DTRightB->SetSpeed( m_globals->o_drivetrainRight );

	// Launcher position
	//printf( "o_launchertilt: %f\n", m_globals->o_launcherTilt );
	// This clamp prevents the tilt from moving too far down or up
	// If it's moving too far in either directions, STAHP.
	if ((m_globals->i_launcherTiltPotentiometer <= MIN_VOLTAGE && m_globals->o_launcherTilt < 0) ||
		(m_globals->i_launcherTiltPotentiometer >= MAX_VOLTAGE && m_globals->o_launcherTilt > 0))
		m_globals->o_launcherTilt = 0;
	m_globals->g_launcherTilt->SetSpeed( m_globals->o_launcherTilt );
	m_globals->g_launcherFire->SetSpeed(m_globals->o_launcherSpeed);
	
	// Finger
	m_globals->g_finger->Set( m_globals->o_finger );

	// Relay lights
	m_globals->g_light1->Set(m_globals->o_light1State);
	m_globals->g_light2->Set(m_globals->o_light2State);
	//printf( "%d\t%d\n", m_globals->o_light1State, m_globals->o_light2State );
	
	// Hang
	m_globals->g_hang->Set(m_globals->o_hang);

	// Spatula
	m_globals->g_spatula->Set( m_globals->o_spatula );
	m_globals->g_intake->SetSpeed( m_globals->o_intake );

	// Hopper
	m_globals->g_hopper->Set(m_globals->o_hopper);

	// Kicker
	if ( !m_globals->d_launcherFireSequence && !m_globals->d_manualKick )
		m_globals->s_launcher->forceStopLaunch();
	m_globals->g_launcherKicker->Set( m_globals->o_launcherKicker );
	
#endif
}
