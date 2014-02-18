#if 0

#include "bhs_CAM.h"

GlobalData* bhs_CAM::m_globals= GlobalData::GetInstance();

bhs_CAM::bhs_CAM()
{
	
}

bool bhs_CAM::lightCheck()
{	
	/*
	Lsum += m_globals->i_lightSensorVoltage;
	Lcount++;
	Lavg = Lsum / Lcount;
	
	if(m_globals->i_lightSensorVoltage < (Lavg + 1) && m_globals->i_lightSensorVoltage > (Lavg - 1))
		return true;
	else
		return false;
	*/
	if(m_globals->i_lightSensorVoltage < 2)
		return true;
	else
		return false;
}
	
void bhs_CAM::initPos()
{

}

void bhs_CAM::setPosition(Position p)
{

}

bhs_CAM::Position bhs_CAM::getPosition() const
{
	return currPos;
}

#endif
