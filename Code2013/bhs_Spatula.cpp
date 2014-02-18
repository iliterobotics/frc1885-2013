#include "bhs_Spatula.h"
#define INTAKE_VELOCITY -0.55

bhs_Spatula::bhs_Spatula(){
	m_globals = GlobalData::GetInstance();
	_isUp = true;
}
void bhs_Spatula::moveDown(){
	_isUp = false;
	m_globals->o_spatula = true;
	
	setIntakeOn();
}
void bhs_Spatula::moveUp(){
	_isUp = true;
	m_globals->o_spatula = false;
}
void bhs_Spatula::setIntakeOn(){
	m_globals->o_intake = INTAKE_VELOCITY;
}
void bhs_Spatula::setIntakeOff(){
	m_globals->o_intake = 0;
}

#if 1
bool bhs_Spatula::isDown() const{
	return !_isUp;
}
bool bhs_Spatula::isUp() const{
	return _isUp;
}
#endif
#if 0
bool bhs_Spatula::isIntakeOn() const{
	return !isIntakeOff();
}

bool bhs_Spatula::isIntakeOff() const{
	return fabs(m_globals->d_intake) < 0.05;
}
#endif
