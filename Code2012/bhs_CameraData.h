#ifndef bhs_CameraData_H
#define bhs_CameraData_H

#include <math.h>
#include <string.h>
#include "semLib.h"
#include "bhs_LineUp.h"
#include <stdio.h>
#include <vector>
#include "bhs_CameraData.h"
using std::vector;

class bhs_CameraData {
public:	
	struct camData {
		int centerX;
		int centerY;
		int leftBound;
		int rightBound;
		int topBound;
		int bottomBound;
		int rectWidth;
		int rectHeight;
		int height;
		double distance;
		double angle;
		bool filled;
		bool top;
		bool midRight;
		bool midLeft;
		bool bottom;
	};

	bhs_CameraData();

	//getters and setters
	void getCameraData(double& aRobotX, double& aRobotZ, double& aRobotTheta, double& offsetDegrees);
	void setCameraData(double aRobotX, double aRobotZ, double aRobotTheta, double offsetDegrees);

	bool getShooterData(float& anMoveDegree, float& anDistanceToHoop, int& anDataCnt);
	void setShooterData(bool abLegitData, float anMoveDegree, float anDistanceToHoop);

	bool getHoopData(int anIndex, bhs_CameraData::camData &source);
	void copyCameraData(int anIndex, const bhs_CameraData::camData &source);

	void clearHoopData();

	void initActualHoops();

	void computeDistance(int index);

	void lineUpDataUpdate();

	bool m_runCameraProcessing;
	long m_cameraProcessingIteration;

private:
	SEM_ID m_cameraSemaphore;
	camData mActualHoops[4];  //Note: mActualHoops[0] is the top, 1 is the mid-right, and 2 is the mid-left, 3 is bottom
	bhs_LineUp* mLineUp;

	double mRobotX;
	double mRobotZ;
	double mRobotTheta;
	double mOffsetDegrees;

	float mnMoveXDegreesToCenterHoop;
	float mnDistanceToCenterHoop;
	float mnDistanceToTarget;
	int mnAlignDataCnt;
	bool mbIHaveLegitData;

};
#endif
