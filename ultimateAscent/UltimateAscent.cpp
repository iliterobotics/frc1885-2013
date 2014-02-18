#define _USE_MATH_DEFINES
#define DEG_TO_RAD (M_PI/180.0f)
#define RAD_TO_DEG (180.0f/M_PI)
#define CLI_ROBOT
#define K4X
//#define DEBUG
//#define DEBUG_FEED
#define DRAW_ON_FEED

#include <iostream>
#include <fstream>
#include <math.h>
#include <mjpeg_server.hpp>

#include <stdio.h>
#include "signal.h"
#include "time.h"
#include "string.h"
#include "TimeUtils.h" //Possibly not necessary.
#include "SocketUtils.h" //Possibly not necessary.
#ifdef CLI_ROBOT
#include "InputUtils.h"
#endif

#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "BlobDetector.h"
#include "CoordinateTransform.h"
#include "Communicator.h"
#include "KinectMotor.h"

using namespace cv;
using namespace std;

struct Data
{
   int imageMode;
   int minH;
   int maxH;
   int minS;
   int maxS;
   int minV;
   int maxV;
   int minA;
   int maxA;
};

struct BlobValue
{
   double x;
   double y;
   double z;
   double thetaX;
   double thetaY;
};

void sighandler(int sig);
void retrieveImage(VideoCapture capture);
void compareThresh(Mat &pThresh, Mat &pDepth);
void readFromFile(ifstream &file, Data &pData);
void calculateRealWorld( double angle, double &depth, double &height );
//void calculateRealWorld( double angleY, double deviateY, double &depth, double &height );

bool die = false;
int kinectAngle = 12.5;

int main()
{
   bool connected = false;

   while (!connected) {
      FILE *in;
      char buf[512];
      if(!(in = popen("/usr/bin/lsusb", "r"))) {
         cerr << "Could not find lsusb." << endl;
         return 1;
      }

      while (fgets(buf, sizeof(buf), in)) {
         if (strstr(buf, "Camera")) {
            connected = true;
            cout << "Found a camera." << endl;
         }
      }
      pclose(in);
      sleep(0.5);

      if (!connected) {
         cerr << "Did not find a camera." << endl;
      }
   }

   VideoCapture capture;
   if(!capture.open(CV_CAP_OPENNI))
   {
      cout<<"Failed to open the Kinect, Cancelling..." << endl;
      return 0;
   }
   else
   {
      cout<< "Opened Kinect." << endl;
      retrieveImage(capture);
   }
   cout << "Exiting..." << endl;
   return 0;
}

void retrieveImage(VideoCapture capture)
{
   //Get Parameters from a file
   Data data;
   ifstream configFile;
   readFromFile(configFile, data);

	vector<vector<Point> > blobs;

   //Create Communicator
   Communicator communicator = Communicator();
   thread* th = communicator.init(2221);

   if(th == NULL)
   {
      cout<<"Connection not established... Exiting.\n";
      return;
   }

   //Signal Handlers
   signal(SIGABRT, &sighandler);
   signal(SIGTERM, &sighandler);
   signal(SIGINT, &sighandler);

   //Create Image Matrices
   Mat cameraImage;
   Mat cloudData;
   Mat depthData;

   //Initialize Kinect Motor
   KinectMotor kMotor = KinectMotor();
   KinectStatus status;
   kMotor.Open();

   //Create and Open Stream
   /*KinectWrapper kinect;

     KinectDebug debug;
     debug.connect(&kinect);

     KinectStreamer streamer("0.0.0.0", "9090", ".");
     streamer.connect(&kinect);
    */

   //Debug
   cout<<"minH: "<<data.minH<<", maxH: "<<data.maxH<<endl;
   cout<<"minS: "<<data.minS<<", maxS: "<<data.maxS<<endl;
   cout<<"minV: "<<data.minV<<", maxV: "<<data.maxV<<endl;
   cout<<"minA: "<<data.minA<<", maxA: "<<data.maxA<<endl;
   cout<< data.imageMode << endl;

   //Correct Values (kinect.config)
   /*
      minH 54
      maxH 100
      minS 0
      maxS 255
      minV 240
      maxV 255
      minA 400
      maxA 307200
      imageMode 0
    */

   //Create required streaming variables
   boost::mutex mMutex;
   //int mQuality;
   //int mSleepMs; //1000./fps

   const Mat& cameraPtr = cameraImage;
   http::server::server_ptr mServerPtr = http::server::init_streaming_server("0.0.0.0", "80", ".", 1);
   http::server::streamer_ptr streamerPtr(new http::server::streamer);
   register_streamer(mServerPtr, streamerPtr, "/stream_0");

   http::server::streamer_ptr mStream0Ptr = streamerPtr;

   mServerPtr->start();

   //Sets the mode for the camera
   bool modeRes = false;
   switch(data.imageMode)
   {
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
   if(!modeRes) //Error check for the desired imageMode;
   {
      cout << "\nThis image mode is not supported by the device, attempting to use CV_CAP_OPENNI_VGA_30HZ.\n\n";
      if (!capture.set(CV_CAP_OPENNI_IMAGE_GENERATOR_OUTPUT_MODE, CV_CAP_OPENNI_VGA_30HZ))
      {
         cout << "\nFailed to set image mode to CV_CAP_OPENNI_VGA_30HZ.\n\nexiting...\n";
         return;
      }
   }

   // Setup nonblocking input
#ifdef CLI_ROBOT
   InputUtils::setNonBlock(true);
#endif

   while(!die)
   {
      if(!capture.grab())
      {
         cout<< "Can not grab images." << endl;
         die = true;
      }
      else
      {
         //Retrieve the RGB Image and the Point Cloud Map
         capture.retrieve(cameraImage, CV_CAP_OPENNI_BGR_IMAGE);
         if(communicator.needsData()
#if defined(CLI_ROBOT) && defined(DEBUG)
               || InputUtils::hasInput()
#endif
           )
         {
#if defined(CLI_ROBOT) && defined(DEBUG)
            while (InputUtils::hasInput()) {
               fgetc(stdin);
            }
#endif

            capture.retrieve(cloudData, CV_CAP_OPENNI_POINT_CLOUD_MAP);
            capture.retrieve(depthData, CV_CAP_OPENNI_VALID_DEPTH_MASK);
#ifdef K4X
            kMotor.GetStatus(status);
#endif
            //Configure the Thresholds
            Scalar mMinThresh = Scalar(data.minH, data.minS, data.minV);
            Scalar mMaxThresh = Scalar(data.maxH, data.maxS, data.maxV);

            //Apply thresholds to a new Binary Image
            Mat threshold;
            Mat hsvImage;
            cvtColor(cameraImage, hsvImage, CV_BGR2HSV);
            inRange(hsvImage, mMinThresh, mMaxThresh, threshold);

#ifdef DEBUG_FEED
            Mat feed;
            cameraImage.copyTo(feed, threshold);
#endif


            //Detect Blobs
				blobs = BlobDetector::findBlobs(threshold, 200, 10.);
            vector<BlobValue> blobVals;

            for(int i = 0; i < blobs.size(); i++) {
               double overallX = 0, overallY = 0, overallZ = 0;
               int validPoints = 0;
               vector<Point> blob = blobs[i];

               for(int j = 0; j < blob.size(); j++) {
                  Vec3f depth = BlobDetector::getValidDepth(blob[j], cloudData);

                  if (depth[0] == 0.0f || depth[1] == 0.0f || depth[2] == 0.0f) {
                     continue;
                  } else {
                     validPoints++;
                     overallX += depth[0];
                     overallY += depth[1];
                     overallZ += depth[2];
                  }
               }

               if (validPoints == 4) {
                  overallX /= 4;
                  overallY /= 4;
                  overallZ /= 4;

                  //Computate the thetaX and ThetaY
                  double thetaX, thetaY;
                  CoordinateTransform::determineAngle(overallX, overallY, overallZ, thetaX, thetaY);

                  //Correct Data
                  calculateRealWorld( (double) kinectAngle, overallZ, overallY );

                  //Store the ThetaX and ThetaY
                  BlobValue bVal;
                  bVal.x = overallX;
                  bVal.y = overallY;
                  bVal.z = overallZ;
                  bVal.thetaX = thetaX * RAD_TO_DEG;
                  bVal.thetaY = thetaY * RAD_TO_DEG;
                  blobVals.push_back(bVal);
#ifdef DEBUG
                  //Print the coordinate values of the middle of the blob
                  cout << "X: " << overallX
                     << ", Y: " << overallY
                     << ", Z: " << overallZ
                     << ", X angle: " << bVal.thetaX
                     << ", Y angle: " << bVal.thetaY
                     << ", Kinect Angle: " << (double) kinectAngle << "\n";
#endif
#ifdef DEBUG_FEED
                  drawContours(feed, blobs, i, Scalar(0, 255, 255));
               } else {
                  drawContours(feed, blobs, i, Scalar(0, 255, 0));
#endif
               }
            }

            //If we actually have blobs
            if(blobVals.size() > 0)
            {
               //Find the highest blob
               int highIdx = 0;
               for(size_t i = 1; i < blobVals.size(); i++)
               {
                  BlobValue *bVal = &blobVals.at(i);
                  if(blobVals.at(highIdx).y > bVal->y)
                  {
                     highIdx = i;
                  }
               }
               //Send the highest blob to the robot.
               BlobValue bVal = blobVals.at(highIdx);
               communicator.setData(bVal.thetaY + kinectAngle, bVal.thetaX, 1.0);
            }
            else
            {
               cout<< "Cannot Find any Blobs" << endl;
               communicator.setData(0.0, 0.0, 0.0);
            }
#ifdef DEBUG_FEED
            imshow("Feed", feed);
            if(waitKey(5) >= 0) {
            }
#endif
			}

			//Post image to kinect stream.
			boost::mutex::scoped_lock lock(mMutex);
#ifdef DRAW_ON_FEED
			for(int i = 0; i < blobs.size(); i++) {
				drawContours(cameraPtr, blobs, i, Scalar(255, 0, 255));
			}
#endif
			mStream0Ptr->post_image(cameraPtr, 30, true); //->bgr640x480
		}
	}
}

//Read the configuration from the file
void readFromFile(ifstream &file, Data &pData)
{
   file.open("/usr/kinect.config");
   string pName;
   int value;
   while(file >> pName >> value)
   {
      if(pName.compare("minH") == 0)
      {
         pData.minH = value;
      }
      else if(pName.compare("maxH") == 0)
      {
         pData.maxH = value;
      }
      else if(pName.compare("minS") == 0)
      {
         pData.minS = value;
      }
      else if(pName.compare("maxS") == 0)
      {
         pData.maxS = value;
      }
      else if(pName.compare("minV") == 0)
      {
         pData.minV = value;
      }
      else if(pName.compare("maxV") == 0)
      {
         pData.maxV = value;
      }
      else if(pName.compare("minA") == 0)
      {
         pData.minA = value;
      }
      else if(pName.compare("maxA") == 0)
      {
         pData.maxA = value;
      }
      else if(pName.compare("imageMode") == 0)
      {
         pData.imageMode = value;
      }
      else
      {
         cout << "Configure error. No such configurable variable: " << pName << endl;
      }
   }
   file.close();
}
//Function for Signal Handling, includes the closing signal.
void sighandler(int sig)
{
   cout << "Handling ";
   switch(sig)
   {
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
   cout << "..." << endl;
   die = true;
}

//Function for real world values.
void calculateRealWorld( double angle, double &depth, double &height )
   // void calculateRealWorld( double angleY, double deviateY, double &depth, double &height )
{
   /*
      int W, V, OV;
      double a, H, O, realDepth, realHeight;

      W = 90 - (int) angleY;
      V = 90 - (int) deviateY - (int) angleY;
      OV = W - V;

      H = height / cos(OV);
      O = H*sin(OV);
      realDepth = (depth - O)*cos((int) deviateY);
      a = (depth - O)*sin((int) deviateY);
      realHeight = a + H;

      depth = realDepth;
      height = realHeight;
    */

   depth = depth*(cos(angle * DEG_TO_RAD));
   height = height + (depth*(sin(angle * DEG_TO_RAD)));
}
