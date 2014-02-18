#ifndef BHS_ROBOT_H_
#define BHS_ROBOT_H_

#include "DriverStationLCD.h"
#include "Watchdog.h"

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"

#include "bhs_Input.h"
#include "bhs_SensorInput.h"
#include "bhs_Autonomous.h"
#include "bhs_DriverInput.h"

#include "bhs_Subsystem.h"
#include "bhs_Drivetrain.h"
#include "bhs_DriveAngle.h"
#include "bhs_Nudge.h"
#include "bhs_Subsys1.h"
#include "bhs_Subsys2.h"
#include "bhs_Subsys3.h"

#include "bhs_OutputManager.h"

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

		bhs_GlobalData m_gd;

		bhs_Input* m_sensorInput;
		bhs_Input* m_autonomousInput;
		bhs_Input* m_driverInput;
		
		bhs_DriveTrain* m_drivetrain;
		bhs_DriveAngle* m_driveAngle;
		bhs_Nudge* m_nudge;
		
		bhs_Subsystem* m_subsys1;
		bhs_Subsystem* m_subsys2;
		bhs_Subsystem* m_subsys3;

		bhs_OutputManager m_outputManager;
};

#endif // BHS_ROBOT_H_
