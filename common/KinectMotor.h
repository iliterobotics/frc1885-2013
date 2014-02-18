#ifndef _KINECT_MOTOR_H_
#define _KINECT_MOTOR_H_

// OpenNI includes
#include <XnUSB.h>

// Standard includes
#include <stdint.h>

typedef enum {
   MOTOR_STATUS_STOPPED = 0x00,
   MOTOR_STATUS_LIMIT = 0x01,
   MOTOR_STATUS_MOVING = 0x04
} MotorStatus;

typedef struct {
   int16_t accel_x;
   int16_t accel_y;
   int16_t accel_z;
   int8_t angle;
   MotorStatus motorStatus;
} KinectStatus;

/**
 * Class to control Kinect's motor.
 */
class KinectMotor
{
   public:
      KinectMotor();
      virtual ~KinectMotor();

      /**
       * Open device.
       * @return true if succeeded, false - otherwise
       */
      bool Open();

      /**
       * Close device.
       */
      void Close();

      /**
       * Move motor up or down to specified angle value.
       * @param angle angle value
       * @return true if succeeded, false - otherwise
       */
      bool SetAngle(uint16_t angle);

      bool GetStatus(KinectStatus &status);

      double GetAngle();

   private:
      XN_USB_DEV_HANDLE m_dev;
      bool m_isOpen;
};

#endif
