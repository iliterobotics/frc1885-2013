#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "HsvFilter.h"

CvHsvThreshold::CvHsvThreshold()
	: mMinThreshold(Scalar(0,0,0)),
	mMaxThreshold(Scalar(255,255,255)),
	mMinBlobArea(0) {
}

CvHsvThreshold::CvHsvThreshold(Scalar pMinThreshold, Scalar pMaxThreshold, int pMinBlobArea)
	: mMinThreshold(pMinThreshold),
	mMaxThreshold(pMaxThreshold),
	mMinBlobArea(pMinBlobArea) {
}

CvHsvThreshold::~CvHsvThreshold() {
}

void CvHsvThreshold::setThreshold(Scalar pMinThreshold, Scalar pMaxThreshold) {
	mMinThreshold = pMinThreshold;
	mMaxThreshold = pMaxThreshold;
}

void CvHsvThreshold::setMinBlobArea(int pMinBlobArea) {
	mMinBlobArea = pMinBlobArea;
}

void CvHsvThreshold::bgrThreshold(Mat &pBgrImage, Mat &pThresholdImage) {
	Mat hsvImage;
	cvtColor(pBgrImage, hsvImage, CV_BGR2HSV);
	hsvThreshold(hsvImage, pThresholdImage);
}

void CvHsvThreshold::hsvThreshold(Mat &pHsvImage, Mat &pThresholdImage) {
	inRange(pHsvImage, mMinThreshold, mMaxThreshold, pThresholdImage);
	blobAreaFilter(pThresholdImage);
}

void CvHsvThreshold::blobAreaFilter(Mat &pThresholdImage) {
}
