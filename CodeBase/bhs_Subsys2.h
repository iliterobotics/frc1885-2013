#ifndef BHS_SUBSYS2_H_
#define BHS_SUBSYS2_H_

#include "bhs_Subsystem.h"
#include "bhs_GDSubsys2.h"

class bhs_Subsys2 : public bhs_Subsystem {
	public:
		bhs_Subsys2(bhs_GDSubsys2*);
		virtual ~bhs_Subsys2();

		virtual void run();

	private:
		bhs_GDSubsys2* m_sd;

		// Variable declarations go here
};

#endif //BHS_SUBSYS2_H_
