/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef TARGETPOTENTIOMETER_H_
#define TARGETPOTENTIOMETER_H_

#include "SensorBase.h"
#include "PIDSource.h"

class AnalogChannel;
class AnalogModule;

struct POTAngles
{
  static const int numAnglesSpecified = 11;
  static const float maAngles[numAnglesSpecified];
  static const float maVoltage[numAnglesSpecified];
};

class TargetPotentiometer : public SensorBase, public PIDSource
{
  public:
    static const UINT32 kOversampleBits = 1;
    static const UINT32 kAverageBits = 5;
    static const float kSamplesPerSecond = 50.0;

    TargetPotentiometer(UINT32 slot, UINT32 channel);
    explicit TargetPotentiometer(UINT32 channel);
    explicit TargetPotentiometer(AnalogChannel *channel);
    explicit TargetPotentiometer(AnalogChannel &channel);
    virtual ~TargetPotentiometer();
    float GetAngle();
    float GetRaw();
    
    double PIDGet();

  private:
    void InitPOT();
    
    AnalogChannel *m_analog;
    bool m_channelAllocated;
};
#endif
