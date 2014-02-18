#include "SpeedBuffer.h"

SpeedBuffer :: SpeedBuffer ()
{
	
}

SpeedBuffer :: ~SpeedBuffer ()
{
	
}


bool SpeedBuffer :: SetMaxSpeed ( Talon* motor, double speed )
{
	if ( m_speeds.count(motor) == 1 )
		return false;
	else
	{
		m_speeds[motor] = speed;
		return true;
	}
}

double SpeedBuffer :: BufferedSpeed ( Talon* motor, double speed )
{
	if ( m_speeds.count(motor) == 0 )
		return 0;
	else
	{
		if ( fabs(speed) > m_speeds[motor] )
			return m_speeds[motor] * ( speed / fabs(speed) );
		else
			return speed;
	}
}
