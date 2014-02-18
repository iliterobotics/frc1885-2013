/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef BHS_GYRO_H
#define BHS_GYRO_H

#include "SensorBase.h"
#include "PIDSource.h"

class AnalogChannel;
class AnalogModule;

/**
 * Use a rate gyro to return the robots heading relative to a starting position.
 * The Gyro class tracks the robots heading based on the starting position. As the robot
 * rotates the new heading is computed by integrating the rate of rotation returned
 * by the sensor. When the class is instantiated, it does a short calibration routine
 * where it samples the gyro while at rest to determine the default offset. This is
 * subtracted from each sample to determine the heading. This gyro class must be used 
 * with a channel that is assigned one of the Analog accumulators from the FPGA. See
 * AnalogChannel for the current accumulator assignments.
 */
class bhs_Gyro : public SensorBase, public PIDSource
{
public:
	static const UINT32 kOversampleBits = 8;
	static const UINT32 kAverageBits = 0;
	static const float kSamplesPerSecond = 50.0;
	static const float kCalibrationSampleTimeA = 5.0;
	static const float kDefaultVoltsPerDegreePerSecond = 0.007;

	bhs_Gyro(UINT8 moduleNumber, UINT32 channel);
	explicit bhs_Gyro(UINT32 channel);
	explicit bhs_Gyro(AnalogChannel *channel);
	explicit bhs_Gyro(AnalogChannel &channel);
	virtual ~bhs_Gyro();
	virtual float GetAngle();
	void SetSensitivity(float voltsPerDegreePerSecond);
	virtual void Reset();

	// PIDSource interface
	double PIDGet();
	
	int getDeadband();
	void setDeadband(int);
	void incDeadband(int);

private:
	void InitGyro();

	AnalogChannel *m_analog;
	float m_voltsPerDegreePerSecond;
	float m_offset;
	bool m_channelAllocated;
	
	int m_deadband;
};
#endif
