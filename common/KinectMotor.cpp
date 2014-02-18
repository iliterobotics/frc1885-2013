#include "KinectMotor.h"

// Standard includes
#include <iostream>

using namespace std;

KinectMotor::KinectMotor()
{
   m_isOpen = false;
}

KinectMotor::~KinectMotor()
{
   Close();
}

bool KinectMotor::Open()
{
   const XnUSBConnectionString *paths;
   XnUInt32 count;
   XnStatus res;

   // Init OpenNI USB
   res = xnUSBInit();
  /* if (res != XN_STATUS_OK) {
      xnPrintError(res, "xnUSBInit failed");
      return false;
   } else {
      cout << "Init OpenNI USB " << res << "\n";
   }*/

   // Open "Kinect motor" USB device
   res = xnUSBEnumerateDevices(0x045E /* VendorID */, 0x02B0 /* ProductID */, &paths, &count);
   if (res != XN_STATUS_OK) {
      xnPrintError(res, "xnUSBEnumerateDevices failed");
      return false;
   } else {
      cout << "Open Kinect Motor " << res << "\n";
   }

   // Open first found device
   res = xnUSBOpenDeviceByPath(paths[0], &m_dev);
   if (res != XN_STATUS_OK) {
      xnPrintError(res, "xnUSBOpenDeviceByPath failed");
      return false;
   } else {
      cout << "Open first device " << res << "\n";
   }

   return true;
}

void KinectMotor::Close()
{
   if (m_isOpen) {
      xnUSBCloseDevice(m_dev);
      m_isOpen = false;
   }
}

bool KinectMotor::GetStatus(KinectStatus &status)
{
   XnStatus res;
   XnUChar buf[10]; // output buffer

   // Send move control request
   XnUInt32 nBytes = 0;
   res = xnUSBReceiveControl(m_dev, XN_USB_CONTROL_TYPE_VENDOR, 0x32, 0x0, 0x0, buf, 10, &nBytes, 0);
   if (res != XN_STATUS_OK) {
      xnPrintError(res, "xnUSBSendControl failed");
      return false;
   } else {
      status.accel_x = ((uint16_t)buf[2] << 8) | buf[3];
      status.accel_y = ((uint16_t)buf[4] << 8) | buf[5];
      status.accel_z = ((uint16_t)buf[6] << 8) | buf[7];
      status.angle = (int8_t)buf[8];
      status.motorStatus = (MotorStatus)buf[9];
   }

   return true;
}

double KinectMotor::GetAngle()
{
   XnStatus res;
   XnUChar buf[10];

   XnUInt32 nBytes = 0;
   res = xnUSBReceiveControl(m_dev, XN_USB_CONTROL_TYPE_VENDOR, 0x32, 0x0, 0x0, buf, 10, &nBytes, 0);
   if(res != XN_STATUS_OK)
   {
      xnPrintError(res, "xnUSBSendControl failed");
      return 0.0;
   }
   else
      return (double)buf[8];
}
bool KinectMotor::SetAngle(uint16_t angle)
{
   
   XnStatus res;

   // Send move control request
   //res = xnUSBSendControl(m_dev, XN_USB_CONTROL_TYPE_VENDOR, 0x31, angle, 0x00, NULL, 0, 0); // DISABLED to prevent damage to kinect
   if (res != XN_STATUS_OK) {
      xnPrintError(res, "xnUSBSendControl failed");
      return false;
   }

   return true;
}
