#ifndef BHS_SHOOTER_H
#define BHS_SHOOTER_H

#include "bhs_GDShooterData.h"
#include "bhs_ShooterProcess.h"
#include "bhs_EncoderPID.h"

class bhs_Shooter {
public:
	static const float LAY_UP_POWER;
	static const float POWER_SHOT_POWER;
	
	static const float POW_RAMP_A;
	static const float POW_RAMP_B;
	static const float POW_RAMP_INC;
	
	bhs_Shooter(bhs_GDShooterData*);
	~bhs_Shooter();
	
	void run();
	
private:
	bhs_GDShooterData* m_shooterData;
	bhs_ShooterProcess m_shooterProcess;
	bhs_EncoderPID m_encoderPID;
	
	bool m_constantOverridePower;
	float m_desiredConstantPower;
	
};

#endif
