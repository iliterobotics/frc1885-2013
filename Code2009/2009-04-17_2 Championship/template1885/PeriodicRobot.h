#include "RobotBase.h"

#ifndef PERIODICROBOT_H_
#define PERIODICROBOT_H_

class PeriodicRobot : public RobotBase
{
public:
	PeriodicRobot();
	virtual ~PeriodicRobot();
	
	virtual void StartCompetition(void);
	
	virtual void RobotInit(void);
	
	virtual void DisabledInit(void);
	virtual void AutonomousInit(void);
	virtual void TeleopInit(void);

	virtual void DisabledPeriodic(void);
	virtual void AutonomousPeriodic(void);
	virtual void TeleopPeriodic(void);



protected:
	SEM_ID mcRobotTimer;
	SEM_ID mcRobotTimer2;
	SEM_ID mcRobotTimer3;
	
private:
	INT32 mnPeriodicTaskId;
	
	bool mbDisabledInitialized;
	bool mbAutonomousInitialized;
	bool mbTeleopInitialized;
};

#endif /*PERIODICROBOT_H_*/
