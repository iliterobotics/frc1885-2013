#ifndef BHS_SUBSYS1_H_
#define BHS_SUBSYS1_H_

#include "bhs_Subsystem.h"
#include "bhs_GDSubsys1.h"

class bhs_Subsys1 : public bhs_Subsystem {
	public:
		bhs_Subsys1(bhs_GDSubsys1*);
		virtual ~bhs_Subsys1();

		virtual void run();

	private:
		bhs_GDSubsys1* m_sd;

	// Variable declarations go here
};

#endif //BHS_SUBSYS1_H_
