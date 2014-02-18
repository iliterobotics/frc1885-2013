#ifndef _KINECT_SUBSCRIBER_H_
#define _KINECT_SUBSCRIBER_H_

#include <boost/thread.hpp>
#include <boost/signal.hpp>

#include "KinectWrapper.h"

class KinectSubscriber {
   public:
      KinectSubscriber();
      virtual ~KinectSubscriber();

      void connect(KinectWrapper *kinectWrapper);
      void disconnect();

   protected:
      virtual void doUpdate(KinectDataPtr dataPtr) = 0;

   private:
      // Kinect
      KinectWrapper *mKinectWrapper;
      KinectWrapper::KinectConnection mKinectConnection;

      // Update
      boost::mutex mUpdateMutex;
      void onUpdate(KinectDataPtr dataPtr);
      void update(KinectDataPtr dataPtr);
};

#endif
