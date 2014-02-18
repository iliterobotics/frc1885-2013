#ifndef BHS_DIAGNOSTICS_H
#define BHS_DIAGNOSTICS_H

#include "bhs_Constants.h"
#include "bhs_GlobalData.h"

class bhs_Diagnostics {
public:
	bhs_Diagnostics(bhs_GlobalData*);
	~bhs_Diagnostics();

	void run();

private:
	bhs_GlobalData* m_globalData;
	
	bhs_CameraData::camData m_topHoopData;
	bhs_CameraData::camData m_leftHoopData;
	bhs_CameraData::camData m_rightHoopData;
	bhs_CameraData::camData m_bottomHoopData;
	
};

#endif // BHS_DIAGNOSTICS_H
