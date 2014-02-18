#include "bhs_Nudge.h"

const float bhs_Nudge::DT_POWER = 0.325;
const int bhs_Nudge::DEFAULT_TICKS_TO_WAIT = 2;
const int bhs_Nudge::EXTRA_TICKS_TO_WAIT = 2;

bhs_Nudge::bhs_Nudge(bhs_GlobalData* a_globalData)
	: m_globalData(a_globalData)
	, m_state(k_stopped)
	, m_ticksToWait(DEFAULT_TICKS_TO_WAIT)
	, m_ticks(0)
{

}

void bhs_Nudge::run() {
	m_ticksToWait = DEFAULT_TICKS_TO_WAIT + m_globalData->m_extraTenthsOfSecToNudge * EXTRA_TICKS_TO_WAIT;

	if (m_globalData->m_impulseNudgeForward) {
		m_state = k_forward;
	} else if (m_globalData->m_impulseNudgeBackward) {
		m_state = k_backward;
	} else if (m_globalData->m_impulseNudgeLeft) {
		m_state = k_left;
	} else if (m_globalData->m_impulseNudgeRight) {
		m_state = k_right;
	}

	switch (m_state) {
	case k_stopped:
		break;
	case k_forward:
		m_globalData->mdd_driveLeftPower = DT_POWER;
		m_globalData->mdd_driveRightPower = DT_POWER;
		processTicks();
		break;
	case k_backward:
		m_globalData->mdd_driveLeftPower = -DT_POWER;
		m_globalData->mdd_driveRightPower = -DT_POWER;
		processTicks();
		break;
	case k_left:
		m_globalData->mdd_driveLeftPower = -DT_POWER;
		m_globalData->mdd_driveRightPower = DT_POWER;
		processTicks();
		break;
	case k_right:
		m_globalData->mdd_driveLeftPower = DT_POWER;
		m_globalData->mdd_driveRightPower = -DT_POWER;
		processTicks();
		break;
	}
}

void bhs_Nudge::processTicks() {
	if (m_ticks > m_ticksToWait) {
		m_globalData->mdd_driveLeftPower = 0;
		m_globalData->mdd_driveRightPower = 0;
		m_ticks = 0;
		m_state = k_stopped;
	} else {
		m_ticks++;
	}
}
