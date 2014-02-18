#include "bhs_LineUp.h"
#include <Math.h>

#define CENTERHOOP 0
#define LEFTHOOP 1
#define RIGHTHOOP 2

#define XVALUE 0
#define ZVALUE 1



bhs_LineUp::bhs_LineUp(double robotPosX, double robotPosZ, double robotFacingAngle) {

	setRobotPosition(robotPosX, robotPosZ);
	setRobotFacing(robotFacingAngle);

	mTargetPos = new double*[5]; 
	for (int i = 0; i < 5; ++i) {
		mTargetPos[i] = new double[2]; 
	}
	configNewTarget(LEFTHOOP, 134.625, 6);
	configNewTarget(CENTERHOOP, 162, 6);
	configNewTarget(RIGHTHOOP, 189.375, 6);
}

void bhs_LineUp::configNewTarget(int aTargetNumber, double aTargetPosX, double aTargetPosZ ) {
	mTargetPos[aTargetNumber][XVALUE] = aTargetPosX;
	mTargetPos[aTargetNumber][ZVALUE] = aTargetPosZ;
}

void bhs_LineUp::setRobotPosition(double aRobotPosX, double aRobotPosZ) {
	this->mRobotPosX = aRobotPosX;
	this->mRobotPosZ = aRobotPosZ;
}

void bhs_LineUp::setRobotFacing(double aRobotFacingAngle) {
	this->mRobotFacingAngle = aRobotFacingAngle;
}

double bhs_LineUp::getOffsetDegrees(int aTargetNumber) {
	double aXDifference = mTargetPos[aTargetNumber][XVALUE] - mRobotPosX;
	double aZDifference = mTargetPos[aTargetNumber][ZVALUE] - mRobotPosZ;

	double aPartialOrientation = 90 - ( (atan(aZDifference / aXDifference))*180/3.1415926 );
	double aOrientationOffset = aPartialOrientation - mRobotFacingAngle;

	aOrientationOffset = floor(aOrientationOffset * 100.0) / 100.0;

	return aOrientationOffset;
}

/*double bhs_LineUp::getOffsetMagnitude(int aTargetNumber) {
	double aXDifference = mTargetPos[aTargetNumber][XVALUE] - mRobotPosX;
	double aZDifference = mTargetPos[aTargetNumber][ZVALUE] - mRobotPosZ;

	double aDistanceFromTarget = sqrt(pow(aZDifference, 2) + pow(aXDifference, 2));
	double aAngleOffset = getOffsetDegrees(aTargetNumber)*3.1415926/180;
	double aOffsetMagnitude = aDistanceFromTarget * sin(aAngleOffset) / sin(3.1415926 - aAngleOffset - atan(aZDifference / aXDifference));

	return aOffsetMagnitude;
}*/

bool bhs_LineUp::onTarget(int aTargetNumber) {
	return getOffsetDegrees(aTargetNumber) == 0;
}

double bhs_LineUp::horizontalDistanceToTarget(int aTargetNumber) {
	double aDistanceFromTarget;
	if(onTarget(aTargetNumber)) {
		double aXDifference = mTargetPos[aTargetNumber][XVALUE] - mRobotPosX;
		double aZDifference = mTargetPos[aTargetNumber][ZVALUE] - mRobotPosZ;

		aDistanceFromTarget = sqrt(pow(aZDifference, 2) + pow(aXDifference, 2));
	} else {
		aDistanceFromTarget = -1;
	}

	return aDistanceFromTarget;
}



