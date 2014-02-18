#include "bhs_ShooterProcess.h"

#define POWERS_FROM_RATES_ARRAY_SIZE 10

// first index (input value) is the encoder rate, second index (output value) is the motor power
// guaranteed to be sorted by the first index
static double g_powersFromRates[POWERS_FROM_RATES_ARRAY_SIZE][2] = {
		{0, 0.0},
		{1950, 0.2},
		{3650, 0.3},
		{5280, 0.4},
		{7000, 0.5},
		{8620, 0.6},
		{10100, 0.7},
		{12100, 0.8},
		{13300, 0.9},
		{15200, 1.0}
};

#define RATES_FROM_DISTANCES_ARRAY_SIZE 11

// first index (input value) is the distance in inches, second index (output value) is the encoder rate
// guaranteed to be sorted by the first index
// the following values are calibrated for swishing, not banking
static double g_ratesFromDistances[RATES_FROM_DISTANCES_ARRAY_SIZE][2] = {
		{0, 0},
		{79.5, 8500},
		{87, 8614},
		{102, 9006},
		{115, 9225},
		{128.5, 9550},
		{148, 10120},
		{168, 10540},
		{197, 11500},
		{221, 12100},
		{245, 12690}
};

bhs_ShooterProcess::bhs_ShooterProcess() {

}

float bhs_ShooterProcess::getPowerFromRate(double a_desiredRate) {
	//	return interpolate((double**) g_powersFromRates, a_desiredRate, POWERS_FROM_RATES_ARRAY_SIZE);
	float l_power = 0.0;
	if (POWERS_FROM_RATES_ARRAY_SIZE > 0) {
		if (a_desiredRate < g_powersFromRates[0][0]) {
			l_power = g_powersFromRates[0][1];
		} else if (a_desiredRate >= g_powersFromRates[POWERS_FROM_RATES_ARRAY_SIZE-1][0]) {
			l_power = g_powersFromRates[POWERS_FROM_RATES_ARRAY_SIZE-1][1];
		} else {
			for (int i=0; i<POWERS_FROM_RATES_ARRAY_SIZE-1; i++) {
				if (a_desiredRate >= g_powersFromRates[i][0] && a_desiredRate < g_powersFromRates[i+1][0]) {
					double l_slope = (g_powersFromRates[i+1][1] - g_powersFromRates[i][1]) /
							(g_powersFromRates[i+1][0] - g_powersFromRates[i][0]);
					l_power = g_powersFromRates[i][1] + l_slope * (a_desiredRate - g_powersFromRates[i][0]);
					break;
				}
			}
		}
	}
	return l_power;
}

double bhs_ShooterProcess::getRateFromDistance(double a_distance) {
	//	return interpolate((double**) g_ratesFromDistances, a_distance, RATES_FROM_DISTANCES_ARRAY_SIZE);
	double l_rate = 0.0;
	if (RATES_FROM_DISTANCES_ARRAY_SIZE > 0) {
		if (a_distance < g_ratesFromDistances[0][0]) {
			l_rate = g_ratesFromDistances[0][1];
		} else if (a_distance >= g_ratesFromDistances[RATES_FROM_DISTANCES_ARRAY_SIZE-1][0]) {
			l_rate = g_ratesFromDistances[RATES_FROM_DISTANCES_ARRAY_SIZE-1][1];
		} else {
			for (int i=0; i<RATES_FROM_DISTANCES_ARRAY_SIZE-1; i++) {
				if (a_distance >= g_ratesFromDistances[i][0] && a_distance < g_ratesFromDistances[i+1][0]) {
					double l_slope = (g_ratesFromDistances[i+1][1] - g_ratesFromDistances[i][1]) /
							(g_ratesFromDistances[i+1][0] - g_ratesFromDistances[i][0]);
					l_rate = g_ratesFromDistances[i][1] + l_slope * (a_distance - g_ratesFromDistances[i][0]);
					break;
				}
			}
		}
	}
	return l_rate;
}

// a_array must have two columns per row, the first column being the input value and
// the second column being the output value
double bhs_ShooterProcess::interpolate(double** a_array, double a_inputValue, int a_arraySize) {
	double l_outputValue = 0.0;
	if (a_arraySize > 0) {
		if (a_inputValue < a_array[0][0]) {
			// if the input value is less than the smallest input value in the array,
			// then return the output value that corresponds with the smallest input value
			l_outputValue = a_array[0][1];
		} else if (a_inputValue >= a_array[a_arraySize-1][0]) {
			// if the input value is greater than the largest input value in the array,
			// then return the output value that corresponds with the largest input value
			l_outputValue = a_array[a_arraySize-1][1];
		} else {
			// the input value must be within the smallest and largest input values in the array
			for (int i=0; i<a_arraySize-1; i++) {
				// if the input value is within two consecutive input values in the array,
				// then return a linearly interpolated value between the two consecutive output values
				// that correspond to those two consecutive input values
				if (a_inputValue >= a_array[i][0] && a_inputValue < a_array[i+1][0]) {
					double l_slope = (a_array[i+1][1] - a_array[i][1]) /
							(a_array[i+1][0] - a_array[i][0]);
					l_outputValue = a_array[i][1] + l_slope * (a_inputValue - a_array[i][0]);
					break;
				}
			}
		}
	}
	return l_outputValue;
}
