#include "bhs_SensorInput.h"

// Constant instantiations go here

bhs_SensorInput::bhs_SensorInput(bhs_GlobalData* a_gd) {
	m_gd = a_gd;

	// Sensor instantiations go here
}

bhs_SensorInput::~bhs_SensorInput() {
	m_gd = 0;
}

void bhs_SensorInput::init() {
	// Set members to initial value
	// Reset sensors
}

void bhs_SensorInput::run() {
	// Read values from sensors and store in global variables
}
