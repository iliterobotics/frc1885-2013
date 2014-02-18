#ifndef BHS_GLOBAL_DATA_H_
#define BHS_GLOBAL_DATA_H_

#include "bhs_Constants.h"
#include "bhs_GDDrivetrain.h"
#include "bhs_GDSubsys1.h"
#include "bhs_GDSubsys2.h"
#include "bhs_GDSubsys3.h"

class bhs_GlobalData
	: public bhs_GDDrivetrain
	, public bhs_GDSubsys1
	, public bhs_GDSubsys2
	, public bhs_GDSubsys3
{
	public:
		bhs_GlobalData();

		int m_autonomousNumber;
		float m_autonomousDelay;		
};

#endif // BHS_GLOBAL_DATA_H_
