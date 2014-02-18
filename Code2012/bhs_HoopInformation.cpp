#ifndef BHS_HOOP_INFORMATION
#define BHS_HOOP_INFORMATION
#define CAMERA_HEIGHT 55.5
#include <stdio.h>
#include <Math.h>
#include <vector>
#include <list>
#include "bhs_CameraData.h"
#include "bhs_AxisCamera.h"

using std::vector;
class bhs_HoopInformation
{
public:
	
	static void fixImageData(vector<bhs_CameraData::camData> &targetList, bhs_CameraData* cameraData)
	{
		if(targetList.size()>0)
		{
		int largestArea = 0;
		vector<bhs_CameraData::camData>::iterator iterator;
		for(iterator = targetList.begin(); iterator != targetList.end(); iterator++)
		{
			if((*iterator).rectHeight*(*iterator).rectWidth > largestArea)
			{
				largestArea = (*iterator).rectHeight*(*iterator).rectWidth;
			}
		}
		for(iterator = targetList.begin(); iterator != targetList.end(); iterator++)
		{
			if((*iterator).rectHeight*(*iterator).rectWidth < largestArea*0.5)
			{
				iterator = targetList.erase(iterator);
				iterator--;
			}
		}
		AssignHoops(targetList,cameraData);
		float distance = estimateDistance(cameraData);
		if(distance > 0)
		{
			cameraData->setShooterData(true,0,distance);
		}
	//	computeRobotAngle(cameraData);
		}
	}
private:
	static void FourLargestTargets(vector<bhs_CameraData::camData> &targetList)
	{
		list<bhs_CameraData::camData> largestTargets;
		list<bhs_CameraData::camData>::iterator listPosition;
		vector<bhs_CameraData::camData>::iterator vecIterator;
		for(vecIterator = targetList.begin(); vecIterator != targetList.end(); vecIterator++)
		{
			printf("Before Modification X: %d\n",(*vecIterator).centerX);
		}
		printf("\n");
		for(vecIterator = targetList.begin(); vecIterator != targetList.end(); vecIterator++)
		{
			listPosition = largestTargets.begin();
			if((*vecIterator).rectHeight*(*vecIterator).rectWidth > (*listPosition).rectWidth * (*listPosition).rectHeight)
			{
				largestTargets.push_front(*vecIterator);
			}
			else if ((*vecIterator).rectHeight*(*vecIterator).rectWidth > (*(++listPosition)).rectWidth * (*listPosition).rectHeight)
			{																//^Note that the list iterator is incremented with every check
				largestTargets.insert(listPosition,*vecIterator);		//insert the target that belongs at that point
			}
			else if ((*vecIterator).rectHeight*(*vecIterator).rectWidth > (*(++listPosition)).rectWidth * (*listPosition).rectHeight)
			{
				largestTargets.insert(listPosition,*vecIterator);
			}
			else if ((*vecIterator).rectHeight*(*vecIterator).rectWidth > (*(++listPosition)).rectWidth * (*listPosition).rectHeight)
			{
				largestTargets.insert(listPosition,*vecIterator);
			}
			
		}
		targetList.clear();
		int listLength = (largestTargets.size()<4)? largestTargets.size() : 4;
		for(int i = 0; i<listLength; i++)
		{
			targetList.push_back(largestTargets.front());
			largestTargets.pop_front();
		}
		for(vecIterator = targetList.begin(); vecIterator != targetList.end(); vecIterator++)
		{
			printf("After Modification X: %d\n",(*vecIterator).centerX);
		}
		
	}
	static float estimateDistance(bhs_CameraData* cameraData)
	{
		float distance = -1;
		bhs_CameraData::camData distanceTarget;
		cameraData->getHoopData(2,distanceTarget);
		if(distanceTarget.filled)
		{
			distance = -1.411 * (240 - distanceTarget.centerY)+199.2;
		}
		else
		{
			cameraData->getHoopData(1,distanceTarget);
			if(distanceTarget.filled)
			{
				distance = -1.411 * (240 - distanceTarget.centerY)+199.2;
			}
		}
		return distance;
	}
	static float computeDistanceForRobot(bhs_CameraData* cameraData)
	{
		float distance = -1.0;
		float height = 0; //height, in inches, from the height of the camera's lense
		float lnInchesPerPixel = 0;
		bhs_CameraData::camData distanceTarget;
		cameraData->getHoopData(3,distanceTarget);
		if(distanceTarget.filled)		//Check the hoops in ascending order until ont that is filled is found
		{								//Then determine the number of inches per pixel at the distance of the hoop
			height = CAMERA_HEIGHT - 39;
			lnInchesPerPixel = height/(distanceTarget.centerY - 240);
		}
		else
		{
			cameraData->getHoopData(2,distanceTarget);
			if(distanceTarget.filled)
			{
				height = 72 - CAMERA_HEIGHT;
				lnInchesPerPixel = height/(240 - distanceTarget.centerY);
			}
			else
			{
				cameraData->getHoopData(1, distanceTarget);
				if(distanceTarget.filled)
				{
					height = 72 - CAMERA_HEIGHT;
					lnInchesPerPixel = height/(240 - distanceTarget.centerY);
				}
				else
				{
					cameraData->getHoopData(0, distanceTarget);
					if(distanceTarget.filled)
					{
						height = 109 - CAMERA_HEIGHT;
						lnInchesPerPixel = height/(240 - distanceTarget.centerY);
					}
				}
			}
		}
		//Hoop Heights are 98, 61, and 28 inches, with their centers being 11 inches above those heights
		distance = lnInchesPerPixel/0.00140625;	
		return distance;
	}
	
	
	static void AssignHoops(vector<bhs_CameraData::camData> &mHoop, bhs_CameraData* cameraData)
	{
		cameraData->clearHoopData();

				//Places the hoops into the corresponding data structures
				//0 = top
				//1 = middle right
				//2 = middle left
				//3 = bottom
			
			int lnTopCenterX, lnBottomCenterX;
			//iterate through structure to get buttom
			bool lbBottomFound = false;
			for(vector<bhs_CameraData::camData>::iterator ITR = mHoop.begin();
					ITR != mHoop.end(); ITR++)  
			{
				if((*ITR).centerY  > 240)
				{
					printf("Found Bottom\n");
					cameraData->copyCameraData(3,*ITR);
					lnBottomCenterX = (*ITR).centerX;
						
					mHoop.erase(ITR);//remove from vector
					lbBottomFound = true;
					break;
				}
			}
			if(lbBottomFound)
			{					
				printf("mHoop size %d\n",mHoop.size());
			}
			bool lbTopFound = false;
			if(mHoop.size() >= 2)//we still have to get the top
			{
				vector<bhs_CameraData::camData>::iterator ITR_WORKING = mHoop.end();
				vector<bhs_CameraData::camData>::iterator ITR_TOPFOUND = mHoop.end();
				int lnCenterYLowest = 1200;
				int lnRectHalfHeight = 0;
					
				for(vector<bhs_CameraData::camData>::iterator ITR = mHoop.begin();ITR != mHoop.end(); ITR++)  
				{
					if(ITR == mHoop.begin())
					{
						ITR_WORKING = ITR;
						lnCenterYLowest = (*ITR).centerY;
						lnRectHalfHeight = (*ITR).rectHeight/2;
					}
					else
					{
					if((lnCenterYLowest) < ((*ITR).centerY- ((*ITR).rectHeight/2)))
					{
						ITR_TOPFOUND = ITR_WORKING;
						break;
					}
					else if(((*ITR).centerY) <  (lnCenterYLowest-lnRectHalfHeight))
					{
						ITR_TOPFOUND = ITR;
					}	
				}
			}
			
			if(ITR_TOPFOUND != mHoop.end())//we have top hoop
			{
				printf("Top FOUND");
				cameraData->copyCameraData(0, *ITR_TOPFOUND);
				lnTopCenterX = (*ITR_TOPFOUND).centerX;
				lbTopFound = true;
				mHoop.erase(ITR_TOPFOUND);
			}
			
		}
		if(lbTopFound)
		{
			printf("TOP found - Hoop size %d\n",mHoop.size());
		}
		if(mHoop.size() >= 2)
		{
			//default to first image box is on right and second image is on left
			int lnFirstElementIndex = 1;
			int lnSecondElementIndex = 2;
			
			if(mHoop[0].centerX < mHoop[2].centerX)//after testing we decided that first image is on left and second image is on right
			{ //??? Why is the second Index 2 rather than 1?
				lnFirstElementIndex = 2;
				lnSecondElementIndex = 1;
			}
			cameraData->copyCameraData(lnFirstElementIndex,mHoop[0]);
			cameraData->copyCameraData(lnSecondElementIndex,mHoop[1]);
		}
		else if(mHoop.size() > 0)
		{
		int lnSaveToIndex = 1;//default to middle right
		if(lbTopFound)
			{
				if (mHoop[0].centerX < lnTopCenterX)
				{
					lnSaveToIndex = 2;//left middle
				}
			}
			else if(lbBottomFound)
			{
				if (mHoop[0].centerX < lnBottomCenterX)
				{
					lnSaveToIndex = 2;//lefty middle
				}
			}
			//else default to right middle anyway (index = 2)
			cameraData->copyCameraData(lnSaveToIndex,mHoop[0]);
		}
	}
	static void computeRobotAngle(bhs_CameraData *cameraData)
	{
	/*	bhs_CameraData::camData lcTopHoop, lcLeftHoop, lcRightHoop;
		cameraData->getHoopData(0,lcTopHoop);
		cameraData->getHoopData(2,lcLeftHoop);
		cameraData->getHoopData(1,lcRightHoop);
		int lnMiddle = ((lcRightHoop.centerX - lcLeftHoop.centerX)/2) + lcLeftHoop.centerX;
		int lnTopOffset = lcTopHoop.centerX - lnMiddle;
	
		
		
		//Old Angle Calculation below:
		
		bhs_CameraData::camData leftHoop;
		bhs_CameraData::camData rightHoop;
		cameraData->getHoopData(1,rightHoop);
		cameraData->getHoopData(2,leftHoop);
		bhs_CameraData::camData topHoop;
		cameraData->getHoopData(0,topHoop);
		if(leftHoop.filled&&rightHoop.filled)
		{
			float distBetweenHoops = 54.75; //inches
			float distanceRight = 0;
			float distanceLeft = 0;
			float height = 72 - CAMERA_HEIGHT;
			float lnInchesPerPixel = height/(240 - rightHoop.centerY);
			distanceRight = lnInchesPerPixel/0.00140625;		//Compute Distance to both middle hoops
			lnInchesPerPixel = height/(240 - leftHoop.centerY);
			distanceLeft = lnInchesPerPixel/0.00140625;
			
			float angleFromRight = acos(((distBetweenHoops*distBetweenHoops)+(distanceRight*distanceRight)-(distanceLeft*distanceLeft))
					/(2*distBetweenHoops*distanceRight));  //Law of Cosines
			float rightHoopX = 351.375;
			float rightHoopZ = 0;
			float robotX = rightHoopX + (cos(angleFromRight)*distanceRight);
			float robotZ = rightHoopZ + (sin(angleFromRight)*distanceRight);		//Compute both the robot's x an y position
			//
			
			float distancePerPixel = (distanceRight - distanceLeft) / (rightHoop.centerX - leftHoop.centerX);	//the nuber of inches that the distance changes for every pixel to the right
			float topDistance = (topHoop.centerX - leftHoop.centerX) * distancePerPixel + distanceLeft;
			float distanceFromTop = (320 - topHoop.centerX)* distancePerPixel;
			float inchesStraightForward = distanceFromTop + topDistance;
			
	//		float robotAngle = acos(((topDistance*topDistance)+(inchesStraightForward * inchesStraightForward)
	//		-(*))/(2*topDistance*inchesStraightForward));		//Law of Cosines again
			cameraData->setCameraData(robotX,robotZ,0,robotAngle);
		}*/
	}
};
#endif
