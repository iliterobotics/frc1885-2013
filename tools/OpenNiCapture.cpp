#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "HsvFilter.h"

#include "signal.h"
#include <iostream>
#include <sstream>
#include <string>
#include "time.h"

using namespace cv;
using namespace std;

bool die = false;

void sighandler(int sig) {
	cout << "Handling ";
	switch(sig) {
		case SIGABRT:
			cout << "SIGABRT";
			break;
		case SIGTERM:
			cout << "SIGTERM";
			break;
		case SIGINT:
			cout << "SIGINT";
			break;
		default:
			cout << "UNKNOWN(" << sig << ")";
			break;
	}
	cout << "...\n";
	die = true;
}

struct onMouseCameraParam {
	Mat *image;
	bool isRGB;
};

struct onMouseDisparityParam {
	Mat *image;
	bool disp32;
};

static void colorizeDisparity(const Mat& gray, Mat& rgb, double maxDisp=-1.f,
		float S=1.f, float V=1.f) {
	CV_Assert(!gray.empty());
	CV_Assert(gray.type() == CV_8UC1);

	if(maxDisp <= 0) {
		maxDisp = 0;
		minMaxLoc(gray, 0, &maxDisp);
	}

	rgb.create(gray.size(), CV_8UC3);
	rgb = Scalar::all(0);
	if(maxDisp < 1) {
		return;
	}

	for(int y = 0; y < gray.rows; y++) {
		for(int x = 0; x < gray.cols; x++) {
			uchar d = gray.at<uchar>(y,x);
			unsigned int H =((uchar)maxDisp - d) * 240 /(uchar)maxDisp;

			unsigned int hi =(H/60) % 6;
			float f = H/60.f - H/60;
			float p = V *(1 - S);
			float q = V *(1 - f * S);
			float t = V *(1 -(1 - f) * S);

			Point3f res;

			switch(hi) {
				case 0: //R = V,  G = t,  B = p
					res = Point3f(p, t, V);
					break;
				case 1: // R = q, G = V,  B = p
					res = Point3f(p, V, q);
					break;
				case 2: // R = p, G = V,  B = t
					res = Point3f(t, V, p);
					break;
				case 3: // R = p, G = q,  B = V
					res = Point3f(V, q, p);
					break;
				case 4: // R = t, G = p,  B = V
					res = Point3f(V, p, t);
					break;
				case 5: // R = V, G = p,  B = q
					res = Point3f(q, p, V);
					break;
			}

			using std::min;
			using std::max;
			uchar b =(uchar)(max(0.f, min(res.x, 1.f)) * 255.f);
			uchar g =(uchar)(max(0.f, min(res.y, 1.f)) * 255.f);
			uchar r =(uchar)(max(0.f, min(res.z, 1.f)) * 255.f);

			rgb.at<Point3_<uchar> >(y,x) = Point3_<uchar>(b, g, r);
		}
	}
}

static float getMaxDisparity(VideoCapture& capture) {
	const int minDistance = 400; // mm
	float b =(float)capture.get(CV_CAP_OPENNI_DEPTH_GENERATOR_BASELINE); // mm
	float F =(float)capture.get(CV_CAP_OPENNI_DEPTH_GENERATOR_FOCAL_LENGTH); // pixels
	return b * F / minDistance;
}

static void parseCommandLine(int argc, char* argv[], bool& isRGB,
		bool& isColorizedDisp, bool& isFixedMaxDisp, bool& disp32,
		int& imageMode, string& readFilename, bool& isFileReading,
		string& writeFilename, bool &camera, bool &autoDetect,
		bool &cloud, bool &depth, bool &disparity, bool &validDepth,
		bool &detect) {
	// set defaut values
	isRGB = false;
	isColorizedDisp = false;
	isFixedMaxDisp = true;
	disp32 = false;
	imageMode = 0;
	readFilename.clear();
	isFileReading = false;
	writeFilename.clear();
	camera = false;
	autoDetect = false;
	cloud = false;
	depth = false;
	disparity = false;
	validDepth = false;
	detect = false;

	if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			if(!strcmp(argv[i], "--help") || !strcmp( argv[i], "-h")) {
				cout << "\nThis program demonstrates usage of depth sensors(Kinect, XtionPRO,...).\n"
				     << "The user gets some of the supported output images.\n"
				     << "\nAll supported output map types:\n"
				     << "1) Data given from depth generator\n"
				     << "   OPENNI_DEPTH_MAP            - depth values in mm(CV_16UC1)\n"
				     << "   OPENNI_POINT_CLOUD_MAP      - XYZ in meters(CV_32FC3)\n"
				     << "   OPENNI_DISPARITY_MAP        - disparity in pixels(CV_8UC1)\n"
				     << "   OPENNI_DISPARITY_MAP_32F    - disparity in pixels(CV_32FC1)\n"
				     << "   OPENNI_VALID_DEPTH_MASK     - mask of valid pixels(not ocluded, not shaded etc.)(CV_8UC1)\n"
				     << "2) Data given from RGB image generator\n"
				     << "   OPENNI_BGR_IMAGE            - color image(CV_8UC3)\n"
				     << "   OPENNI_GRAY_IMAGE           - gray image(CV_8UC1)\n\n"
				     << "Command Line Parameters:\n";
				cout << "--help, -h     Display help information\n";
				cout << "--rgb          Use the color image from the camera\n";
				cout << "--colorize     Colorize the disparity image\n";
				cout << "--fixed, -f    Fixed max disparity? (0/1, Default = 1)\n";
				cout << "--disp32       Uses OPENNI_DISPARITY_MAP_32F instead of OPENNI_DISPARITY_MAP\n";
				cout << "--mode, -m     image mode, resolution and fps (Default = 0)\n"
				     << "                0 - CV_CAP_OPENNI_VGA_30HZ (Kinect/XtionPRO)\n"
				     << "                1 - CV_CAP_OPENNI_SXGA_15HZ (Kinect/XtionPRO)\n"
				     << "                2 - CV_CAP_OPENNI_SXGA_30HZ (Kinect/XtionPRO)\n"
				     << "                3 - CV_CAP_OPENNI_QVGA_30HZ (XtionPRO)\n"
				     << "                4 - CV_CAP_OPENNI_QVGA_60HZ (XtionPRO)\n";
				cout << "--read, -r     Filename of the .oni video file to read grabbed data from.\n";
				cout << "--camera       Display the camera image.\n";
				cout << "--auto         Auto-detect something.\n";
				cout << "--cloud        Display the cloud image.\n";
				cout << "--depth        Display the depth image.\n";
				cout << "--disparity    Display the disparity image.\n";
				cout << "--valid        Display the valid depth image.\n";
				cout << "--detect       Demo of hsv thresholding.\n";
				cout << "\nExample:\n " << argv[0]
			        << "--rgb --colorize --fixed 0 --disp32 --mode 2 --read <file1> --write <file2> --camera --cloud --depth --disparity --valid\n";
				exit(0);
			} else if(!strcmp(argv[i], "--rgb")) {
				isRGB = true;
			} else if(!strcmp(argv[i], "--colorize")) {
				isColorizedDisp = true;
			} else if(!strcmp(argv[i], "--fixed") || !strcmp(argv[i], "-f")) {
				isFixedMaxDisp = atoi(argv[++i]) == 0 ? false : true;
			} else if(!strcmp(argv[i], "--disp32")) {
				disp32 = true;
			} else if(!strcmp(argv[i], "--mode") || !strcmp(argv[i], "-m")) {
				imageMode = atoi(argv[++i]);
			} else if(!strcmp(argv[i], "--read") || !strcmp(argv[i], "-r")) {
				readFilename = argv[++i];
				isFileReading = true;
			} else if(!strcmp(argv[i], "--camera")) {
				camera = true;
			} else if(!strcmp(argv[i], "--auto")) {
				autoDetect = true;
			} else if(!strcmp(argv[i], "--cloud")) {
				cloud = true;
			} else if(!strcmp(argv[i], "--depth")) {
				depth = true;
			} else if(!strcmp(argv[i], "--disparity")) {
				disparity = true;
			} else if(!strcmp(argv[i], "--valid")) {
				validDepth = true;
			} else if(!strcmp(argv[i], "--detect")) {
				detect = true;
			} else {
				cout << "Unsupported command line argument: " << argv[i] << ".\n";
				exit(-1);
			}
		}
	}
}

void onCloudMouse(int event, int x, int y, int flags, void* param) {
	Mat *image = (Mat*)param;
	ostringstream oss;
	Vec3f cloudPoint = image->at<Vec3f>(y, x);
	oss << "x=" << cloudPoint[0]
		<< " y=" << cloudPoint[1]
		<< " z=" << cloudPoint[2]
		<< " (meters)";
	displayOverlay("cloud", oss.str(), 0);
}

void onDepthMouse(int event, int x, int y, int flags, void* param) {
	Mat *image = (Mat*)param;
	ostringstream oss;
	short depthPoint = image->at<short>(y, x);
	if (depthPoint == 0) {
		oss << "Invalid Depth";
	} else {
		oss << "depth =" << depthPoint << " (millimeters)";
	}
	displayOverlay("depth", oss.str(), 0);
}

void onDisparityMouse(int event, int x, int y, int flags, void* param) {
	//onMouseDisparityParam *disparityParam = (onMouseDisparityParam*)param;
	//Mat *image = disparityParam->image;
	//bool disp32 = disparityParam->disp32;
	//ostringstream oss;
	//displayOverlay("disparity", oss.str(), 0);
}

void onValidDepthMouse(int event, int x, int y, int flags, void* param) {
	Mat *image = (Mat*)param;
	ostringstream oss;
	uchar validDepthPoint = image->at<uchar>(y, x);
	if (validDepthPoint == 0) {
		oss << "Invalid";
	} else {
		oss << "Valid";
	}
	displayOverlay("valid depth", oss.str(), 0);
}

void onCameraMouse(int event, int x, int y, int flags, void* param) {
	onMouseCameraParam *cameraParam = (onMouseCameraParam*)param;
	Mat *image = cameraParam->image;
	bool isRGB = cameraParam->isRGB;
	ostringstream oss;
	if (isRGB) {
		Vec3b colorPoint = image->at<Vec3b>(y, x);
		oss << hex << "0x" << (int)colorPoint[2] << (int)colorPoint[1] << (int)colorPoint[0]
			<< " (rgb hex)";
	} else {
		uchar grayPoint = image->at<uchar>(y, x);
		oss << (grayPoint/(float)0xFF) << " intensity";
	}
	displayOverlay("camera", oss.str(), 0);
}

/*
 * To work with Kinect or XtionPRO the user must install OpenNI library and PrimeSensorModule for OpenNI and
 * configure OpenCV with WITH_OPENNI flag is ON(using CMake).
 */
int main(int argc, char* argv[]) {
	int imageMode;
	bool isRGB;
	bool isColorizedDisp;
	bool isFixedMaxDisp;
	bool disp32;
	bool camera;
	bool autoDetect;
	bool cloud;
	bool depth;
	bool disparity;
	bool validDepth;
	bool detect;
	string readFilename;
	bool isVideoReading;
	string writeFilename;
	parseCommandLine(argc, argv, isRGB, isColorizedDisp, isFixedMaxDisp, disp32,
			imageMode, readFilename, isVideoReading, writeFilename, camera,
			autoDetect, cloud, depth, disparity, validDepth, detect);

	if (!(camera || cloud || depth || disparity || validDepth || autoDetect)) {
		cout << "No camera modes active, please use at least one of the following"
			"arguments: --camera, --auto, --cloud, --depth, --disparity, --valid\n";
		return 1;
	}

	VideoCapture capture;
	if(isVideoReading) {
		capture.open(readFilename);
		cout << "Opened " << readFilename << "\n";
	} else if (camera) {
		capture.open(CV_CAP_OPENNI);
		//cout << "Opened OpenNI device\n";
		cout << "Opened Kinect\n";
	} else {
		capture.open(CV_CAP_ANY);
		cout << "Opened something\n";
	}

	if(!capture.isOpened()) {
		cout << "Can not open a capture object.\n";
		return 1;
	}

	if(!isVideoReading) {
		bool modeRes=false;
		switch(imageMode) {
			case 0: // This should be used exclusively...
				modeRes = capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ);
				break;
			case 1: // Newer Kinects may not support this (older kinect for xbox should)
				modeRes = capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_SXGA_15HZ);
				break;
			case 2: // Newer Kinects may not support this (older kinect for xbox should)
				modeRes = capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_SXGA_30HZ);
				break;
			case 3: // Supported by the Xtion Pro Live
				modeRes = capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_QVGA_30HZ);
				break;
			case 4: // Supported by the Xtion Pro Live
				modeRes = capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_QVGA_60HZ);
				break;
			default:
				CV_Error(CV_StsBadArg, "Unsupported image mode property.\n");
				break;
		}
		if(!modeRes) {
			cout << "\nThis image mode is not supported by the device, attempting to use CV_CAP_OPENNI_VGA_30HZ.\n\n";
			if (!capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ)) {
				cout << "\nFailed to set image mode to CV_CAP_OPENNI_VGA_30HZ.\n\nexiting...\n";
				return 1;
			}
		}
	}

	signal(SIGABRT, &sighandler);
	signal(SIGTERM, &sighandler);
	signal(SIGINT, &sighandler);

	// Create image matrices
	Mat cloudImage;
	Mat depthImage;
	Mat disparityImage;
	Mat validDepthImage;
	Mat cameraImage;
	Mat detectImage;

	// detect range values
        int hMin = 0;
        int hMax = 255;
        int sMin = 0;
        int sMax = 255;
        int vMin = 0;
        int vMax = 255;
	CvHsvThreshold hsvThreshold;

	// Create image wrappers, windows, etc.
	if (cloud) {
		cvNamedWindow("cloud");
		cvSetMouseCallback("cloud", onCloudMouse, (void*)&cloudImage);
	}
	if (depth) {
		cvNamedWindow("depth");
		cvSetMouseCallback("depth", onDepthMouse, (void*)&depthImage);
	}
	if (disparity) {
		onMouseDisparityParam disparityParam = { &disparityImage, disp32 };
		cvNamedWindow("disparity");
		cvSetMouseCallback("disparity", onDisparityMouse, (void*)&disparityParam);
	}
	if (validDepth) {
		cvNamedWindow("valid depth");
		cvSetMouseCallback("valid depth", onValidDepthMouse, (void*)&validDepthImage);
	}
	if (camera) {
		onMouseCameraParam cameraParam = { &cameraImage, isRGB };
		cvNamedWindow("camera");
		cvSetMouseCallback("camera", onCameraMouse, (void*)&cameraParam);
	}
	if (detect) {
		cvNamedWindow("detect");
		cvCreateTrackbar("hMin", "detect", &hMin, 255, NULL);
		cvCreateTrackbar("hMax", "detect", &hMax, 255, NULL);
		cvCreateTrackbar("sMin", "detect", &sMin, 255, NULL);
		cvCreateTrackbar("sMax", "detect", &sMax, 255, NULL);
		cvCreateTrackbar("vMin", "detect", &vMin, 255, NULL);
		cvCreateTrackbar("vMax", "detect", &vMax, 255, NULL);
	}

	bool saveImage = false;
	while(!die) {
		if(!capture.grab()) {
			cout << "Can not grab images." << endl;
			die = true;
		} else {
			if(cloud && capture.retrieve(cloudImage, CV_CAP_OPENNI_POINT_CLOUD_MAP)) {
				imshow("cloud", cloudImage);
			}
			if(depth && capture.retrieve(depthImage, CV_CAP_OPENNI_DEPTH_MAP)) {
				const float scaleFactor = 0.05f;
				Mat altDepthImage;
				depthImage.convertTo(altDepthImage, CV_8UC1, scaleFactor);
				imshow("depth", altDepthImage);
			}
			if(disparity && capture.retrieve(disparityImage, CV_CAP_OPENNI_DISPARITY_MAP)) {
				if(isColorizedDisp) {
					Mat colorDisparityImage;
					colorizeDisparity(disparityImage, colorDisparityImage, isFixedMaxDisp ? getMaxDisparity(capture) : -1);
					Mat validColorDisparityImage;
					colorDisparityImage.copyTo(validColorDisparityImage, disparityImage != 0);
					imshow("disparity", validColorDisparityImage);
				} else {
					imshow("disparity", disparityImage);
				}
			}
			if(validDepth && capture.retrieve(validDepthImage, CV_CAP_OPENNI_VALID_DEPTH_MASK)) {
				imshow("valid depth", validDepthImage);
			}
			if(camera && capture.retrieve(cameraImage, isRGB ? CV_CAP_OPENNI_BGR_IMAGE : CV_CAP_OPENNI_GRAY_IMAGE)) {
				imshow("camera", cameraImage);
				if (isRGB && detect) {
					hsvThreshold.setThreshold(Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax));
					hsvThreshold.bgrThreshold(cameraImage, detectImage);
					imshow("detect", detectImage);
				}
				//if (saveImage) {
				//	IplImage cameraIplImage((IplImage)cameraImage);
				//	cvSaveImage("saved.jpg", &cameraIplImage);
				//	saveImage = false;
				//}
			}
		}

		// Do not remove this, otherwise the program will appear to not work at all...
		if(waitKey(10) >= 0) { // 10 ms, 100 Hz
			saveImage = true;
		}
	}

	cout << "Exiting...\n";
	return 0;
}

