#include "bhs_GDConveyorData.h"

bhs_GDConveyorData::bhs_GDConveyorData()
	: mcd_buttonIntakeCollect(false)
	, mcd_buttonIntakeReverse(false)
	
	, mcd_buttonFeederFeeding(false)
	, mcd_buttonFeederReverse(false)
	
	, mcd_leftConveyorPower(0.0)
	, mcd_vertConveyorValue(Relay::kOff)
	, mcd_rightConveyorPower(0.0)
	, mcd_backConveyorPower(0.0)
{
}
