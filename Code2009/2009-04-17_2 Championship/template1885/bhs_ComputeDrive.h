#ifndef _BHS_COMPUTEDRIVE_H_
#define _BHS_COMPUTEDRIVE_H_

/*
 //Method Name: ComputeDrive
 //
 //Description: This method computes the 4 motor powers, applies direction to the  
 //	       power values, and  saves the computed data to the robot global data
 //             class.
 // 
 //Parameters:
 //	apData: Pointer to where this method will save its results.
 //	aLeftMotorFrontDirection: Provides the directional value for the left front motor.	
 //                                 The values should be either 1 or -1.
 //	aLeftMotorBackDirection: Provides the directional value for the left rear motor.
 //                                The values should be  either 1 or -1.
 //	aRightMotorFrontDirection: Provides the directional value for the right front motor.
 * 								The values should be either 1 or -1.
 //	aRightMotorBackDirection: Provides the directional value for the right rear motor.
 * 							   The values should be either 1 or -1.
 */

#include "RobotGlobalData.h"
#include "Jaguar.h"

class bhs_ComputeDrive
{
  public:
    bhs_ComputeDrive();

    /* FUNCATION NAME:VirtualDriveMath
     * DESCRIPTION   :This function determines the virtual motor power base on
     *                joystick position for the drive mode the robot is in
     *                NOTE: Drive mode Skid steer and Vector drive
     * PARAMETERS    :
     *  apData  - Pointer to structure to store computation results
     *  anUserXaxis - Joystick X axis
     *  anUserYaxis - Joystick X axis
     * RETURN        :NA
     */
    void VirtualDriveMath(RobotGlobalData* apData, float anUserXaxis,
                          float anUserYaxis);
    /* FUNCATION NAME:
     * DESCRIPTION   :
     * PARAMETERS    :
     * RETURN        :
     */
    //This function maps which direction the motors should be facing forwards/sideway
    //base the mode the robot drive system is in
    void MapMotorDirections(RobotGlobalData* apData);

  private:
    /* FUNCATION NAME:SkidSteer
     * DESCRIPTION   :
     * PARAMETERS    :
     * RETURN        :
     */
    void SkidSteer(RobotGlobalData* apData, float anUserXaxis, float anUserYaxis);

    /* FUNCATION NAME:VectorDrive
     * DESCRIPTION   :
     * PARAMETERS    :
     * RETURN        :
     */
    void VectorDrive(RobotGlobalData* apData, float anUserXaxis, float anUserYaxis);

    /* FUNCATION NAME:automaticMotorConfiguration
     * DESCRIPTION   :
     * PARAMETERS    :
     * RETURN        :
     */
    void automaticMotorConfiguration(RobotGlobalData* apData);

    /* NAME       :teMotorLetter
     * DESCRIPTION:Enumeration describing which motor belongs in which array index
     */
    enum teMotorLetter
    { eeMOTOR_A, eeMOTOR_B, eeMOTOR_C, eeMOTOR_D, eenumMotors};

    /* NAME       :teDirections
     * DESCRIPTION:Enumeration describing which zone automatic motor twitching in
     *             the maAutomaticModeMotorConfiguration array.
     */
    enum teDirections
    { eeNORTH, eeNORTH_EAST, eeEAST,
      eeSOUTH_EAST, eeSOUTH, eeSOUTH_WEST,
      eeWEST, eeNORTH_WEST, eenumZones};

    /* NAME       :maAutomaticModeMotorConfiguration
     * DESCRIPTION:Array containing the motor configurations base on which zone
     *             the joystick is in for Automatic drive mode
     */
    bool maAutomaticModeMotorConfiguration[eenumZones][eenumMotors];

};

#endif
