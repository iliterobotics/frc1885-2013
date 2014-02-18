#ifndef __BHS_CAM__H__
#define __BHS_CAM__H__

#include "GlobalData.h"

class bhs_CAM
{
public:
	bhs_CAM();
	~bhs_CAM();
	
	static GlobalData* m_globals;
	
	double Lsum;
	int Lcount;
	double Lavg;
	
	enum Position
	{
		PosDown,
		Pos1,
		Pos2,
		Pos3,
		Pos4,
		PosMax
	};	

	bool lightCheck();
	void initPos();
	void setPosition(Position p);
	Position getPosition() const;
	
	private:
Position currPos;
};

#endif // __BHS_CAM__H__
