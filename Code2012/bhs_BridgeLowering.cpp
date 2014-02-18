#include "bhs_BridgeLowering.h"

bhs_BridgeLowering::bhs_BridgeLowering(bhs_GDBridgeData* a_bridgeData)
	: m_bridgeData(a_bridgeData)
{

}

bhs_BridgeLowering::~bhs_BridgeLowering() {
	m_bridgeData = 0;
}

void bhs_BridgeLowering::run() {
	m_bridgeData->mbd_bridgeFrontArmValue = Relay::kOff;

	if (m_bridgeData->mbd_buttonFrontUp && !m_bridgeData->mbd_frontUpLimitSwitch) {
		m_bridgeData->mbd_bridgeFrontArmValue = Relay::kReverse;
	} else if (m_bridgeData->mbd_buttonFrontDown && !m_bridgeData->mbd_frontDownLimitSwitch) {
		m_bridgeData->mbd_bridgeFrontArmValue = Relay::kForward;
	} else if (m_bridgeData->mbd_buttonFrontUp2 && !m_bridgeData->mbd_frontUpLimitSwitch) {
		m_bridgeData->mbd_bridgeFrontArmValue = Relay::kReverse;
	} else if (m_bridgeData->mbd_buttonFrontDown2 && !m_bridgeData->mbd_frontDownLimitSwitch) {
		m_bridgeData->mbd_bridgeFrontArmValue = Relay::kForward;
	} 
}
