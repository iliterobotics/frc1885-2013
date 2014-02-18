/*
 * bhs_Accelerometer.cpp
 *
 *  Created on: Jan 29, 2009
 *      Author: ITRobo
 */

#include "bhs_Accelerometer.h"
#include "bhs_Constants.h"
#include "Timer.h"
#include "AnalogModule.h"
#include "Utility.h"
#include "WPIStatus.h"
#include "sysLib.h"

static int AccelerometerStartupTask(int apAccel)
{
  ((bhs_Accelerometer*)apAccel)->InitAccelerometer();
  return 0;
}

static int AccelerometerAccumulatorTiming(int apAccel, int anPeriodicDelay)
{
  bhs_Accelerometer* lpAccel = (bhs_Accelerometer*)apAccel;
  lpAccel->runAccumulator(anPeriodicDelay);
  return 0;
}

void bhs_Accelerometer::runAccumulator(int anPeriodicDelay)
{
  while (!mbKillAccumulator)
  {
    taskDelay(anPeriodicDelay);
    semGive(mpAccumSem);
  }
  semFlush(mpAccumSem);
}

void bhs_Accelerometer::AccelerometerStartup()
{
  mnAccelerometerStartupTaskId = taskSpawn("Accelerometer Startup",
                                            101,           //task pri.
                                            VX_FP_TASK,       // options
                                            64000,          // stack size
                                            (FUNCPTR)AccelerometerStartupTask, // function to start
                                            (int)this,       // targeting class to start
                                             0, 0, 0, 0, 0, 0, 0, 0, 0);
}

/**
 * Common function for initializing the accelerometer.
 */
void bhs_Accelerometer::InitAccelerometer()
{
  m_voltsPerG = 1.0;
  
  m_analogChannel->SetAverageBits(kAverageBits);
  m_analogChannel->SetOversampleBits(kOversampleBits);
  float sampleRate = kSamplesPerSecond * 
    (1 << (kAverageBits + kOversampleBits));
  m_analogChannel->GetModule()->SetSampleRate(sampleRate);
  Wait(1.0);
  
  int lnPeriodicDelay = sysClkRateGet()/(int)kSamplesPerSecond;
  
  mnAccelerometerAccumulatorTaskId = taskSpawn("Accelerometer Accumulator Timing", 99, //task pri.
                                                VX_FP_TASK, // options
                                                64000, // stack size
                                                (FUNCPTR)AccelerometerAccumulatorTiming, // function to start
                                                (int)this, // targeting class to start
                                                lnPeriodicDelay, 0, 0, 0, 0, 0, 0, 0, 0);
  
  int lnSampleCount = 0;
  float lnTempValue = 0;
  float lnMaximumValue = -10000;
  float lnMinimumValue = 10000;
  float lnAccumulatedValue = 0;
  
  while(lnSampleCount < bhs_Constants::cnACCELEROMETER_CALIBRATE_CYCLES + bhs_Constants::cnACCELEROMETER_CALIBRATE_IGNORE)
  {
    semTake(mpAccumSem, WAIT_FOREVER);
    
    lnTempValue = m_analogChannel->GetAverageVoltage();
    if(lnSampleCount >= bhs_Constants::cnACCELEROMETER_CALIBRATE_IGNORE)
    {
      lnAccumulatedValue += lnTempValue;
      if(lnTempValue > lnMaximumValue)
      {
        lnMaximumValue = lnTempValue;
      }
      if(lnTempValue < lnMinimumValue)
      {
        lnMinimumValue = lnTempValue;
      }
    }
    lnSampleCount++;
  }
  mbKillAccumulator = true;

  m_zeroGVoltage = lnAccumulatedValue / bhs_Constants::cnACCELEROMETER_CALIBRATE_CYCLES;
  m_deadZoneLow = lnMinimumValue;
  m_deadZoneHigh = lnMaximumValue;
  
  printf("zero %f deadLow %f deadHigh %f\n", m_zeroGVoltage, m_deadZoneLow, m_deadZoneHigh);
}

/**
 * Create a new instance of an accelerometer.
 * 
 * The accelerometer is assumed to be in the first analog module in the given analog channel. The
 * constructor allocates desired analog channel.
 */
bhs_Accelerometer::bhs_Accelerometer(UINT32 channel)
{
  m_analogChannel = new AnalogChannel(channel);
  m_allocatedChannel = true;
  mpAccumSem = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
  mbKillAccumulator = false;
  AccelerometerStartup();
}

/**
 * Create new instance of accelerometer.
 * 
 * Make a new instance of the accelerometer given a module and channel. The constructor allocates
 * the desired analog channel from the specified module
 */
bhs_Accelerometer::bhs_Accelerometer(UINT32 slot, UINT32 channel)
{
  m_analogChannel = new AnalogChannel(slot, channel);
  m_allocatedChannel = true;
  AccelerometerStartup();
}

/**
 * Create a new instance of Accelerometer from an existing AnalogChannel.
 * Make a new instance of accelerometer given an AnalogChannel. This is particularly
 * useful if the port is going to be read as an analog channel as well as through
 * the Accelerometer class.
 */
bhs_Accelerometer::bhs_Accelerometer(AnalogChannel *channel)
{
  if (channel == NULL)
  {
    wpi_fatal(NullParameter);
  }
  else
  {
    m_analogChannel = channel;
    AccelerometerStartup();
  }
  m_allocatedChannel = false;
}
  
/**
 * Delete the analog components used for the accelerometer.
 */
bhs_Accelerometer::~bhs_Accelerometer()
{
  if (m_allocatedChannel)
  {
    delete m_analogChannel;
  }
}

/**
 * Return the acceleration in Gs.
 * 
 * The acceleration is returned units of Gs.
 * 
 * @return The current acceleration of the sensor in Gs.
 */
float bhs_Accelerometer::GetAcceleration()
{
  float lnAverageVoltage = m_analogChannel->GetAverageVoltage();
  if(lnAverageVoltage > m_deadZoneLow && lnAverageVoltage < m_deadZoneHigh)
  {
    return 0;
  }
  else
  {
    return (m_analogChannel->GetAverageVoltage() - m_zeroGVoltage) / m_voltsPerG;
  }
}

/**
 * Set the accelerometer sensitivity.
 * 
 * This sets the sensitivity of the accelerometer used for calculating the acceleration.
 * The sensitivity varys by accelerometer model. There are constants defined for various models.
 * 
 * @param sensitivity The sensitivity of accelerometer in Volts per G.
 */
void bhs_Accelerometer::SetSensitivity(float sensitivity)
{
  m_voltsPerG = sensitivity;
}

/**
 * Set the voltage that corresponds to 0 G.
 * 
 * The zero G voltage varys by accelerometer model. There are constants defined for various models.
 * 
 * @param zero The zero G voltage.
 */
void bhs_Accelerometer::SetZero(float zero)
{
  m_zeroGVoltage = zero;
}

