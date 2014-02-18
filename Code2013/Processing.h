#ifndef PROCESSING_H
#define PROCESSING_H

#include <Math.h>
#include "GlobalData.h"
#include "PID.h"
#include "Timer.h"

class Processing
{

//FIXME: make private once drivetrain processing is removed from bhs_SSD
public:
	static void processDriveTrain();
	static void processLauncherPosition();
	static double launcherPotToAngle(double pot);
	
	static void process();

private:
	Processing();
	
	static void processFinger();
	static void processHopper();
	static void processSpatula();
	static void processManualKick();
	//void processDriveTrain();
		static void driveStraight(const double speed);
		static void turnDegrees(const double setPoint);
			static double angleDifference(double a, double b);

	//void processLauncherPosition();
		static double dsTiltPotToAngle(double pot);
		static double dsSpeedPotScale(double x);
		//static double launcherPotToAngle(double pot);
		static double launcherAngleToPot(double angle);
	static void processAutoFireSequence();
		static bool isReadyToLaunch();
		static void ensureReadyToLaunch();
	static void processHang();
		static bool isReadyToHang();
		static void ensureReadyToHang();

	static GlobalData* gd;
	
#if 1
	static PID m_straightPID;
	static PID m_turnPID;
	static int m_loadBlinkCount;
#endif
	
	// Auto fire sequence states
	static int m_fireState;
	enum
	{
		K_INIT = 0,
		K_ACQUIRE_DATA,
		K_ALIGNMENT,
		K_TRANSIT,
		K_FIRE
	};
	static int m_dataAttempt;
	static Timer* m_kickerTimer;

};

#endif
