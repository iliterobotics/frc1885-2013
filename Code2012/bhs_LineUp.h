#ifndef BHS_LINE_UP_H
#define BHS_LINE_UP_H

class bhs_LineUp{
public:
	bhs_LineUp(double robotPosX, double robotPosZ, double robotFacingAngle); 
	void configNewTarget(int aTargetNumber, double aTargetPosX, double aTargetPosZ );
	void setRobotPosition(double aRobotPosX, double aRobotPosZ);
	void setRobotFacing(double aRobotFacingAngle);
	double getOffsetDegrees(int aTargetNumber);
	double getOffsetMagnitude(int aTargetNumber);
	bool onTarget(int aTargetNumber);
	double horizontalDistanceToTarget(int aTargetNumber);
	
	
private:
	double mRobotPosX; //Inches
	double mRobotPosZ; //Inches
	double** mTargetPos; //Inches
	double mRobotFacingAngle; //Degrees
};

#endif
