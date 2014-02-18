#include "bhs_Nudge.h"

const float bhs_Nudge::DT_POWER = 0.325;
const int bhs_Nudge::DEFAULT_TICKS_TO_WAIT = 2;
const int bhs_Nudge::EXTRA_TICKS_TO_WAIT = 2;

bhs_Nudge::bhs_Nudge(bhs_GlobalData* a_gd) {
	m_gd = a_gd;
	m_state = k_stopped;
	m_ticksToWait = DEFAULT_TICKS_TO_WAIT;
	m_ticks = 0;
}

void bhs_Nudge::run() {
	m_ticksToWait = DEFAULT_TICKS_TO_WAIT + m_gd->mdd_extraTenthsOfSecToNudge * EXTRA_TICKS_TO_WAIT;

	if (m_gd->mdd_impulseNudgeForward) 
		m_state = k_forward;
	else if (m_gd->mdd_impulseNudgeBackward) 
		m_state = k_backward;
	else if (m_gd->mdd_impulseNudgeLeft)
		m_state = k_left;
	else if (m_gd->mdd_impulseNudgeRight)
		m_state = k_right;

	switch (m_state) {
	case k_stopped:
		break;
	case k_forward:
		m_gd->mdd_driveLeftPower = DT_POWER;
		m_gd->mdd_driveRightPower = DT_POWER;
		processTicks();
		break;
	case k_backward:
		m_gd->mdd_driveLeftPower = -DT_POWER;
		m_gd->mdd_driveRightPower = -DT_POWER;
		processTicks();
		break;
	case k_left:
		m_gd->mdd_driveLeftPower = -DT_POWER;
		m_gd->mdd_driveRightPower = DT_POWER;
		processTicks();
		break;
	case k_right:
		m_gd->mdd_driveLeftPower = DT_POWER;
		m_gd->mdd_driveRightPower = -DT_POWER;
		processTicks();
		break;
	}
}

void bhs_Nudge::processTicks() {
	if (m_ticks > m_ticksToWait) {
		m_gd->mdd_driveLeftPower = 0;
		m_gd->mdd_driveRightPower = 0;
		m_ticks = 0;
		m_state = k_stopped;
	} else {
		m_ticks++;
	}
}
