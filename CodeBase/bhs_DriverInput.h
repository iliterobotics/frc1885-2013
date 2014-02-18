#ifndef BHS_DRIVER_INPUT_H_
#define BHS_DRIVER_INPUT_H_

#include <list>

#include "DriverStation.h"
#include "Joystick.h"

#include "bhs_ButtonImpulse.h"
#include "bhs_Constants.h"
#include "bhs_GlobalData.h"
#include "bhs_Input.h"

using namespace std;

class bhs_DriverInput : public bhs_Input {
	public:
		// joysticks
		static const UINT32 JOYSTICK_DRIVER_1_PORT = 1;
		static const UINT32 JOYSTICK_DRIVER_2_PORT = 2;
		static const UINT32 JOYSTICK_OPERATOR_1_PORT = 3;
		static const UINT32 JOYSTICK_OPERATOR_2_PORT = 4;
		
		// driver buttons
		static const UINT32 BUTTON_USE_ARCADE_DRIVE = 6;
		static const UINT32 BUTTON_NUDGE_FORWARD = 8;
		static const UINT32 BUTTON_NUDGE_BACKWARD = 9;
		static const UINT32 BUTTON_NUDGE_LEFT = 8;
		static const UINT32 BUTTON_NUDGE_RIGHT = 9;
		
		// inputs channels
		// static const UINT32 _CHANNEL;
		// ...
		
		bhs_DriverInput(bhs_GlobalData*);
		virtual ~bhs_DriverInput();

		virtual void init();
		virtual void run();

	private:
		bhs_GlobalData* m_gd;

		Joystick m_joystick1;
		Joystick m_joystick2;
		Joystick m_buttonboard;

		bhs_ButtonImpulse m_impulseNudgeLeft;
		bhs_ButtonImpulse m_impulseNudgeRight;
		bhs_ButtonImpulse m_impulseNudgeForward;
		bhs_ButtonImpulse m_impulseNudgeBackward;
#if TEST_DRIVE_ANGLE_ANGLES
		bhs_ButtonImpulse* m_driveAngleImpulse;
#endif
};

#endif //BHS_DRIVER_INPUT_H_
