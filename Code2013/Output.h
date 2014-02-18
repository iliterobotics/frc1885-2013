#ifndef OUTPUT_H
#define OUTPUT_H

#include "GlobalData.h"
#include "PID.h"
#include <Math.h>
#include "bhs_Launcher.h"

class Output
{
	
public:
	~Output();
	
	static void OutputToActuators();
	static void processDrivetrain();
	static void driveStraight( const double speed );
	static void turnDegrees( const double setpoint );
	static double angleDifference(double a, double b);
	
private:
	Output();
	static GlobalData* m_globals;
	
	static PID m_straightPID;
	static double m_currentStraightTime;
	static double m_previousStraightTime;
	static PID m_turnPID;
	static double m_currentTurnTime;
	static double m_previousTurnTime;
	
};

#endif
