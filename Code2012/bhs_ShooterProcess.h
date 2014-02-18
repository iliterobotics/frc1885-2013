#ifndef BHS_SHOOTER_PROCESS_H
#define BHS_SHOOTER_PROCESS_H

class bhs_ShooterProcess {
public:
	bhs_ShooterProcess();
	
	float getPowerFromRate(double);
	double getRateFromDistance(double);

private:
	double interpolate(double**, double, int);

};

#endif
