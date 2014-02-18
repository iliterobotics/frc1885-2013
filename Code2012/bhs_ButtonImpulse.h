#ifndef BHS_BUTTON_IMPULSE_H
#define BHS_BUTTON_IMPULSE_H

class bhs_ButtonImpulse {
public:
	bhs_ButtonImpulse();

	bool apply(bool);

private:
	bool m_currentValue;
	bool m_lastValue;

};

#endif
