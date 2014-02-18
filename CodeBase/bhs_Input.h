#ifndef BHS_INPUT_H_
#define BHS_INPUT_H_

#include "bhs_GlobalData.h"

class bhs_Input {
	public:
		virtual void init() = 0;
		virtual void run() = 0;

	protected:
		bhs_GlobalData* m_gd;
};

#endif //BHS_INPUT_H_
