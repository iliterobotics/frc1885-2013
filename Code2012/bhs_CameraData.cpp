#include "bhs_CameraData.h"
#define CAMERA_HEIGHT 55.5
#include <stdio.h>
bhs_CameraData::bhs_CameraData()
	: m_runCameraProcessing(true)
	, m_cameraProcessingIteration(0)

	, mRobotX(0.0)
	, mRobotZ(0.0)
	, mRobotTheta(0.0)
	, mOffsetDegrees(0.0)

	, mnMoveXDegreesToCenterHoop(0.0)
	, mnDistanceToCenterHoop(0.0)
	, mnAlignDataCnt(0)
	, mbIHaveLegitData(false)
{
	m_cameraSemaphore = semBCreate(SEM_Q_FIFO, SEM_FULL);
	memset(&mActualHoops, 0, sizeof(mActualHoops));
	mLineUp = new bhs_LineUp(0, 0, 0);
	mLineUp->configNewTarget(0, 0, 162);
}

void bhs_CameraData::setCameraData(double aRobotX, double aRobotZ, double aRobotTheta, double aOffsetDegrees) {
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	mRobotX = aRobotX;
	mRobotZ = aRobotZ;
	mRobotTheta = aRobotTheta;
	mOffsetDegrees = aOffsetDegrees;

	semGive(m_cameraSemaphore);
}

void bhs_CameraData::getCameraData(double &aRobotX, double &aRobotZ, double &aRobotTheta, double &aOffsetDegrees) {
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	aRobotX = mRobotX;
	aRobotZ = mRobotZ;
	aRobotTheta = mRobotTheta;
	aOffsetDegrees = mOffsetDegrees;

	semGive(m_cameraSemaphore);
}

bool bhs_CameraData::getShooterData( float& anMoveDegree, float& anDistanceToHoop, int &anDataCnt)
{
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	anMoveDegree = mnMoveXDegreesToCenterHoop;
	anDistanceToHoop = mnDistanceToTarget;
	anDataCnt = mnAlignDataCnt;
	bool lbTemp = mbIHaveLegitData;
	semGive(m_cameraSemaphore);
	return lbTemp;
}

void bhs_CameraData::setShooterData(bool abLegitData, float anMoveDegree, float anDistanceToHoop)
{
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	mnMoveXDegreesToCenterHoop = anMoveDegree;
	mnDistanceToCenterHoop = anDistanceToHoop;
	mbIHaveLegitData = abLegitData;
	mnDistanceToTarget = mnDistanceToCenterHoop - 39;
	printf("dist to hoop: %f\n", mnDistanceToTarget);
	mnAlignDataCnt++;
	semGive(m_cameraSemaphore);
}

void bhs_CameraData::clearHoopData()
{
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	mActualHoops[3].filled = false;
	mActualHoops[2].filled = false;
	mActualHoops[1].filled = false;
	mActualHoops[0].filled = false;
	semGive(m_cameraSemaphore);
}

void bhs_CameraData::copyCameraData(int anIndex, const bhs_CameraData::camData& source)
{
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	mActualHoops[anIndex].centerX = source.centerX;
	mActualHoops[anIndex].centerY = source.centerY;
	mActualHoops[anIndex].leftBound = source.leftBound;
	mActualHoops[anIndex].rightBound = source.rightBound;
	mActualHoops[anIndex].topBound = source.topBound;
	mActualHoops[anIndex].bottomBound = source.bottomBound;
	mActualHoops[anIndex].rectWidth = source.rectWidth;
	mActualHoops[anIndex].rectHeight = source.rectHeight;
	mActualHoops[anIndex].height = source.height;
	mActualHoops[anIndex].filled = true;
	mActualHoops[anIndex].distance = source.distance;
	mActualHoops[anIndex].angle = source.angle;
	
//	printf("Hoop %d:Center X:%d Center Y:%d Height:%d width:%d\n",anIndex,source.centerX,source.centerY,source.rectHeight,source.rectWidth);
	semGive(m_cameraSemaphore);
	return;
}

void bhs_CameraData::initActualHoops() {
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	mActualHoops[0].top = true;
	mActualHoops[1].midRight = true;
	mActualHoops[2].midLeft = true;
	mActualHoops[3].bottom = true;
	semGive(m_cameraSemaphore);
}

void bhs_CameraData::computeDistance(int index)
{
	
}
void bhs_CameraData::lineUpDataUpdate()
{
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	for(int index = 0; index < 3; index++) {
		if(mActualHoops[index].filled) {
			mRobotTheta = mActualHoops[index].angle;
			mRobotX = mActualHoops[index].distance*sin(mRobotTheta);
			mRobotZ = mActualHoops[index].distance*cos(mRobotTheta);

			mLineUp->setRobotPosition(mRobotX, mRobotZ);
			mLineUp->setRobotFacing(mRobotTheta);
			mOffsetDegrees = mLineUp->getOffsetDegrees(0);
			break;
		}
	}
	semGive(m_cameraSemaphore);
}

bool bhs_CameraData::getHoopData(int anIndex, bhs_CameraData::camData &source) {
	semTake(m_cameraSemaphore, WAIT_FOREVER);
	if(anIndex < 4) {
		source = mActualHoops[anIndex];
	}
	semGive(m_cameraSemaphore);
	return anIndex < 4;
}


