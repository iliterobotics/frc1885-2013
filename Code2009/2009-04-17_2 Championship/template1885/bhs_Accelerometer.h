/*
 * bhs_Accelerometer.h
 *
 *  Created on: Jan 29, 2009
 *      Author: ITRobo
 */

#ifndef BHSACCELEROMETER_H_
#define BHSACCELEROMETER_H_

#include "AnalogChannel.h"
#include "SensorBase.h"

/** 
 * Handle operation of the accelerometer.
 * The accelerometer reads acceleration directly through the sensor. Many sensors have
 * multiple axis and can be treated as multiple devices. Each is calibrated by finding
 * the center value over a period of time.
 */
class bhs_Accelerometer : public SensorBase {
public:
  static const UINT32 kOversampleBits = 7;
  static const UINT32 kAverageBits = 3;
  //This is not the true sample rate that the FPGA is performing.
  //Read the true sample rate in the constructor.
  static const float kSamplesPerSecond = 50.0;
  static const float kDefaultVoltsPerDegreePerSecond = 0.0125;
  
  explicit bhs_Accelerometer(UINT32 channel);
  bhs_Accelerometer(UINT32 slot, UINT32 channel);
  explicit bhs_Accelerometer(AnalogChannel *channel);
  virtual ~bhs_Accelerometer();

  float GetAcceleration();
  void SetSensitivity(float sensitivity);
  void SetZero(float zero);
  
  void InitAccelerometer();
  void runAccumulator(int anPeriodicDelay);

private:
  void AccelerometerStartup();
  
  int mnAccelerometerStartupTaskId;
  int mnAccelerometerAccumulatorTaskId;
  bool mbKillAccumulator;
  SEM_ID mpAccumSem;

  AnalogChannel *m_analogChannel;
  float m_voltsPerG;
  float m_zeroGVoltage;
  float m_deadZoneLow;
  float m_deadZoneHigh;
  bool m_allocatedChannel;
};

#endif

