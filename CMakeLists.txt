cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(vision)

set(VISION_MAJOR_VERSION 0)
set(VISION_MINOR_VERSION 1)
set(VISION_PATCH_LEVEL 0)
set(VISION_VERSION
  ${VISION_MAJOR_VERSION}.${VISION_MINOR_VERSION}.${VISION_PATCH_VERSION})

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x -ggdb")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
find_package(OpenCV REQUIRED)
find_package(OpenNI REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${OPENNI_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/common
	${PROJECT_SOURCE_DIR}/streamer
	${PROJECT_SOURCE_DIR}/comm/cpp
)

add_subdirectory(comm/cpp)
add_subdirectory(common)
add_subdirectory(ultimateAscent)
add_subdirectory(tools)

# CMake Options
OPTION(BUILD_TOOLS "Build tool programs" ON)
OPTION(BUILD_TESTS "Build test programs" OFF)
OPTION(BUILD_ULTIMATE_ASCENT "Build ultimate ascent programs" ON)

# Tool executables
if (BUILD_TOOLS)
   add_executable (KinectServer tools/KinectServer.cpp)
   add_executable (OpenNiCapture tools/OpenNiCapture.cpp)
   add_executable (BasicKinectBlobs tools/BasicKinectBlobs.cpp)
   add_executable (TcpCameraClient tools/TcpCameraClient.cpp)
   add_executable (KinectMotor tools/KinectMotor.cpp)

   target_link_libraries(KinectServer ${OPENNI_LIBRARIES} ${OpenCV_LIBRARIES} common boost_system boost_thread-mt boost_signals-mt ecto_streamer_serv)
   target_link_libraries(OpenNiCapture ${OpenCV_LIBRARIES} common)
   target_link_libraries(BasicKinectBlobs ${OpenCV_LIBRARIES} common)
   target_link_libraries(TcpCameraClient ${OpenCV_LIBRARIES} common)
   target_link_libraries(KinectMotor OpenNI common)
endif()

# Test executables
if (BUILD_TESTS)
   add_executable (AnglesTest test/AnglesTest.cpp)
   add_executable (SocketTest test/SocketTest.cpp)
   add_executable (TimeTest test/TimeTest.cpp)
   add_executable (TcpServerTest test/TcpServerTest.cpp)
   add_executable (TcpClientTest test/TcpClientTest.cpp)
   #add_executable (UdpServerTest test/UdpServerTest.cpp)
   #add_executable (UdpClientTest test/UdpClientTest.cpp)
   add_executable (InputTest test/InputTest.cpp)

   target_link_libraries(AnglesTest common)
   target_link_libraries(SocketTest common)
   target_link_libraries(TimeTest common)
   target_link_libraries(TcpServerTest common pthread)
   target_link_libraries(TcpClientTest common pthread)
   #target_link_libraries(UdpServerTest common pthread)
   #target_link_libraries(UdpClientTest common pthread)
   target_link_libraries(InputTest common)
endif()

# Ultimate Ascent
if (BUILD_ULTIMATE_ASCENT)
   add_executable (UltimateAscent ultimateAscent/UltimateAscent.cpp)
   #add_executable (testing ultimateAscent/Testing/Kinect_Test.cpp)
   #add_executable (Kinect_Server ultimateAscent/Testing/Kinect_Server.cpp)
   #add_executable (Kinect_Client ultimateAscent/Testing/Kinect_Client.cpp)

   target_link_libraries(UltimateAscent OpenNI ${OpenCV_LIBRARIES} common comm boost_system boost_thread-mt boost_signals-mt ecto_streamer_serv)
   #target_link_libraries(testing ${OpenCV_LIBRARIES} common)
   #target_link_libraries(Kinect_Server ${OpenCV_LIBRARIES} common pthread)
   #target_link_libraries(Kinect_Client ${OpenCV_LIBRARIES} common pthread)
endif()

