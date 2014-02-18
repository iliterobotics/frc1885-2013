#include "PeriodicRobot.h"
#include "WPILib.h"
#include "Task.h"
#include "Timer.h"
#include "taskLibCommon.h"
#include "signal.h"
#include "sysLib.h"

static int PeriodicRobotTask1885(int aComm, int aComm2, int aComm3, int aTaskDelay) 
{
	SEM_ID* lpComSem = (SEM_ID*)aComm;    //Main loop
	SEM_ID* lpComSem2 = (SEM_ID*)aComm2;  //Camera tracking loop
	int cameraTrackCount = 0;
	SEM_ID* lpComSem3 = (SEM_ID*)aComm3;  //Accelerometer and gyro loop
	int analogGetCount = 0;
	while(true)
	{
		taskDelay(aTaskDelay);
		cameraTrackCount++;
		analogGetCount++;
		//Main loop every 5ms
		semGive(*lpComSem);
		//Camera tracking every 20ms
		if(cameraTrackCount % 4 == 0)
		{
		  semGive(*lpComSem2);
		  cameraTrackCount = 0;
		}
		//Get analog data every 20ms
		if(analogGetCount % 4 == 0)
		{
		  semGive(*lpComSem3);
		  analogGetCount = 0;
		}
	}
	return 0;
}

PeriodicRobot::PeriodicRobot()
: RobotBase(),
mbDisabledInitialized(false),
mbAutonomousInitialized(false),
mbTeleopInitialized(false)
{
	//create semaphore to communicate between timer task and "StartCompetition" Loop
	mcRobotTimer = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	mcRobotTimer2 = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	mcRobotTimer3 = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	
	//computing how many ticks to delay to achieve a 200Hz period
	//200Hz = 5 millisecond period
	int lnPeriodicDelay = sysClkRateGet()/200;
	mnPeriodicTaskId = taskSpawn("Robot Periodic Timer",
			  99,						//task pri.
			  VX_FP_TASK,				// options
			  64000,					// stack size
			  (FUNCPTR)PeriodicRobotTask1885,	// function to start
			  (int)&mcRobotTimer,				// parameter 1 - semaphore for controlling loop ex.
			  (int)&mcRobotTimer2,				// semaphore for controlling another loop
			  (int)&mcRobotTimer3,				// semaphore for controlling another loop
			  lnPeriodicDelay,			// ticks to delay before freeing semaphore
			   0, 0, 0, 0, 0, 0);
}

PeriodicRobot::~PeriodicRobot()
{
	//taskKill(mnPeriodicTaskId);
	semFlush(mcRobotTimer);
	semFlush(mcRobotTimer2);
	semFlush(mcRobotTimer3);
}

/**
 * Provide an alternate "main loop" via StartCompetition().
 * 
 * This specific StartCompetition() implements "main loop" behavior like that of the FRC
 * control system in 2008 and earlier, with a primary (slow) loop that is
 * called periodically, and a "fast loop" (a.k.a. "spin loop") that is 
 * called as fast as possible with no delay between calls. 
 */
void PeriodicRobot::StartCompetition(void)
{
	int lnLoopCounter = 0;
	printf("RobotPeriodicBase StartCompetition() Commenced\n");
	
	// first and one-time initialization
	RobotInit();
	
	// loop forever, calling the appropriate mode-dependent function
	while (TRUE)
	{
		//wait on the timer task to tell us it is ok to go
		semTake(mcRobotTimer, WAIT_FOREVER);  //Prevents this function from hogging all CPU cycles
		
		if(lnLoopCounter == 0)
		{
			//printf("Loop executing\n");
		}
		else if(lnLoopCounter >=400){
			lnLoopCounter = -1;
		}
		lnLoopCounter++;
		
		// Call the appropriate function depending upon the current robot mode
		if (IsDisabled())
		{
			// call DisabledInit() if we are now just entering disabled mode from
			// either a different mode or from power-on
			if(!mbDisabledInitialized)
			{
				DisabledInit();
				mbDisabledInitialized = true;
				// reset the initialization flags for the other modes
				mbAutonomousInitialized = false;
				mbTeleopInitialized = false;
				printf("Disabled_Init() completed\n");
			}
			DisabledPeriodic();
			
		}
		else if (IsAutonomous())
		{
			// call AutonomousInit() if we are now just entering autonomous mode from
			// either a different mode or from power-on
			if(!mbAutonomousInitialized)
			{
				// KBS NOTE:  old code reset all PWMs and relays to "safe values"
				// whenever entering autonomous mode, before calling
				// "Autonomous_Init()"
				AutonomousInit();
				mbAutonomousInitialized = true;
				// reset the initialization flags for the other modes
				mbDisabledInitialized = false;
				mbTeleopInitialized = false;
				printf("Autonomous_Init() completed\n");
			}
			AutonomousPeriodic();

		}
		else
		{
			// call TeleopInit() if we are now just entering teleop mode from
			// either a different mode or from power-on
			if(!mbTeleopInitialized)
			{
				TeleopInit();
				mbTeleopInitialized = true;
				// reset the initialization flags for the other modes
				mbDisabledInitialized = false;
				mbAutonomousInitialized = false;
				printf("Teleop_Init() completed\n");
			}
			TeleopPeriodic();
			
		}
	}	
	printf("RobotIterativeBase StartCompetition() Ended\n");
}	
void PeriodicRobot::RobotInit(void)
{
}
	
void PeriodicRobot::DisabledInit(void)
{
}
void PeriodicRobot::AutonomousInit(void)
{
}
void PeriodicRobot::TeleopInit(void)
{
}
void PeriodicRobot::DisabledPeriodic(void)
{
}
void PeriodicRobot::AutonomousPeriodic(void)
{
}
void PeriodicRobot::TeleopPeriodic(void)
{
}
