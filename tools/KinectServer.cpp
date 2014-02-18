#include "KinectWrapper.h"
#include "KinectSubscriber.h"
#include "TimeUtils.h"
#include "SocketUtils.h"

#include <mjpeg_server.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

#include <signal.h>

using namespace std;

class KinectDebug : public KinectSubscriber {
   public:
      KinectDebug() : lastUpdate(TimeUtils::currTimeD()), frameCount(0) {
      }

   protected:
      void doUpdate(KinectDataPtr dataPtr) {
         cout << "fps: " << 1/(TimeUtils::timeSince(lastUpdate)) << ", frame: " << ++frameCount << "\n";
         lastUpdate = TimeUtils::currTimeD();
      }

   private:
      double lastUpdate;
      int frameCount;
};

class KinectStreamer : public KinectSubscriber {
   public:
      KinectStreamer(std::string pAddr, std::string pPort, std::string pDocRoot, int pNumThreads=1, int pQuality=30, int pFps=30) : mQuality(pQuality), mSleepMs(1000./pFps) {
         mServerPtr = http::server::init_streaming_server(pAddr, pPort, pDocRoot, pNumThreads);
         mStream0Ptr = createStreamer(mServerPtr, "/stream_0"); // http://127.0.0.1/stream_0 (as an example)
         //mStream1Ptr = createStreamer(mServerPtr, "/stream_1"); // http://127.0.0.1/stream_1 (as an example)
         mServerPtr->start();
      }

      void setQuality(int pQuality) {
         boost::mutex::scoped_lock lock(mMutex);
         mQuality = pQuality;
      }

      void setFps(int pFps) {
         boost::mutex::scoped_lock lock(mMutex);
         mSleepMs = 1000./pFps;
      }

   protected:
      void doUpdate(KinectDataPtr dataPtr) {
         int quality, sleepMs;
         {
            boost::mutex::scoped_lock lock(mMutex);
            quality = mQuality;
            sleepMs = mSleepMs;
         }
         mStream0Ptr->post_image(dataPtr->bgr640x480, quality, false);
         //mStream1Ptr->post_image(dataPtr->bgr, quality, wait);
         //boost::this_thread::sleep(boost::posix_time::milliseconds(sleepMs));
      }

   private:
      http::server::streamer_ptr createStreamer(http::server::server_ptr pServerPtr, const std::string& pPath) {
         http::server::streamer_ptr streamerPtr(new http::server::streamer);
         register_streamer(pServerPtr, streamerPtr, pPath);
         return streamerPtr;
      }

      boost::mutex mMutex;
      int mQuality;
      int mSleepMs; //1000./fps

      http::server::server_ptr mServerPtr;
      http::server::streamer_ptr mStream0Ptr;
      //http::server::streamer_ptr mStream1Ptr;
};

int main() {
   KinectWrapper kinect;

   KinectDebug debug;
   debug.connect(&kinect);

   KinectStreamer streamer("0.0.0.0", "9090", ".");
   streamer.connect(&kinect);

   for(;;) {
      sleep(10);
   }

   return 0;
}

