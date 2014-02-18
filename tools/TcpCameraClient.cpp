#include "opencv2/highgui/highgui.hpp"
#include "SocketUtils.h"
#include "TimeUtils.h"

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
   int sockFd = SocketUtils::createTcpClient("127.0.0.1", 1234);
   bool first = true;
   double lastTime;
   if (sockFd < 0) {
   } else {
      Mat image(480, 640, CV_8UC3);
      while(true) {
         if(SocketUtils::tcpRead(sockFd, (char*)image.data, image.step*image.rows, 0, -1) < 0) {
            break;
         } else {
            if (!first) {
               cout << "Updated image, refresh rate: " << 1./(TimeUtils::timeSince(lastTime)) << "\n";
            } else {
               first = false;
            }
            lastTime = TimeUtils::currTimeD();
            imshow("Kinect Feed", image);
         }

         // Do not remove this, otherwise the program will appear to not work at all...
         if(waitKey(10) >= 0) {
         }
      }
   }
}

