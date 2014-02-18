#ifndef BHS_ROTATETURRET_H_
#define BHS_ROTATETURRET_H_

#include "bhs_PID.h"
#include "RobotGlobalData.h"
#include "bhs_Constants.h"

struct bhs_RotateTurretData
{
  float mnPower;
  bool  mbDone;
};

class bhs_RotateTurret
{
  public:
    bhs_RotateTurret();
    ~bhs_RotateTurret();
    
    void configCount(int anCnt);

    /* FUNCTION NAME: setGains
     * DESCRIPTION  : configures P,I and D gains needed to rotate the 2009 
     * PARAMETERS :
     *  anProportional  - Proportional Gain
     *  anIntegral    - Integral Gain
     *  anDerivative  - Derivative Gain
     * RETURN   : NA
     */
    void setGains(double anProportional, double anIntegral, double anDerivative);

    /* FUNCTION NAME: rotateToAngle
     * DESCRIPTION  : configures P,I and D gains needed to rotate the 2009 
     * PARAMETERS :
     *  anCurrentAngle  - Current angle of the Robot
     *  anDesireAngle   - The angle the Robot should go to
     * RETURN   : indicates if the robot is done rotating and what the X axis
     *            needs to be for achieving the desire angle
     */
    bhs_RotateTurretData rotateToAngle(double anCurrentAngle, double anDesireAngle);

  private:
    bhs_PID mcRotatePid;
    int     mcCountAtDegree;
    int     mcNumCountsForRotationFinish;
};

#endif
