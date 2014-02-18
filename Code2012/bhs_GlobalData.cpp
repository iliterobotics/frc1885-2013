#include "bhs_GlobalData.h"

bhs_GlobalData::bhs_GlobalData()
	: m_autonomousNumber(0)
	, m_autonomousDelay(0.0)
	, m_impulseNudgeLeft(false)
	, m_impulseNudgeRight(false)
	, m_impulseNudgeForward(false)
	, m_impulseNudgeBackward(false)
	, m_extraTenthsOfSecToNudge(0)
{

}
