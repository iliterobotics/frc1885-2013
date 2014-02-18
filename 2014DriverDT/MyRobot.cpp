#include "WPILib.h"

class RobotDemo : public SimpleRobot
{
	Joystick leftStick, rightStick;
	Victor dtLeftFront, dtRightFront, dtLeftBack, dtRightBack;

	bool arcadeDrive;
	float joy1x, joy1y, joy2x, joy2y;
	float dtLeftPower, dtRightPower;

	static const int arcadeToggle = 3, tankToggle = 5;

public:
	RobotDemo(void):
		leftStick(1),
		rightStick(2),
		dtLeftFront(1),
		dtRightFront(2),
		dtLeftBack(3),
		dtRightBack(4),
		arcadeDrive(true),
		joy1x(0.0),
		joy1y(0.0),
		joy2x(0.0),
		joy2y(0.0),
		dtLeftPower(0.0),
		dtRightPower(0.0)
	{
	}

	void Autonomous(void)	{}

	void OperatorControl(void)
	{
		while (IsOperatorControl())
		{
			getInput();
			processDriveTrain();
			outputToMotors();

			Wait(0.005);				// wait for a motor update time
		}
	}

	void getInput()
	{
		// Check for arcade/tank mode
		if (leftStick.GetRawButton(arcadeToggle)) 
		{
			arcadeDrive = true;
		}
		else if (leftStick.GetRawButton(tankToggle))
		{
			arcadeDrive = false;
		}

		// Get joystick inputs
		joy1x = leftStick.GetX(GenericHID::kLeftHand);
		joy1y = leftStick.GetY(GenericHID::kLeftHand);
		joy2x = rightStick.GetX(GenericHID::kRightHand);
		joy2y = rightStick.GetY(GenericHID::kRightHand);
	}

	void processDriveTrain ()
	{
		if (arcadeDrive)
		{
			if (joy2y > 0)
			{
				dtLeftPower = -joy2x - joy2y;
				dtRightPower = joy2y - joy2x;
			}
			else
			{
				dtLeftPower = joy2x - joy2y;
				dtRightPower = joy2x + joy2y;
			}
		}
		else
		{
			dtLeftPower = -joy1y;
			dtRightPower = joy2y;
		}
	}

	void outputToMotors()
	{
		dtLeftFront.SetSpeed(dtLeftPower);
		dtLeftBack.SetSpeed(dtLeftPower);
		dtRightFront.SetSpeed(dtRightPower);
		dtRightBack.SetSpeed(dtRightPower);
	}
};

START_ROBOT_CLASS(RobotDemo);

