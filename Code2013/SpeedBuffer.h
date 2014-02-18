#ifndef SPEEDBUFFER_H
#define SPEEDBUFFER_H

#include <map>
#include <Math.h>
#include "WPILib.h"

class SpeedBuffer
{
public:
	SpeedBuffer();
	~SpeedBuffer();
	bool SetMaxSpeed( Talon* motor, double speed );
	double BufferedSpeed( Talon* motor, double speed );
	
private:
	std::map <Talon*, double> m_speeds;
	
};

#endif
