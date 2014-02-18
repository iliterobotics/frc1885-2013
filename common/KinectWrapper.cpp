#include "KinectWrapper.h"

#include <opencv2/imgproc/imgproc.hpp>

KinectWrapper::KinectWrapper() : mThread(0), mStopping(false) {
   XnStatus status;

   if ((status = mContext.Init()) != XN_STATUS_OK) {
      std::cout << "Failed to initialize the context: " << std::string(xnGetStatusString(status)) << std::endl;
      return;
   }

   if ((status = mDepthGenerator.Create(mContext)) != XN_STATUS_OK) {
      std::cout << "Failed to create depth generator: " << std::string(xnGetStatusString(status)) << std::endl;
      return;
   }

   if ((status = mImageGenerator.Create(mContext)) != XN_STATUS_OK) {
      std::cout << "Failed to create image generator: " << std::string(xnGetStatusString(status)) << std::endl;
      return;
   }

   //status = mDepthGenerator.SetMirror(true);

   //status = mImageGenerator.SetMirror(true);

   if ((status = mDepthGenerator.GetAlternativeViewPointCap().SetViewPoint(mImageGenerator)) != XN_STATUS_OK) {
      std::cout << "Failed to set depth viewpoint: " << std::string(xnGetStatusString(status)) << std::endl;
      return;
   }

   //status = mDepthGenerator.FrameSyncWith(mImageGenerator);

   if ((status = mContext.StartGeneratingAll()) != XN_STATUS_OK) {
      std::cout << "Failed to start generating data: " << std::string(xnGetStatusString(status)) << std::endl;
      return;
   }

   mThread = new boost::thread(boost::bind(&KinectWrapper::start, this));
}

KinectWrapper::~KinectWrapper() {
   stop();
   mContext.StopGeneratingAll();
   mContext.Release();
}

KinectWrapper::KinectConnection KinectWrapper::connect(KinectSignal::slot_function_type subscriber) {
   return mSignal.connect(subscriber);
}

void KinectWrapper::disconnect(KinectConnection subscriber) {
   subscriber.disconnect();
}

void KinectWrapper::start() {
   XnStatus status;
   do {
      if ((status = mContext.WaitAndUpdateAll()) == XN_STATUS_OK) {
         if (mDepthGenerator.IsValid() && mImageGenerator.IsValid()) {
            mDepthGenerator.GetMetaData(mDepthMetaData);
            mImageGenerator.GetMetaData(mImageMetaData);

            KinectDataPtr dataPtr(new KinectData);
            dataPtr->depth.create(mDepthMetaData.YRes(), mDepthMetaData.XRes(), CV_16UC1);
            dataPtr->rgb.create(mImageMetaData.YRes(), mImageMetaData.XRes(), CV_8UC3);

            memcpy(dataPtr->depth.data, mDepthMetaData.Data(), dataPtr->depth.total()*sizeof(XnDepthPixel));
            memcpy(dataPtr->rgb.data, mImageMetaData.RGB24Data(), dataPtr->rgb.total()*sizeof(XnRGB24Pixel));
            cv::cvtColor(dataPtr->rgb, dataPtr->bgr640x480, CV_RGB2BGR);
            //cv::pyrDown(dataPtr->bgr640x480, dataPtr->bgr);
            //cv::cvtColor(dataPtr->bgr, dataPtr->hsv, CV_RGB2HSV);

            mSignal(dataPtr);
         }
      }
   } while(status == XN_STATUS_OK && !mStopping);
}

void KinectWrapper::stop() {
   if (mThread != 0) {
      mStopping = true;
      mThread->join();
      mThread = 0;
   }
}

