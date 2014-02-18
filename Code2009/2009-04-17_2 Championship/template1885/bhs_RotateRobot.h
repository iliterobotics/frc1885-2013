#ifndef BHS_ROTATEROBOT_H_
#define BHS_ROTATEROBOT_H_

#include "bhs_PID.h"

struct bhs_RotateData
{
  float mnXaxis;
  bool  mbDone;
};

class bhs_RotateRobot
{
  public:
    bhs_RotateRobot();
    ~bhs_RotateRobot();
    
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
    bhs_RotateData rotateToAngle(double anCurrentAngle, double anDesireAngle);

  private:
    bhs_PID mcRotatePid;
    int     mcCountAtDegree;
    int     mcNumCountsForRotationFinish;
};

#endif
