#ifndef _KINECT_WRAPPER_H_
#define _KINECT_WRAPPER_H_

#include <XnCppWrapper.h>

#include <boost/thread.hpp>
#include <boost/signal.hpp>
#include <boost/shared_ptr.hpp>

#include <opencv2/core/core.hpp>

typedef struct KinectData {
	/*
		~KinectData() {
		std::cout << "Deleted Kinect Data\n";
		}
		*/
	cv::Mat depth;
	cv::Mat rgb;
   cv::Mat bgr640x480;
   cv::Mat bgr; //320x240
   cv::Mat hsv; //320x240
} KinectData;

typedef boost::shared_ptr<KinectData> KinectDataPtr;

class KinectWrapper {
	public:
		typedef boost::signal<void (KinectDataPtr)> KinectSignal;
		typedef boost::signals::connection KinectConnection;

		KinectWrapper();
		~KinectWrapper();

		KinectConnection connect(KinectSignal::slot_function_type subscriber);
		void disconnect(KinectConnection subscriber);

	private:
		// Thread
		void start();
		void stop();
		boost::thread* mThread;
		bool mStopping;

		// Signals
		KinectSignal mSignal;

		// OpenNI
		xn::Context mContext;
		xn::DepthGenerator mDepthGenerator;
		xn::DepthMetaData mDepthMetaData;
		xn::ImageGenerator mImageGenerator;
		xn::ImageMetaData mImageMetaData;
};

#endif
