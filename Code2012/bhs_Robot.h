#ifndef BHS_ROBOT_H
#define BHS_ROBOT_H

#include "DriverStationLCD.h"
#include "Watchdog.h"

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"

#include "bhs_SensorInput.h"
#include "bhs_JoystickInput.h"
#include "bhs_AxisCamera.h"
#include "bhs_AxisCamera2.h"
#include "bhs_Autonomous.h"
#include "bhs_AutonomousBasicWithBridge.h"

#include "bhs_DriveTrain.h"
#include "bhs_DriveAngle.h"
#include "bhs_Conveyor.h"
#include "bhs_Shooter.h"
#include "bhs_BridgeLowering.h"
#include "bhs_Nudge.h"

#include "bhs_OutputManager.h"
#include "bhs_Diagnostics.h"
#include "bhs_PCDataServer.h"

class bhs_Robot {
public:
	bhs_Robot();
	~bhs_Robot();

	void disabledInit();
	void autonomousInit();
	void autonomous();
	void teleopInit();
	void teleop();

private:
	void normalRobotProcessing();

	bhs_GlobalData m_globalData;

	bhs_SensorInput m_sensorInput;
	bhs_JoystickInput m_joystickInput;
#if USE_HOOP_CAMERA
	bhs_AxisCamera* m_axisCamera;
#endif
#if USE_GROUND_CAMERA
	bhs_AxisCamera2* m_axisCamera2;
#endif
	bhs_Autonomous* m_autonomousInput;
	
	bhs_DriveTrain m_driveTrain;
	bhs_DriveAngle m_driveAngle;
	bhs_Conveyor m_conveyor;
	bhs_Shooter m_shooter;
	bhs_BridgeLowering m_bridgeLowering;
	bhs_Nudge m_nudge;
	
	bhs_OutputManager m_outputManager;
	bhs_Diagnostics m_diagnostics;
	bhs_PCDataServer m_dataServer;

};

#endif // BHS_ROBOT_H
