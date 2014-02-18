#ifndef _HSV_FILTER_H
#define _HSV_FILTER_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/* 
This class provides thresholding using HSV color space
and filtering images by first filtering out pixels with
hsv not contained within the configured threshold and
then by removing blobs with insufficent area

References:
http://www.shervinemami.info/blobs.html#blobLibraries
*/

using namespace cv;

class CvHsvThreshold {
public:
	// Default Constructor
	CvHsvThreshold();
	CvHsvThreshold(Scalar pMinThreshold, Scalar pMaxThreshold, int pMinBlobArea);

	// Default destructor
	~CvHsvThreshold();
	
	// Sets the threshold scalar
	void setThreshold(Scalar pMinThreshold, Scalar pMaxThreshold);

	// Sets the minimum blob area
	void setMinBlobArea(int pMinBlobArea);

	// Performs thresholding on a bgr image
	void bgrThreshold(Mat &pBgrImage, Mat &pThresholdImage);

	// Performs thresholding on an hsv image
	void hsvThreshold(Mat &pHsvImage, Mat &pThresholdImage);

private:
	// Contains the range of hue, saturation, and value (HSV) for thresholding
	Scalar mMinThreshold;
	Scalar mMaxThreshold;

	// Contains the minimum blob area for thresholding
	int mMinBlobArea;

	// Performs blob area filtering
	void blobAreaFilter(Mat &pThresholdImage);
};

#endif
