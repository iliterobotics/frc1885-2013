#include "KinectMotor.h"

// Standard includes
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
   KinectMotor motor;
   KinectStatus status;

   if (!motor.Open()) // Open motor device
      return 1;

   cout << "Opened motor\n";

   motor.SetAngle(0);

   for (;;) {
      motor.GetStatus(status);
      if ((uint8_t)status.angle == 0x80) {
         cout << "Kinect angle: MAX\n";
      } else {
         cout << "Kinect angle: " << status.angle/2. << "\n";
      }
   }

   return 0;
}
