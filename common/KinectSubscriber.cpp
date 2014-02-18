#include "KinectSubscriber.h"

KinectSubscriber::KinectSubscriber() : mKinectWrapper(0) {
}

KinectSubscriber::~KinectSubscriber() {
   boost::mutex::scoped_lock lock(mUpdateMutex);
   disconnect();
}

void KinectSubscriber::connect(KinectWrapper *kinectWrapper) {
   if (mKinectWrapper == 0) {
      mKinectWrapper = kinectWrapper;
      mKinectConnection = mKinectWrapper->connect(boost::bind(&KinectSubscriber::onUpdate, this, _1));
   }
}

void KinectSubscriber::disconnect() {
   if (mKinectWrapper != 0) {
      mKinectWrapper->disconnect(mKinectConnection);
      mKinectWrapper = 0;
   }
}

void KinectSubscriber::onUpdate(KinectDataPtr dataPtr) {
   boost::thread t(boost::bind(&KinectSubscriber::update, this, dataPtr));
   t.detach();
}

void KinectSubscriber::update(KinectDataPtr dataPtr) {
   boost::mutex::scoped_lock lock(mUpdateMutex, boost::try_to_lock);
   if (lock) {
      doUpdate(dataPtr);
   } else {
      // Last update is still running...
   }
}

