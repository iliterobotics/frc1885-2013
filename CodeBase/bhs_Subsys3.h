#ifndef BHS_SUBSYS3_H_
#define BHS_SUBSYS3_H_

#include "bhs_Subsystem.h"
#include "bhs_GDSubsys3.h"

class bhs_Subsys3 : public bhs_Subsystem {
	public:
		bhs_Subsys3(bhs_GDSubsys3*);
		virtual ~bhs_Subsys3();

		virtual void run();

	private:
		bhs_GDSubsys3* m_sd;

		// Variable declarations go here
};

#endif //BHS_SUBSYS3_H_
