#include <iostream.h>
#include "WPILib.h"
#include <taskLib.h>
#include "bhs_Actuators.h"
#include "PeriodicRobot.h"

#include "RobotGlobalData.h"
#include "InputFormat.h"
#include "bhs_ComputeDrive.h"
#include "MotorAssignment.h"
#include "MotorAcceleration.h"
#include "bhs_Camera.h"
#include "bhs_SensorPlatform.h"
#include "CameraTurret.h"
#include "TargetLocator.h"
#include "semLibCommon.h"
#include "bhs_HostLaptopData.h"
#include "bhs_TargetAnalysis.h"
#include "bhs_RotateRobot.h"
#include "bhs_RotateTurret.h"
#include "bhs_EdgeDetector.h"
//#include "bhs_DumperControl.h"

class IterativeDemo : public PeriodicRobot
{
    // Declare a variable to use to access the driver station object
    DriverStation *mpDS; // driver station object
    UINT32 mnPriorPacketNumber; // keep track of the most recent packet number from the DS
    UINT32 mnTelePeriodicLoops;
    UINT32 mnAutoPeriodicLoops;
    UINT32 mnDisabledPeriodicLoops;

    enum OldAutonomousMode
    { kOldAutoDriveForward, kOldAutoTurn};
    OldAutonomousMode meOldAutonomousState;
    
    enum AimAndShootAutonomousMode
    { kAASAutoWaitingForTargetInRange, kAASAutoDumping };
    AimAndShootAutonomousMode meAASAutonomousState;

    enum AutonomousType
    { kAutoOriginal, kAutoAimAndShoot};
    AutonomousType meAutonomousType;
    

    bhs_Actuators mcActuators;

    RobotGlobalData mcData;
    bhs_ComputeDrive mcDriveMath;
    InputFormat mcFormatOpInput;
    MotorAssignment mcConnectToMotors;
    //bhs_Camera mcCamera;

    /*CameraTurret mcCameraTurret;
     TargetLocator mcTargeting;*/

    bhs_SensorPlatform mcSensors;

    bhs_HostLaptopData mcLaptopData;

    //bhs_TargetAnalysis mcTargetAnalysis;

    bhs_RotateRobot mcRotateRobot;

    //bhs_DumperControl mcDumperControl;

  public:
    
    /**
     * Constructor for this "IterativeDemo" Class.
     * 
     * 
     * The constructor creates all of the objects used for the different inputs and outputs of
     * the robot.  Essentially, the constructor defines the input/output mapping for the robot,
     * providing named objects for each of the robot interfaces. 
     */
    IterativeDemo(void)
    {
      
      printf("IterativeDemo Constructor Started\n");
      
      // Acquire the Driver Station object
      mpDS = DriverStation::GetInstance();
      mnPriorPacketNumber = 0;
      mnTelePeriodicLoops = 0;
      mnAutoPeriodicLoops = 0;
      
      //printf("sizeof bool %d short %d int %d long %d float %d double %d\n")

      printf("IterativeDemo Constructor Completed\n");
    }
    
    /********************************** Init Routines *************************************/

    void RobotInit(void)
    {
      // Actions which would be performed once (and only once) upon initialization of the
      // robot would be put here.

      //Team 1885 mentor (Wei) has decided to not have the watchdog timer be active
      //Students are required to not to create infinite loops
      GetWatchdog().SetEnabled(false);
      
      mcFormatOpInput.Initialize(mcActuators.getPanServo(),
                                 mcActuators.getTiltServo());
      
      //mcCamera.initCamera(&mcData);
      //mcTargeting.Initialize(&mcData, &mcCameraTurret, mcRobotTimer2);

      mcSensors.Initialize();
      
      /*mcTargetAnalysis.configureCamera(&mcCamera, &mcData, mpDS);
      mcTargetAnalysis.initTargetAnalysis(mcRobotTimer2);*/
      
      mcRotateRobot.setGains(bhs_Constants::cnROTATE_ROBOT_P, bhs_Constants::cnROTATE_ROBOT_I,
                             bhs_Constants::cnROTATE_ROBOT_D);
    }
    
    void DisabledInit(void)
    {
      mnDisabledPeriodicLoops = 0;
    }
    
    void AutonomousInit(void)
    {
      mcData.meDriveStatus = RobotGlobalData::kMotorsForward;
      mnAutoPeriodicLoops = 0;
      meOldAutonomousState = kOldAutoDriveForward;
      
      //Determine which autonomous mode to use based on the driver station digital inputs
      mcData.mbAutonomousSwitchState = mpDS->GetDigitalIn(bhs_Constants::cnDS_AUTONOMOUS_SWITCH_1);
      if (mcData.mbAutonomousSwitchState == bhs_Constants::cbUSE_OLD_AUTONOMOUS)
      {
        meAutonomousType = kAutoOriginal;
      }
      else
      {
        meAutonomousType = kAutoAimAndShoot;
      }
      meAutonomousType = kAutoOriginal;
    }
    
    void TeleopInit(void)
    {
      mcData.meDriveStatus = RobotGlobalData::kMotorsForward;
      mcActuators.resetAcceleration();
    }
    
    /********************************** Periodic Routines *************************************/

    void RobotComputations(void)
    {
      //computes the power for the front left right back motors and store in
      //global data
      mcDriveMath.VirtualDriveMath(&mcData, mcData.mnUserXaxis, mcData.mnUserYaxis);
      
      //This function determines which way the motors should be facing base on the 
      //robot drive system mode
      mcDriveMath.MapMotorDirections(&mcData);
      
      //take values computed my ComputeDrive and assign them to the
      //actual physical motor depending on state
      mcConnectToMotors.assignToPhysicalMotor(&mcData);
      
      //Ensure that the physical motor is spining in the right direction
      //for the mode
      mcConnectToMotors.setMotorDirections(&mcData);
      
      //mcDumperControl.controlDumper(&mcData);
    }
    
    void DriverStationFeedback(void)
    {
      mcLaptopData.setAllData(&mcData, mpDS);
      
      //***********THIS IS A REQUIRED HACK*************
      //We are not using Dashboard because it packs the data in an awkward manner, making
      //it impossible to translate back in our dashboard without killing ourselves.
      //If the compiler complains, that's because the DriverStation.h file was modified
      //to make IterativeDemo a friend class.
      memcpy(mpDS->m_userStatus, (char*)&mcLaptopData, sizeof(mcLaptopData));
    }
    
    void DisabledPeriodic(void)
    {
      mnDisabledPeriodicLoops++;
      if (mnDisabledPeriodicLoops % 4 == 0)
      {
        DriverStationFeedback();
        mnDisabledPeriodicLoops = 0;
      }
    }
    
    void AutonomousPeriodic(void)
    {
      mcFormatOpInput.handleSensorInputs(&mcData, &mcSensors);
      
      mnAutoPeriodicLoops++;
      
      //Run the autonomous mode determined in AutonomousInit
      switch (meAutonomousType)
      {
        case kAutoOriginal:
          OldAutonomousMode();
          break;
        
        case kAutoAimAndShoot:
          AutonomousAimAndShoot();
          break;
      }
      
      RobotComputations();
      
      /*
       * NOTE:  Anything placed here will be called on each iteration of the periodic loop.
       * Since the default speed of the loop is 200 Hz, code should only really be placed here
       * for I/O that can respond at a 200Hz rate.  (e.g. the Jaguar speed controllers
       */

      // put 200Hz Jaguar control here

      mcActuators.controlDriveTrain(&mcData);
      mcActuators.controlManipulator(&mcData);
      mcActuators.controlPumpAndTwitch(&mcData);
      
      if ((mnAutoPeriodicLoops % 2) == 0)
      {
        /* 
         * Code placed in here will be called on every alternate iteration of the periodic loop.
         * Assuming the default 200 Hz loop speed, code should only really be placed here for 
         * I/O that can respond at a 100Hz rate.  (e.g. the Victor speed controllers)
         */

        // put 100Hz Victor control here
      }
      
      if ((mnAutoPeriodicLoops % 4) == 0)
      {
        /* 
         * Code placed in here will be called on every fourth iteration of the periodic loop.
         * Assuming the default 200 Hz loop speed, code should only really be placed here for 
         * I/O that can respond at a 50Hz rate.  (e.g. the Hitec HS-322HD servos)
         */

        // put 50Hz Servo control here

        mcActuators.controlCameraServos(&mcData);
        
        DriverStationFeedback();
      }
    }
    
    void OldAutonomousMode(void)
    {
      switch (meOldAutonomousState)
      {
        case kOldAutoDriveForward:
          mcData.meDriveStatus = RobotGlobalData::kMotorsForward;
          mcData.mnUserXaxis = 0;
          mcData.mnUserYaxis = 1;
          if (mnAutoPeriodicLoops>=500)
          {
            meOldAutonomousState = kOldAutoTurn;
          }
          break;
        case kOldAutoTurn:
          mcData.meDriveStatus = RobotGlobalData::kMotorsForward;
          mcData.mnUserXaxis = .5;
          mcData.mnUserYaxis = 1;
          break;
        default:
          meOldAutonomousState = kOldAutoDriveForward;
      }
    }
    
    void AutonomousAimAndShoot(void)
    {
      //Drive forward
      mcData.meDriveStatus = RobotGlobalData::kMotorsForward;
      mcData.mnUserYaxis = 1;
      
      
      //If a target is seen, use a PID to rotate the robot toward it
      bhs_TargetingData lcTargetingData;
      mcData.getTargetingData(lcTargetingData);
      
      if(lcTargetingData.mnCurrentTargetAngle != -1)
      {
        float lnCenteredTargetAngle = lcTargetingData.mnCurrentTargetAngle + (mcData.mnGyroValue - 90);
        mcData.mnUserXaxis = mcRotateRobot.rotateToAngle(mcData.mnGyroValue, lnCenteredTargetAngle).mnXaxis;
      }
      else
      {
        mcData.mnUserXaxis = 0;
      }
      
      
      switch(meAASAutonomousState)
      {
        case kAASAutoWaitingForTargetInRange:
          //If the target is close and within ten degrees from the front, then dump the balls.
          if(lcTargetingData.mnCurrentTargetDistance <= bhs_Constants::cnAUTO_MINIMUM_DISTANCE &&
             lcTargetingData.mnCurrentTargetAngle >= bhs_Constants::cnAUTO_LOWER_ANGLE_RANGE &&
             lcTargetingData.mnCurrentTargetAngle <= bhs_Constants::cnAUTO_UPPER_ANGLE_RANGE)
          {
            meAASAutonomousState = kAASAutoDumping;
          }
          
        case kAASAutoDumping:
          //If the target is still in the field of view and if the current closest opposing target is not
          //obviously a different target based on the distance difference, keep updating the shooting
          //distance with the target data.
          //Assume that if the target disappeared, it is just too close for the camera to see and keep
          //dumping.
          if(lcTargetingData.mnCurrentTargetDistance != -1 &&
             lcTargetingData.mnCurrentTargetDistance <
               mcData.mnDesiredShootingDistance + bhs_Constants::cnAUTO_MINIMUM_DISTANCE_DIFFERENCE)
          {
            mcData.mnDesiredShootingDistance = lcTargetingData.mnCurrentTargetDistance;
          }
          //Dump the balls.
          //mcDumperControl.controlDumper(&mcData);
      }
    }
    
    
    
    
    void TeleopPeriodic(void)
    {
      static int lsTempPeriodicCnter = 0;
      // increment the number of teleop periodic loops completed
      mnTelePeriodicLoops++;
      
      //DRIVE TRAIN GOES IN THIS IF STATEMENT
      UINT32 lnCurrentPacketNumber = mpDS->GetPacketNumber();
      if (lnCurrentPacketNumber!= mnPriorPacketNumber)
      {
        
        /* 
         * Code placed in here will be called only when a new packet of information
         * has been received by the Driver Station.  Any code which needs new information
         * from the DS should go in here
         */

        mnPriorPacketNumber = lnCurrentPacketNumber;
        
        //Takes care of all operator input functions except drive train.
        //Also calculates some math on the X and Y axes of the joystick and puts the results
        //in the given RobotGlobalData object
        mcFormatOpInput.handleOperatorInputs(&mcData, mpDS, &mcSensors);
        
        RobotComputations();
        
        lsTempPeriodicCnter++;
        
      } // if (mpDS->GetPacketNumber()...

      /*
       * NOTE:  Anything placed here will be called on each iteration of the periodic loop.
       * Since the default speed of the loop is 200 Hz, code should only really be placed here
       * for I/O that can respond at a 200Hz rate.  (e.g. the Jaguar speed controllers
       */

      // put 200Hz Jaguar control here

      if (lsTempPeriodicCnter >= 50)
      {
        /*printf("rawX %f rawY %f calcX %f calcY %f\nA %f B %f C %f D %f\n",
         mpDS->GetStickAxis(1, 1), mpDS->GetStickAxis(1, 2),
         mcData.mnUserXaxis, mcData.mnUserYaxis,
         mcData.mnMotorAPower, mcData.mnMotorBPower,
         mcData.mnMotorCPower, mcData.mnMotorDPower);*/
      }
      
      mcActuators.controlDriveTrain(&mcData);
      mcActuators.controlManipulator(&mcData);
      mcActuators.controlPumpAndTwitch(&mcData);
      
      if ((mnTelePeriodicLoops % 2) == 0)
      {
        /* 
         * Code placed in here will be called on every alternate iteration of the periodic loop.
         * Assuming the default 200 Hz loop speed, code should only really be placed here for 
         * I/O that can respond at a 100Hz rate.  (e.g. the Victor speed controllers)
         */

        // put 100Hz Victor control here
      }
      
      if ((mnTelePeriodicLoops % 4) == 0)
      {
        /* 
         * Code placed in here will be called on every fourth iteration of the periodic loop.
         * Assuming the default 200 Hz loop speed, code should only really be placed here for 
         * I/O that can respond at a 50Hz rate.  (e.g. the Hitec HS-322HD servos)
         */

        // put 50Hz Servo control here

        mcActuators.controlCameraServos(&mcData);
        
        DriverStationFeedback();
      }
      
      if (lsTempPeriodicCnter >= 50)
      {
        /*printf("Target Angle: %f Target Distance: %f\n",
         mcData.mnUserDesiredShootingAngle,
         mcData.mnUserDesiredShootingDistance);*/
        //printf("Joystick angle %f\n", mcData.mnUserAngle);
        /*printf("Accelerated A %f B %f C %f D %f\n",
         mcData.mnMotorAPower, mcData.mnMotorBPower,
         mcData.mnMotorCPower, mcData.mnMotorDPower);*/
        lsTempPeriodicCnter=0;
      }
      
    } // TeleopPeriodic(void)
};

START_ROBOT_CLASS(IterativeDemo)
;
