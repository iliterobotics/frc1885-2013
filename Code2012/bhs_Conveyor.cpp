#include "bhs_Conveyor.h"

const float bhs_Conveyor::INTAKE_POWER = 1.0;
const float bhs_Conveyor::FEEDER_POWER = 0.7;

bhs_Conveyor::bhs_Conveyor(bhs_GDConveyorData* a_conveyorData)
	: m_conveyorData(a_conveyorData)
{
}

bhs_Conveyor::~bhs_Conveyor() {
	m_conveyorData = 0;
}

void bhs_Conveyor::run() {
	if (m_conveyorData->mcd_buttonIntakeCollect) {
		m_conveyorData->mcd_leftConveyorPower = INTAKE_POWER;
		m_conveyorData->mcd_vertConveyorValue = Relay::kForward;
		m_conveyorData->mcd_backConveyorPower = INTAKE_POWER;
	} else if (m_conveyorData->mcd_buttonIntakeReverse) {
		m_conveyorData->mcd_leftConveyorPower = -INTAKE_POWER;
		m_conveyorData->mcd_vertConveyorValue = Relay::kOff;
		m_conveyorData->mcd_backConveyorPower = -INTAKE_POWER;
	} else {
		m_conveyorData->mcd_leftConveyorPower = 0.0;
		m_conveyorData->mcd_vertConveyorValue = Relay::kOff;
		m_conveyorData->mcd_backConveyorPower = 0.0;
	}
	
	if (m_conveyorData->mcd_buttonFeederFeeding) {
		m_conveyorData->mcd_rightConveyorPower = FEEDER_POWER;
	} else if (m_conveyorData->mcd_buttonFeederReverse) {
		m_conveyorData->mcd_rightConveyorPower = -FEEDER_POWER;
	} else {
		m_conveyorData->mcd_rightConveyorPower = 0.0;
	}
}
