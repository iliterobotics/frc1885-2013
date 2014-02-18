#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "BlobDetector.h"

#include <iostream>

using namespace cv;
using namespace std;

bool die = false;

int main(int argc, char* argv[]) {
   int imageMode = 0;

   VideoCapture capture(CV_CAP_OPENNI);
   if(!capture.isOpened()) {
      cout << "Can not open the Kinect\n";
      return 1;
   }

   if (!capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ)) {
      cout << "\nFailed to set image mode to CV_CAP_OPENNI_VGA_30HZ.\n\nexiting...\n";
      return 1;
   }

   // This might be used to aligned the depth and image generators (Need to figure this out...)
   //capture.set(CV_CAP_OPENNI_DEPTH_GENERATOR_REGISTRATION, 0);

   int hMin = 75;
   int hMax = 150;
   int sMin = 50;
   int sMax = 255;
   int vMin = 175;
   int vMax = 255;
   int aMin = 200;
   int epsilon = 50;

   cvNamedWindow("Threshold");
   cvCreateTrackbar("hMin", "Threshold", &hMin, 255, NULL);
   cvCreateTrackbar("hMax", "Threshold", &hMax, 255, NULL);
   cvCreateTrackbar("sMin", "Threshold", &sMin, 255, NULL);
   cvCreateTrackbar("sMax", "Threshold", &sMax, 255, NULL);
   cvCreateTrackbar("vMin", "Threshold", &vMin, 255, NULL);
   cvCreateTrackbar("vMax", "Threshold", &vMax, 255, NULL);
   cvCreateTrackbar("Area", "Threshold", &aMin, 640*480/100, NULL);
   cvCreateTrackbar("Epsilon", "Threshold", &epsilon, 100, NULL);

   // Display forever
   while(!die) {
      if(!capture.grab()) {
         cout << "Can not grab images." << endl;
         die = true;
      } else {
         Mat cloud, bgr, validDepth;
         bool capturedCloud = capture.retrieve(cloud, CV_CAP_OPENNI_POINT_CLOUD_MAP);
         bool capturedBgr = capture.retrieve(bgr, CV_CAP_OPENNI_BGR_IMAGE);
         if (capturedCloud && capturedBgr) {
            Mat feed, hsv, threshold;
            cvtColor(bgr, hsv, CV_BGR2HSV);
            inRange(hsv, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), threshold);
            bgr.copyTo(feed, threshold);

            vector<vector<Point> > blobs = BlobDetector::findBlobs(threshold, aMin, epsilon/10.);
            for (int i = 0; i < blobs.size(); i++) {
               drawContours(feed, blobs, i, Scalar(0, 255, 255));
            }

            imshow("Threshold", threshold);
            imshow("Kinect Feed", feed);
         }
      }

      // Do not remove this, otherwise the program will appear to not work at all...
      if(waitKey(15) >= 0) {
      }
   }

   cout << "Exiting...\n";
   return 0;
}

