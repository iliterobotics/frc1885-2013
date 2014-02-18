/*
 * CameraTurret.h
 *
 *  Created on: Jan 22, 2009
 *      Author: ITRobo
 */

/*
 * Description:
 */
#ifndef _CAMERATURRET_H_
#define _CAMERATURRET_H_

#include "RobotGlobalData.h"

class CameraTurret
{
  public:
    CameraTurret();
    ~CameraTurret();
    
    void setTiltAngle(RobotGlobalData* apData, float anCameraTiltInput);
    
    void setPanAngle(RobotGlobalData* apData, float anCameraPanInput);
  
  private:
    static const float TiltOffSet =0;
    static const float PanOffSet =0;
    
};
#endif
