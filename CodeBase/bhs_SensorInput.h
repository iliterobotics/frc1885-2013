#ifndef BHS_SENSOR_INPUT_H_
#define BHS_SENSOR_INPUT_H_

#include <list>

#include "AnalogModule.h"
#include "AnalogChannel.h"
#include "Encoder.h"
#include "DigitalInput.h"
#include "DriverStation.h"

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"
#include "bhs_Input.h"

using namespace std;

class bhs_SensorInput : public bhs_Input {
	public:
		// analog inputs
		// static const UINT32 _CHANNEL;
		// ...
		
		// digital inputs
		// static const UINT32 _CHANNEL;
		// ...
		
		bhs_SensorInput(bhs_GlobalData*);
		virtual ~bhs_SensorInput();

		virtual void init();
		virtual void run();

	private:
		bhs_GlobalData* m_gd;

		// Sensor declarations go here
};

#endif //BHS_SENSOR_INPUT_H_
