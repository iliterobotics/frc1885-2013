#ifndef BHS_AUTONOMOUS_H_
#define BHS_AUTONOMOUS_H_

#include <DriverStation.h>
#include "Timer.h"

#include "bhs_GlobalData.h"
#include "bhs_Input.h"

class bhs_Autonomous : public bhs_Input {
	public:
		bhs_Autonomous(bhs_GlobalData*);
		virtual ~bhs_Autonomous();

		virtual void init();
		virtual void run();
		
	private:
		typedef enum {
			k_state1 = 0
			// ...
		} State;
		
		bhs_GlobalData* m_gd;
		State m_state;

		Timer m_timer;
};

#endif //BHS_AUTONOMOUS_H_
