#ifndef BHS_ENCODER_PID_H
#define BHS_ENCODER_PID_H

#include "bhs_PID.h"

class bhs_EncoderPID {
public:
	static const double DEFAULT_MAX_PID_ADDITION;
	
	bhs_EncoderPID();
	
	void setMaxPIDAddition(double);
	double getMaxPIDAddition();
	void setPID(double, double, double);
	float getMotorPowFromEncRate(float, double, double);
	
private:
	bhs_PID m_pid;
	
	double m_maxPIDAddition;
	
};

#endif
