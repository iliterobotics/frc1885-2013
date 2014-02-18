#ifndef INPUT_H
#define INPUT_H

#include "GlobalData.h"
#include "WPILib.h"
#include <Math.h>

class Input
{

public:
	~Input();
	
	static void GetInputs();

private:
	Input();

	static GlobalData* m_globals;

	static void GetDriverControlInputs();
	static void GetSensorInputs();
};

#endif
