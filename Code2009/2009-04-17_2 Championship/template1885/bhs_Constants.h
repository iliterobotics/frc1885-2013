/*
 * bhs_Constants.h
 *
 *  Created on: Jan 6, 2009
 *      Author: ITLab
 */

/*
 * Description:
 */
#ifndef __BHS_CONSTANTS__
#define __BHS_CONSTANTS__

#include "Relay.h"

class bhs_Constants
{
  public:
    
    
    /** Which team is the target on? */
    //enum TeamColor { kBLUE, kRED, kUNKNOWN };
    
    static const double PI = 3.14159265358979323;
    static const double G_FORCE_TO_ACCELERATION = 9.80665;
    
    //buttons on joystick one
    static const short TWITCH_FORWARD_BUTTON = 0x0001;             //1
    static const short TWITCH_SIDEWAYS_BUTTON = 0x0002;            //2
    static const short TWITCH_VECTOR_HYBRID_FRONT_BUTTON = 0x0004; //3
    static const short TWITCH_VECTOR_HYBRID_BACK_BUTTON = 0x0008;  //4
    static const short TWITCH_VECTOR_HYBRID_RIGHT_BUTTON = 0x0010; //5
    static const short TWITCH_VECTOR_HYBRID_LEFT_BUTTON = 0x0020;  //6
    /*static const short CONVEYOR_FORWARD_BUTTON = 0x0040;           //7
    static const short CONVEYOR_REVERSE_BUTTON = 0x0100;           //9
    static const short CONVEYOR_OFF_BUTTON = 0x0200;               //10*/
    static const short CELL_FEEDER_FORWARD_BUTTON = 0x0040;        //7
    static const short CELL_FEEDER_REVERSE_BUTTON = 0x0080;        //8
    static const short CELL_FEEDER_OFF_BUTTON = 0x0100;            //9
    static const short TWITCH_AUTO_BUTTON = 0x0400;                //11
    static const short TWITCH_VECTOR_BUTTON = 0x0800;              //12
    
    //static const short COMPRESSOR_BUTTON = 0x0040;
    
    
    /**********buttons on joystick two**********/
    /*static const short TURRET_MANUAL_AIM_BUTTON = 0x0001;  //1
    static const short TURRET_AUTO_AIM_BUTTON = 0x0000;    //not being used*/
    //static const short DUMPER_BACKWARDS_BUTTON = 0x0002;  //2
    /*static const short CAMERA_ZONE_1_BUTTON = 0x0008;      //4
    static const short CAMERA_ZONE_2_BUTTON = 0x0004;      //3
    static const short CAMERA_ZONE_3_BUTTON = 0x0010;      //5*/
    
    static const short CELL_LOADER_TAKE_IN_BUTTON = 0x0008;  //4
    static const short CELL_LOADER_PUSH_OUT_BUTTON = 0x0010;  //5
    static const short CELL_LOADER_OFF_BUTTON = 0x0004;      //3
    static const short CELL_HOLDER_ON_BUTTON = 0x0100;       //9
    static const short CELL_HOLDER_OFF_BUTTON = 0x0080;      //8
    
    //static const short TURRET_LOCK_BUTTON = 0x0400;        //11
    
    //buttons on joystick four for debugging
    //static const short DEBUG_TURRET_BUTTONS  = 0x0180;  //press buttons 8 and 9
    static const short DEBUG_DUMPER_BUTTONS = 0x0420;  //press buttons 6 and 11
    
    //**********Dumper Constants**********
    /*static const float cnDUMPER_DIST_MULTIPLIER = 2;
    static const float cnDUMPER_ANGLE_MULTIPLIER = 1.5;
    static const float cnDUMPER_ANGLE_MAX = 122.5;
    static const float cnDUMPER_ANGLE_MIN = 67;
    static const float cnDUMPER_DIST_MAX = 75;
    static const float cnDUMPER_DIST_MIN = 0;*/
    
    static const float cnCELL_LOADER_TAKE_IN_POWER = 0.7;
    static const float cnCELL_LOADER_PUSH_OUT_POWER = -0.7;
    static const float cnCELL_LOADER_OFF_POWER = 0;
    static const int cnCELL_LOADER_MOTOR_1_DIRECTION = -1;
    static const int cnCELL_LOADER_MOTOR_2_DIRECTION = 1;
    //static const float cnDUMPER_BACKWARDS_POWER = -0.15;
    
    /*static const Relay::Value cnCONVEYOR_FORWARD_VALUE = Relay::kForward;
    static const Relay::Value cnCONVEYOR_REVERSE_VALUE = Relay::kReverse;
    static const Relay::Value cnCONVEYOR_OFF_VALUE = Relay::kOff;
    
    static const Relay::Value cnCONVEYOR_2_FORWARD_VALUE = Relay::kReverse;
    static const Relay::Value cnCONVEYOR_2_REVERSE_VALUE = Relay::kForward;
    static const Relay::Value cnCONVEYOR_2_OFF_VALUE = Relay::kOff;*/
    
    static const Relay::Value cnCELL_FEEDER_FORWARD_VALUE = Relay::kForward;
    static const Relay::Value cnCELL_FEEDER_REVERSE_VALUE = Relay::kReverse;
    static const Relay::Value cnCELL_FEEDER_OFF_VALUE = Relay::kOff;
    
    static const Relay::Value cnCELL_HOLDER_ON_VALUE = Relay::kForward;
    static const Relay::Value cnCELL_HOLDER_OFF_VALUE = Relay::kOff;
    
    
    //*********Actuators*********
    static const UINT32 cnDRIVE_TRAIN_MOTOR_A_CHANNEL = 1;
    static const UINT32 cnDRIVE_TRAIN_MOTOR_B_CHANNEL = 2;
    static const UINT32 cnDRIVE_TRAIN_MOTOR_C_CHANNEL = 3;
    static const UINT32 cnDRIVE_TRAIN_MOTOR_D_CHANNEL = 4;
    //static const UINT32 cnTURRET_MOTOR_CHANNEL = 7;
    static const UINT32 cnEMPTY_CELL_LOADER_MOTOR_1_CHANNEL = 5;
    static const UINT32 cnEMPTY_CELL_LOADER_MOTOR_2_CHANNEL = 6;
    static const UINT32 cnPAN_SERVO_CHANNEL = 9;
    static const UINT32 cnTILT_SERVO_CHANNEL = 10;
    
    static const UINT32 cnCOMPRESSOR_RELAY_CHANNEL = 1;
    /*static const UINT32 cnCONVEYOR_1_RELAY_CHANNEL = 2;
    static const UINT32 cnCONVEYOR_2_RELAY_CHANNEL = 4;*/
    
    static const UINT32 cnCELL_FEEDER_RELAY_CHANNEL = 2;
    static const UINT32 cnCELL_HOLDER_RELAY_CHANNEL = 5;
    
    
    //********Turret Constants*********
    /*static const float cnTURRET_MOTOR_MAXIMUM_POWER = 1.0;
    static const float cnTURRET_MOTOR_MINIMUM_POWER = 0.13;
    static const float cnTURRET_ROTATION_P = 0.050068;
    static const float cnTURRET_ROTATION_I = 0;
    static const float cnTURRET_ROTATION_D = -0.000049;*/
    
    
    //********Drive Train Constants*********
    static const float TWITCH_DEAD_ZONE = .1;
    static const float cnROTATE_ROBOT_P = 0.011068;
    static const float cnROTATE_ROBOT_I = 0;
    static const float cnROTATE_ROBOT_D = -0.000629;
    
    
    //*********Camera Constants*******
    static const float cnNUM_DEGREES_VERT = 40.5;
    static const float cnNUM_DEGREES_HORIZ = 54;
    static const float cnHEIGHT_OF_TRAILER = 77;
    static const float cnHEIGHT_OF_CAMERA = 56/*= the height of the camera from the ground*/;
    static const int cnMAX_TARGET_REPORTS = 15;
    static const float cnFIXED_TILT_ANGLE = 95;
    static const float cnACTUAL_FIXED_TILT_ANGLE = 0;
    static const float cnPAN_ANGLES[3];
    static const float cnACTUAL_ANGLES[3];
    
    
    //*********Digital sensors********
    static const UINT32 cnDIGITAL_AIR_PRESSURE_IN = 1;
    /*static const UINT32 cnDIGITAL_TURRET_UPPER_LIMIT = 10;
    static const UINT32 cnDIGITAL_TURRET_LOWER_LIMIT = 11;*/
    
    static const UINT32 cnDIGITAL_LIMIT_CLOSED = 0;
    static const UINT32 cnDIGITAL_LIMIT_OPEN = 1;
    
    //****Driver Station digital in****
    static const short cnDS_AUTONOMOUS_SWITCH_1 = 1;
    
    
    //*********Analog sensors*********
    
    static const float cnAccelerometerCalibrationSampleTime = 5.0;
    
    //number of times that the Sensor Platform class runs every second (Hertz)
    static const int cnSENSOR_PLATFORM_EXE_RATE=49;
    static const int cnACCELEROMETER_CALIBRATE_CYCLES=200;
    static const int cnACCELEROMETER_CALIBRATE_IGNORE=3;
    
    static const int cnACCELEROMETER_X_CHANNEL=2; 
    static const int cnACCELEROMETER_Y_CHANNEL=3;
    static const int cnGYRO_CHANNEL=1;
    static const float cnACCELEROMETER_X_SENSITIVITY=1.000;
    static const float cnACCELEROMETER_Y_SENSITIVITY=1.000;
    static const float cnGYRO_SENSITIVITY=0.0125;
    
    static const int cnTURRET_POTENTIOMETER_CHANNEL = 4;
    
    //***********Autonomous***********
    static const bool cbUSE_OLD_AUTONOMOUS = false;
    static const float cnAUTO_UPPER_ANGLE_RANGE = 95;
    static const float cnAUTO_LOWER_ANGLE_RANGE = 85;
    static const float cnAUTO_MINIMUM_DISTANCE = 48;
    static const float cnAUTO_MINIMUM_DISTANCE_DIFFERENCE = 18;
    static const float cnAUTO_DUMPER_SPEED_BY_DISTANCE = 30;
};

#endif
