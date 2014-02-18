#ifndef _COORDINATE_TRANSFORM_H
#define _COORDINATE_TRANSFORM_H

class CoordinateTransform
{
	public:
		static void kinectToWorld(float x, float y, float z, float *out_x, float *out_y, float *out_z, float angle, float height);
		static void determineAngle(double x, double y, double z, double &thetaX, double &thetaY);
};

#endif
