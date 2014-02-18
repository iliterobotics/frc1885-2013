/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "bhs_Gyro.h"
#include "AnalogChannel.h"
#include "AnalogModule.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"

const UINT32 bhs_Gyro::kOversampleBits;
const UINT32 bhs_Gyro::kAverageBits;
const float bhs_Gyro::kSamplesPerSecond;
const float bhs_Gyro::kCalibrationSampleTimeA;
const float bhs_Gyro::kDefaultVoltsPerDegreePerSecond;

/**
 * Initialize the gyro.
 * Calibrate the gyro by running for a number of samples and computing the center value for this
 * part. Then use the center value as the Accumulator center value for subsequent measurements.
 * It's important to make sure that the robot is not moving while the centering calculations are
 * in progress, this is typically done when the robot is first turned on while it's sitting at
 * rest before the competition starts.
 */
void bhs_Gyro::InitGyro()
{
	m_deadband = 200;
	
	if (!m_analog->IsAccumulatorChannel())
	{
		wpi_setWPIErrorWithContext(ParameterOutOfRange,
				"moduleNumber and/or channel (must be accumulator channel)");
		if (m_channelAllocated)
		{
			delete m_analog;
			m_analog = NULL;
		}
		return;
	}

	m_voltsPerDegreePerSecond = kDefaultVoltsPerDegreePerSecond;
	m_analog->SetAverageBits(kAverageBits);
	m_analog->SetOversampleBits(kOversampleBits);
	float sampleRate = kSamplesPerSecond * 
		(1 << (kAverageBits + kOversampleBits));
	m_analog->GetModule()->SetSampleRate(sampleRate);
	Wait(1.0);

	m_analog->InitAccumulator();
	Wait(kCalibrationSampleTimeA);

	INT64 value;
	UINT32 count;
	m_analog->GetAccumulatorOutput(&value, &count);

	UINT32 center = (UINT32)((float)value / (float)count + 0.5);

	m_offset = ((float)value / (float)count) - (float)center;

	m_analog->SetAccumulatorCenter(center);
	m_analog->SetAccumulatorDeadband(m_deadband); ///< TODO: compute / parameterize this
	m_analog->ResetAccumulator();

	nUsageReporting::report(nUsageReporting::kResourceType_Gyro, m_analog->GetChannel(), m_analog->GetModuleNumber() - 1);
}

/**
 * Gyro constructor given a slot and a channel.
 * 
 * @param moduleNumber The analog module the gyro is connected to (1).
 * @param channel The analog channel the gyro is connected to (1 or 2).
 */
bhs_Gyro::bhs_Gyro(UINT8 moduleNumber, UINT32 channel)
{
	m_analog = new AnalogChannel(moduleNumber, channel);
	m_channelAllocated = true;
	InitGyro();
}

/**
 * Gyro constructor with only a channel.
 * 
 * Use the default analog module slot.
 * 
 * @param channel The analog channel the gyro is connected to.
 */
bhs_Gyro::bhs_Gyro(UINT32 channel)
{
	m_analog = new AnalogChannel(channel);
	m_channelAllocated = true;
	InitGyro();
}

/**
 * Gyro constructor with a precreated analog channel object.
 * Use this constructor when the analog channel needs to be shared. There
 * is no reference counting when an AnalogChannel is passed to the gyro.
 * @param channel The AnalogChannel object that the gyro is connected to.
 */
bhs_Gyro::bhs_Gyro(AnalogChannel *channel)
{
	m_analog = channel;
	m_channelAllocated = false;
	if (channel == NULL)
	{
		wpi_setWPIError(NullParameter);
	}
	else
	{
		InitGyro();
	}
}

bhs_Gyro::bhs_Gyro(AnalogChannel &channel)
{
	m_analog = &channel;
	m_channelAllocated = false;
	InitGyro();
}

/**
 * Reset the gyro.
 * Resets the gyro to a heading of zero. This can be used if there is significant
 * drift in the gyro and it needs to be recalibrated after it has been running.
 */
void bhs_Gyro::Reset()
{
	m_analog->ResetAccumulator();
}

/**
 * Delete (free) the accumulator and the analog components used for the gyro.
 */
bhs_Gyro::~bhs_Gyro()
{
	if (m_channelAllocated)
		delete m_analog;
}

/**
 * Return the actual angle in degrees that the robot is currently facing.
 * 
 * The angle is based on the current accumulator value corrected by the oversampling rate, the
 * gyro type and the A/D calibration values.
 * The angle is continuous, that is can go beyond 360 degrees. This make algorithms that wouldn't
 * want to see a discontinuity in the gyro output as it sweeps past 0 on the second time around.
 * 
 * @return the current heading of the robot in degrees. This heading is based on integration
 * of the returned rate from the gyro.
 */
float bhs_Gyro::GetAngle( void )
{
	INT64 rawValue;
	UINT32 count;
	m_analog->GetAccumulatorOutput(&rawValue, &count);

	INT64 value = rawValue - (INT64)((float)count * m_offset);

	double scaledValue = value * 1e-9 * (double)m_analog->GetLSBWeight() * (double)(1 << m_analog->GetAverageBits()) /
		(m_analog->GetModule()->GetSampleRate() * m_voltsPerDegreePerSecond);
	
	return (float)scaledValue;
}


/**
 * Set the gyro type based on the sensitivity.
 * This takes the number of volts/degree/second sensitivity of the gyro and uses it in subsequent
 * calculations to allow the code to work with multiple gyros.
 * 
 * @param voltsPerDegreePerSecond The type of gyro specified as the voltage that represents one degree/second.
 */
void bhs_Gyro::SetSensitivity( float voltsPerDegreePerSecond )
{
	m_voltsPerDegreePerSecond = voltsPerDegreePerSecond;
}

/**
 * Get the angle in degrees for the PIDSource base object.
 * 
 * @return The angle in degrees.
 */
double bhs_Gyro::PIDGet()
{
	return GetAngle();
}

int bhs_Gyro::getDeadband() {
	return m_deadband;
}

void bhs_Gyro::setDeadband(int a_inc) {
	m_deadband = a_inc;
	m_analog->SetAccumulatorDeadband(m_deadband);
}

void bhs_Gyro::incDeadband(int a_inc) {
	m_deadband += a_inc;
	m_analog->SetAccumulatorDeadband(m_deadband);
}
