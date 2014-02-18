#ifndef MAIN_H
#define MAIN_H

#include "WPILib.h"
#include <Math.h>
#include <sysLib.h>

#include "GlobalData.h"
#include "Input.h"
#include "Output.h"
#include "NetDriver.h"
#include "NetVision.h"
#include "PID.h"
#include "bhs_Autonomous.h"
#include "Processing.h"

class Main : public SimpleRobot
{

public:
	Main();
	~Main();

	void Autonomous();
	void OperatorControl();
	void Disabled();
	void Test();

private:
	NetDriver* netDriver;
	NetVision* netVision;
	
	static GlobalData* m_globals;
	bhs_Autonomous* m_autonomous;
	static void netDriverConnect(Main*, NetDriver*, GlobalData*);
#if 0
	static int netDriverRecv(const NetDriver&);
#endif
	static void netVisionConnect(Main*, NetVision*, GlobalData*);
	
	static PID* m_turnPID;
	bool m_started; // for threads
};


#endif
