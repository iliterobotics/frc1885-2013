#include "bhs_Hopper.h"

bhs_Hopper::bhs_Hopper()
{
	m_globals = GlobalData::GetInstance();
}

void bhs_Hopper::putDown(){
	m_globals->o_hopper = DoubleSolenoid::kForward;
}

void bhs_Hopper::putUp(){
	m_globals->o_hopper = DoubleSolenoid::kReverse;
}

#if 1
void bhs_Hopper::updateState()
{
	m_globals->i_hopperLimitSwitchDown = m_globals->g_hopperLimitSwitchDown->Get();
	m_globals->i_hopperLimitSwitchUp = m_globals->g_hopperLimitSwitchUp->Get();
}

bool bhs_Hopper::isDown(bool forceUpdate) 
{
	if(forceUpdate)
		updateState();
	
	//return m_globals->i_hopperLimitSwitchDown;
	return m_globals->o_hopper == DoubleSolenoid::kForward;
}
bool bhs_Hopper::isUp(bool forceUpdate) 
{
	if(forceUpdate)
		updateState();
	
	//return m_globals->i_hopperLimitSwitchUp;
	return !isDown();
}
#endif
