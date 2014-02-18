package ilite.display.modules.display2013.data;

import ilite.display.modules.display2013.CompetitionDisplay2013Control;

import java.util.Timer;
import java.util.TimerTask;

public class RobotData2013 {
	private static final double sDEFAULT_DOUBLE = -1d;
	private static final boolean sDEFAULT_BOOLEAN = false;
	private static final int sDEFAULT_INTEGER = -1;
	
	public long time = System.currentTimeMillis();
	public long mParseTimeMillis = sDEFAULT_INTEGER;

	public class Motors
	{
		public double mDrivetrainPowerLeft = sDEFAULT_DOUBLE;
		public double mDrivetrainPowerRight = sDEFAULT_DOUBLE;

		public double mTiltMotorPower = sDEFAULT_DOUBLE;
		public double mLaunchWheelMotorPower = sDEFAULT_DOUBLE;
		public double mRollerMotorPower = sDEFAULT_DOUBLE;
	}

	public class DriverInputs
	{
		public byte mGameState = sDEFAULT_INTEGER;

		public double mJoystick1_X = sDEFAULT_DOUBLE;
		public double mJoystick1_Y = sDEFAULT_DOUBLE;
		public double mJoystick2_X = sDEFAULT_DOUBLE;
		public double mJoystick2_Y = sDEFAULT_DOUBLE;

		public boolean mKinectSafetySwitchState = sDEFAULT_BOOLEAN;

		public byte mPresetControlSwitchState = sDEFAULT_INTEGER; // 0 or 1 or 2

		public boolean mHopperSwitchState = sDEFAULT_BOOLEAN;
		public boolean mFireButtonDepressed = sDEFAULT_BOOLEAN;
		public boolean mKickButtonDepressed = sDEFAULT_BOOLEAN;
		public boolean mHangSafetySwitchState = sDEFAULT_BOOLEAN;

		public double mManualSpeedPotValue = sDEFAULT_DOUBLE;
		public double mManualAnglePotValue = sDEFAULT_DOUBLE;
	}

	public class Pneumatics
	{
		public boolean mCompressorState = sDEFAULT_BOOLEAN;
		public boolean mHangLiftState = sDEFAULT_BOOLEAN; // Direct values of the solenoid
		public boolean mHopperState = sDEFAULT_BOOLEAN;
		public boolean mSpatulaState = sDEFAULT_BOOLEAN;
		public boolean mLaunchKickerState = sDEFAULT_BOOLEAN;
	}

	public class Kinect
	{
		public double mKinectAngle = sDEFAULT_DOUBLE;
		public double mKinectWheelSpeed = sDEFAULT_DOUBLE;
		public double mKinectBearing = sDEFAULT_DOUBLE;
	}

	public class Sensors
	{
		public int mDrivetrainLeftEncoderValue = sDEFAULT_INTEGER;
		public int mDrivetrainRightEncoderValue = sDEFAULT_INTEGER;

		public double mDrivetrainGyroValue = sDEFAULT_DOUBLE;      // 0 to 5V
		public double mTiltPotentiometerValue = sDEFAULT_DOUBLE;   // Represents 0 to 5V
		public int mPressureSensorValue = sDEFAULT_INTEGER;        // 0 or 1
	}

	public class ProgrammingDebug
	{
		//TODO Seth & the programming team may also add in some debug values for the
		// various statements within the code & state machine
	}

	public final Motors motors = new Motors();
	public final Sensors sensors = new Sensors();
	public final Kinect kinect = new Kinect();
	public final Pneumatics pneumatics = new Pneumatics();
	public final DriverInputs inputs = new DriverInputs();
	public final ProgrammingDebug debugs = new ProgrammingDebug();	

	public String toCSVString() {
		StringBuilder sb = new StringBuilder();
		sb.append(time).append(',');
		sb.append(motors.mDrivetrainPowerLeft + ",");
		sb.append(motors.mDrivetrainPowerRight + ",");
		sb.append(motors.mTiltMotorPower + ",");
		sb.append(motors.mLaunchWheelMotorPower + ",");
		sb.append(motors.mRollerMotorPower + ",");
		sb.append(inputs.mGameState + ",");
		sb.append(inputs.mJoystick1_X + ",");
		sb.append(inputs.mJoystick1_Y + ",");
		sb.append(inputs.mJoystick2_X + ",");
		sb.append(inputs.mJoystick2_Y + ",");
		sb.append(inputs.mKinectSafetySwitchState + ",");
		sb.append(inputs.mPresetControlSwitchState + ",");
		sb.append(inputs.mHopperSwitchState + ",");
		sb.append(inputs.mFireButtonDepressed + ",");
		sb.append(inputs.mKickButtonDepressed + ",");
		sb.append(inputs.mHangSafetySwitchState + ",");
		sb.append(inputs.mManualSpeedPotValue + ",");
		sb.append(inputs.mManualAnglePotValue + ",");
		sb.append(pneumatics.mCompressorState + ",");
		sb.append(pneumatics.mHangLiftState + ",");
		sb.append(pneumatics.mHopperState + ",");
		sb.append(pneumatics.mSpatulaState + ",");
		sb.append(pneumatics.mLaunchKickerState + ",");
		sb.append(kinect.mKinectAngle + ",");
		sb.append(kinect.mKinectWheelSpeed + ",");
		sb.append(kinect.mKinectBearing + ",");
		sb.append(sensors.mDrivetrainLeftEncoderValue + ",");
		sb.append(sensors.mDrivetrainRightEncoderValue + ",");
		sb.append(sensors.mDrivetrainGyroValue + ",");
		sb.append(sensors.mTiltPotentiometerValue + ",");
		sb.append(sensors.mPressureSensorValue);

				return sb.toString();
	}

	public static String toCSVStringHeader() {
		StringBuilder sb = new StringBuilder();
		sb.append("Match Time").append(",");
		sb.append("Left Drivetrain Motor Power" + ",");
		sb.append("Right Drivetrain Motor Power" + ",");
		sb.append("Tilt Motor Power" + ",");
		sb.append("Launcher Wheel Motor Power" + ",");
		sb.append("Roller Motor Power" + ",");
		sb.append("Game State" + ",");
		sb.append("First Jostick X" + ",");
		sb.append("First Jostick Y" + ",");
		sb.append("Second Jostick X" + ",");
		sb.append("Second Jostick Y" + ",");
		sb.append("Kinect Saftey Switch State" + ",");
		sb.append("Shooter Preset Switch State" + ",");
		sb.append("Hopper Switch State" + ",");
		sb.append("Fire Button Pressed" + ",");
		sb.append("ick Button Pressed" + ",");
		sb.append("Hanging Saftey Switch State" + ",");
		sb.append("Manual Speed Pot Value" + ",");
		sb.append("Manual Angle Pot Value" + ",");
		sb.append("Compressor State" + ",");
		sb.append("Hang Lift State" + ",");
		sb.append("Hopper State" + ",");
		sb.append("Spatula State" + ",");
		sb.append("Launcher Kicker State" + ",");
		sb.append("Kinect Angle" + ",");
		sb.append("Kinect Wheel Speed" + ",");
		sb.append("Kinect Bearing" + ",");
		sb.append("Left Drivetrain Motor Encoder Value" + ",");
		sb.append("Right Drivetrain Motor Encoder Value" + ",");
		sb.append("Drivetrain Gyro Value" + ",");
		sb.append("Launcher Tilt Potentiometer Value" + ",");
		sb.append("Pressure Sensor Value");

		return sb.toString();
	}

	private static double sTestAngle = 0.0;
	private static int sTestTimer = 0;
	public static void startDataDriver(final CompetitionDisplay2013Control pControl)
	{
		new Timer().scheduleAtFixedRate(new TimerTask()
		{
			public void run()
			{
				//				double r = Math.random();
				//				
				//				sTestAngle++;
				//				if(sTestAngle > 55)
				//				{
				//					sTestAngle = 10.0;
				//				}
				//
				//				sTestTimer = (int)(Math.random()*10)+1;
				//				RobotData data = new RobotData();
				//				data.sensors.mTiltPotentiometerValue = sTestAngle;
				//
				//				if(sTestTimer == 5)
				//				{
				//					data.pneumatics.mHangLiftState = true;
				//				}
				//				else if(sTestTimer == 3)
				//				{
				//					data.pneumatics.mSpatulaState =true;
				//				}
				//				else if(sTestTimer == 4)
				//				{
				//					data.pneumatics.mHopperState = true;
				//				}
				//				else
				//				{
				//					data.pneumatics.mLaunchKickerState = true;
				//				}
				//				pControl.addRobotData(data);

			}
		}, 2000, 200);
	}
}
