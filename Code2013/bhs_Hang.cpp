
#include "bhs_Hang.h"

GlobalData* bhs_Hang::m_globals= GlobalData::GetInstance();

bhs_Hang::bhs_Hang(){
	m_globals = GlobalData::GetInstance();
}
void bhs_Hang::extend(){
	m_globals->o_hang = DoubleSolenoid::kForward;
}
void bhs_Hang::retract(){
	m_globals->o_hang = DoubleSolenoid::kReverse;
}

#if 1
bool bhs_Hang::isExtended() {
	return m_globals->o_hang;
}
bool bhs_Hang::isRetracted() {
	return !isExtended();
}
#endif
