#include <math.h>
#include "CoordinateTransform.h"

void CoordinateTransform::kinectToWorld(float x, float y, float z, float *out_x, float *out_y, float *out_z, float angle, float height)
{
	// Rotate the points about the X axis:
	float rads = (angle) * 3.1415f / 180.0f;
	float c = cosf(rads);
	float s = sinf(rads);

	*out_x = x;
	*out_y = c*z - s*y;
	*out_z = c*y + s*z;

	// Translate the point up in Z:
	*out_z += height;
}

/**
*
*/
void CoordinateTransform::determineAngle(double x, double y, double z, double &thetaX, double &thetaY)
{
	double commonSide = sqrt((z*z)-(y*y));

	thetaY = asinf(y/z);// * 180.0f / M_PI;

	//thetaX = (acos(((x*x) + (y*y) - (z*z)) / (2.0*x*y))) * (180 / M_PI);
	thetaX = asinf(x/commonSide); //* 180.0f / M_PI;
}



/**
* Calculates the degrees in the vertical direction needed to aim at the blob
* Need to send in the cloudY value and the depth to the blob
*/
/*
double CoordinateTransform::verticalDegrees(float cloudY, float depth)
{
	double degrees;

	degrees = sind(cloudY/depth);

	return degrees;
}
*/

/**
* Calculates the length of the side that both vertical and horizontal triangles share
* Z is depth
*/
/*
double CoordinateTransform::sameSide(float cloudY, float depth)
{
	double sideLength;

	sideLength = sqrt((depth*depth)-(cloudY*cloudY));

	return sideLength;
}
*/









