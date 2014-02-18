#ifndef __BHS_LAUNCHER__H__
#define __BHS_LAUNCHER__H__

#include <Math.h>
#include "AnalogChannel.h"
#include "Talon.h"
#include "Solenoid.h"
#include "GlobalData.h"
#include "PID.h"
#include "TimedBoolean.h"

class GlobalData;
class PID;

class bhs_Launcher
{
public:
	enum Angle
	{
		AngleDefault = 0,
		AnglePreset1,
		AnglePreset2
	}; //FIXME FIXMEFIXMEFIXME

	bhs_Launcher();
	~bhs_Launcher();

	void moveToAngle( double setpoint, double current );
	void moveToTiltPotValue( double setpoint);
	Angle getDefaultAngle();
	double getAngle();
	bool isAtDesiredAngle();
	
	void moveToSpeed( double speed );
	double getSpeed();
	bool isAtDesiredSpeed();
	
	void launch( bool doLaunch );
	void forceStopLaunch();
	bool getKickerState();
	
	void updateSelf();

	void kickerIn();
	void kickerOut();
	
private:
	GlobalData* m_globals;
	//TimedBoolean m_kickerState;
	
	bool m_kickerState;
	Timer* m_timer;
	
	PID* m_tiltPID;
	
	void moveToVoltage( double voltage );
};

#endif // __BHS_LAUNCHER__H__
