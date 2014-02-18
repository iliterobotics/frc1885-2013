/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "TargetPotentiometer.h"
#include "AnalogChannel.h"
#include "AnalogModule.h"
#include "Timer.h"
#include "Utility.h"
#include "WPIStatus.h"
#include <math.h>

const float POTAngles::maAngles[] =  
{ 67, 71.5, 72.5, 75, 77.5, 84, 90, 92.5, 100, 110, 122.5 };

const float POTAngles::maVoltage[] =  
{ 0, 84, 151, 285, 447, 601,
  745, 828, 1024, 1284, 1546 };


/**
 * Initialize the TargetPotentiometer.
 * Sets the sample rate for the analog channel.  The TargetPotentiometer cannot be calibrated
 * automatically, so the zero degree voltage must be set manually.
 */
void TargetPotentiometer::InitPOT()
{
  m_analog->SetAverageBits(kAverageBits);
  m_analog->SetOversampleBits(kOversampleBits);
  float sampleRate = kSamplesPerSecond * (1 << (kAverageBits + kOversampleBits));
  m_analog->GetModule()->SetSampleRate(sampleRate);
  Wait(1.0);
}

/**
 * TargetPotentiometer constructor given a slot and a channel.
 * 
 * @param slot The cRIO slot for the analog module the TargetPotentiometer is connected to.
 * @param channel The analog channel the TargetPotentiometer is connected to.
 */
TargetPotentiometer::TargetPotentiometer(UINT32 slot, UINT32 channel)
{
  m_analog = new AnalogChannel(slot, channel);
  m_channelAllocated = true;
  InitPOT();
}

/**
 * TargetPotentiometer constructor with only a channel.
 * 
 * Use the default analog module slot.
 * 
 * @param channel The analog channel the TargetPotentiometer is connected to.
 */
TargetPotentiometer::TargetPotentiometer(UINT32 channel)
{
  m_analog = new AnalogChannel(channel);
  m_channelAllocated = true;
  InitPOT();
}

/**
 * TargetPotentiometer constructor with a precreated analog channel object.
 * Use this constructor when the analog channel needs to be shared. There
 * is no reference counting when an AnalogChannel is passed to the TargetPotentiometer.
 * @param channel The AnalogChannel object that the TargetPotentiometer is connected to.
 */
TargetPotentiometer::TargetPotentiometer(AnalogChannel *channel)
{
  m_analog = channel;
  m_channelAllocated = false;
  if (channel == NULL)
  {
    wpi_fatal(NullParameter);
  }
  else
  {
    InitPOT();
  }
}

TargetPotentiometer::TargetPotentiometer(AnalogChannel &channel)
{
  m_analog = &channel;
  m_channelAllocated = false;
  InitPOT();
}

/**
 * Delete (free) the analog components used for the TargetPotentiometer.
 */
TargetPotentiometer::~TargetPotentiometer()
{
  if (m_channelAllocated)
    delete m_analog;
}

/**
 * Return the actual angle in degrees that the TargetPotentiometer is rotated in.
 * 
 * @return the current rotation of the TargetPotentiometer in degrees.
 */
float TargetPotentiometer::GetAngle(void)
{
  float lnVoltage = floor(m_analog->GetAverageVoltage()*1000);
  for (int i=0; i<POTAngles::numAnglesSpecified-1; i++)
  {
    if (lnVoltage == POTAngles::maVoltage[i])
    {
      return POTAngles::maAngles[i];
    }
    else if (lnVoltage > POTAngles::maVoltage[i] && lnVoltage < POTAngles::maVoltage[i+1])
    {
      double lnVoltsPerDegreeInRange = ((double)POTAngles::maVoltage[i+1] - POTAngles::maVoltage[i])
                                       /((double)POTAngles::maAngles[i+1] - POTAngles::maAngles[i]);
      double lnAngleInRange = (lnVoltage-POTAngles::maVoltage[i])/lnVoltsPerDegreeInRange;
      double lnAngle = lnAngleInRange + POTAngles::maAngles[i];
      //printf("pot %f angle %f\n", lnVoltage, lnAngle);
      return (float)lnAngle;
    }
    else if (lnVoltage == POTAngles::maVoltage[i+1])
    {
      return POTAngles::maAngles[i+1];
    }
  }
  if(lnVoltage < POTAngles::maVoltage[0])
  {
    return POTAngles::maAngles[0];
  }
  else if(lnVoltage > POTAngles::maVoltage[POTAngles::numAnglesSpecified-1])
  {
    return POTAngles::maAngles[POTAngles::numAnglesSpecified-1];
  }
  else
  {
    printf("ERROR: This should never be printed.  Check TargetPotentiometer calculations.\n");
    return 0;
  }
}

float TargetPotentiometer::GetRaw()
{
  return m_analog->GetAverageVoltage();
}

/**
 * Get the angle for the PID Source parent.
 * 
 * @return The current angle in degrees.
 */
double TargetPotentiometer::PIDGet()
{
  return GetAngle();
}
