#include "bhs_ButtonImpulse.h"

bhs_ButtonImpulse::bhs_ButtonImpulse()
	: m_currentValue(false)
	, m_lastValue(false)
{

}

bool bhs_ButtonImpulse::apply(bool a_currentValue) {
	m_currentValue = a_currentValue;

	if (m_lastValue && m_currentValue) {
		m_currentValue = false;
	} else {
		m_lastValue = m_currentValue;
	}

	return m_currentValue;
}
