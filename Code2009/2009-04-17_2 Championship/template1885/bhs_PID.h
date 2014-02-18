#ifndef BHS_PID_H_
#define BHS_PID_H_
/* CLASS NAME: 
 * 				bhs_PID
 * DESCRIPTION:
 * 				This class implements a PID (Proportional, Integal, Derivative) 
 * 				controller.
 * 
 * 				Usage:  
 * 						1 - Instantiate class instance
 * 						2 - call setGains to configure PID gain values
 * 						2.5 - call setIntegralLimit (if needed)
 * 						3 - call computeOutput to determine the device drive needed
 * 
 */

class bhs_PID
{
public:
	bhs_PID();
	virtual ~bhs_PID();
	
	/* FUNCTION NAME:	setGains
	 * DESCRIPTION	:	configures PID gain for the P, I and D terms of the PID
	 * 					controller
	 * 					NOTE: This PID controller implementation does not provide
	 * 							a sample period/frequency.  It is the 
	 * 							responsibility of the user to incorporate dt 
	 * 							into the I and D gains.  This was done to
	 * 							minimize computation.
	 * PARAMETERS	:
	 * 	anProportional  - Proportional Gain
	 *  anIntegral		- Integral Gain
	 *  anDerivative	- Derivative Gain
	 * RETURN		:	NA
	 */
	void setGains(double anProportional, double anIntegral, double anDerivative);
	
	/* FUNCTION NAME:	setIntegralLimit
	 * DESCRIPTION	:	Limits the Integral Error (sum term).  This is to prevent
	 * 					the Integral term from getting too large or small.
	 * 					This function/feature may be ignore if the PID controller
	 * 					does not need this capability. 
	 * PARAMETERS	:
	 *  anUpperLimit - Largest Integral error sum
	 *  anLowerLimit - Smallest (left most on the number line) Integral error sum
	 * RETURN		:	NA
	 */
	void setIntegralLimit(double anUpperLimit, double anLowerLimit);

	/* FUNCTION NAME:	reset
	 * DESCRIPTION	:	computeOutput will act like it is being called for the
	 * 					first time after reset is called.  The Derivative and
	 * 					Integral components will get reset.
	 * PARAMETERS	:	NA
	 * RETURN		:	NA
	 */
	void reset();
	
	/* FUNCTION NAME:	computeOutput
	 * DESCRIPTION	:	computes the PID output value
	 * PARAMETERS	:
	 *  anDesireOuput 	- value device needs to be 
	 *  anCurrent		- current value of device 
	 * RETURN		:	drive value to give to device for this iteration
	 */
	double computeOutput(double anDesireOuput, double anCurrent);
protected:
	double mnProportionalGain;
	double mnIntegralGain;
	double mnDerivativeGain;
	
	//limitation parameters
	bool   mbEnforcePIDOutputLimit;
	bool   mnEnforceIntegralLimit;
	
	
	double mnPIDOutputUpperLimit;
	double mnPIDOutputLowerLimit;
	double mnIntegralErrorUpperLimit;
	double mnIntegralErrorLowerLimit;
	
	//PID state variables
	double mnIntergralError;
	bool   mbCanComputDerivativeGain;
	
	double mnPreviousDevicePosition;
};

#endif /*BHS_PID_H_*/
