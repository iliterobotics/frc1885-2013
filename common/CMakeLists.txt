add_library(common STATIC
      TimeUtils.cpp
      KinectWrapper.cpp
      HsvFilter.cpp
      Communicator.cpp
      BlobDetector.cpp
      SocketUtils.cpp
      KinectSubscriber.cpp
      CoordinateTransform.cpp
      InputUtils.cpp
      KinectMotor.cpp
)

target_link_libraries(common ${OpenCV_LIBRARIES} ${OPENNI_LIBRARIES} pthread)
