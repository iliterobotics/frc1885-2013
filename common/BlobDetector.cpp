#include "BlobDetector.h"
#include <iterator>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

namespace {
	bool isValid(int y, int x, Mat &mat) {
		return y >= 0 && x >= 0 &&
			y < mat.rows && x < mat.cols;
	}

	bool isValid(Point &pnt, Mat &mat) {
		return isValid(pnt.y, pnt.x, mat);
	}
}

vector<vector<Point> > BlobDetector::findBlobs(Mat &binaryImage, int pMinArea, double pEpsilon)
{
   vector<vector<Point> > contours;
   findContours(binaryImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
   vector<vector<Point> > hull(contours.size());
   vector<vector<Point> > poly(contours.size());
   vector<vector<Point> > blobs;

   for(size_t i = 0; i < contours.size(); i++)
   {
      if(contours[i].size() < pMinArea)
      {
         continue;
      }

      convexHull(Mat(contours[i]), hull[i]);
      approxPolyDP(Mat(hull[i]), poly[i], pEpsilon, true);

      if (poly[i].size() == 4) {
         blobs.push_back(poly[i]);
      }
   }

   return blobs;
}

//Make the point have a valid depth
Vec3f BlobDetector::getValidDepth(Point &pPnt, Mat &pCloud, int step)
{
	Vec3f depth = pCloud.at<Vec3f>(pPnt.y, pPnt.x);

	// Already valid
	if (depth[2] > 0.0f)
	{
		return depth;
	}

	// Checks points like this:
	// - - -  |  . - -  |  . - -  |  . . -  |       |  . . .  |  . . .
	// - x -  |  - x -  |  - x -  |  - x -  |  ...  |  - x .  |  . x .
	// - - -  |  - - -  |  . - -  |  . - -  |       |  . . .  |  . . .
	// with squares of increasing radius
	for (int radius = step; radius < 50 && depth[2] == 0.0f; radius+=step)
	{
		// Floor and ceiling
		for (int x = radius; x > -radius && depth[2] == 0.0f; x-=step)
		{
			int x_abs = pPnt.x + x;
			int y_upper = pPnt.y - radius;
			int y_lower = pPnt.y + radius;
			if (isValid(y_upper, x_abs, pCloud))
				depth = pCloud.at<Vec3f>(y_upper, x_abs);
			if (depth[2] == 0.0f && isValid(y_lower, x_abs, pCloud))
				depth = pCloud.at<Vec3f>(y_lower, x_abs);
		}

		// Left and right walls
		for (int y = radius - 1; y > -radius + 1 && depth[2] == 0.0f; y-=step)
		{
			int x_right = pPnt.x + radius;
			int x_left = pPnt.x - radius;
			int y_abs = pPnt.y + y;
			if (isValid(y_abs, x_right, pCloud))
				depth = pCloud.at<Vec3f>(y_abs, x_right);
			if (depth[2] == 0.0f && isValid(y_abs, x_left, pCloud))
				depth = pCloud.at<Vec3f>(y_abs, x_left);
		}
	}

	// May still be 0.0
	return depth;
}
