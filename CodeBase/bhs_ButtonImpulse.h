#ifndef BHS_BUTTON_IMPULSE_H_
#define BHS_BUTTON_IMPULSE_H_

class bhs_ButtonImpulse {
	public:
		bhs_ButtonImpulse();

		bool apply(bool);

	private:
		bool m_currentValue;
		bool m_lastValue;

};

#endif //BHS_BUTTON_IMPULSE_H_
