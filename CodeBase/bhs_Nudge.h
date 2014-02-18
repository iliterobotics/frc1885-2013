#ifndef BHS_NUDGE_H_
#define BHS_NUDGE_H_

#include "bhs_GlobalData.h"

class bhs_Nudge {
	public:
		static const float DT_POWER;
		static const int DEFAULT_TICKS_TO_WAIT;
		static const int EXTRA_TICKS_TO_WAIT;
		
		bhs_Nudge(bhs_GlobalData*);
		
		void run();
		
	private:
		typedef enum NudgeState {
			k_stopped = 0,
			k_forward,
			k_backward,
			k_left,
			k_right
		};
		
		void processTicks();
		
		bhs_GlobalData* m_gd;
		
		NudgeState m_state;	
		int m_ticksToWait;
		int m_ticks;

};

#endif //BHS_NUDGE_H_
