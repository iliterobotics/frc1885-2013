package ilite.display.modules.display2012;

import java.awt.GridLayout;

import javax.swing.JLabel;
import javax.swing.JPanel;

public class DiagnosticOutput {
	
	JLabel mJoystickX;
	JLabel mJoystickY;
	
	JLabel mDriveTrain;
	JLabel mDriveTrainPower;
	JLabel mDriveTrainOffset;
	
	JLabel mEncoderDistance;
	JLabel mBallVelocity;
	JLabel mDeadzone;
	
	JLabel mGyro;
	JLabel mGyroBias;
	JLabel mGyroAngles;
	
	JLabel mShooter;
	JLabel mShooterPower;
	JLabel mShooterAngle;
	JLabel mShooterRPM;
	JLabel mShooterSpeed;
	JLabel mShooterOverride;
	
	JLabel mRobotCoordinates;
	JLabel mRobotCoordinatePair;
	
	JLabel mTopHoop;
	JLabel mTopHoopDetetction;
	JLabel mTopHoopCoordinates;
	
	JLabel mMidLeftHoop;
	JLabel mMidLeftHoopDetetction;
	JLabel mMidLeftHoopCoordinates;
	
	JLabel mMidRightHoop;
	JLabel mMidRightHoopDetetction;
	JLabel mMidRightHoopCoordinates;
	
	JLabel mBottomHoop;
	JLabel mBottomHoopDetetction;
	JLabel mBottomHoopCoordinates;
	
	JLabel mLimitSwitch;
	JLabel mFrontLimitSwitch;
	JLabel mBackLimitSwitch;
	
	public DiagnosticOutput(JPanel panel) {
		
		initializeLabels();
		addToPanel(panel);
		update();
	}
	
	void setLabel(JLabel label, float anVal) {
		label.setText(label.getName() + " --> " + anVal);
	}
	
	void setDTLabels() {
		mDriveTrain.setText("Drivetrain -->");
		mDriveTrainPower.setText("Power: (" + GlobalData2012.gmDriveLeftPower + ", " + GlobalData2012.gmDriveRightPower + ")");
		mDriveTrainOffset.setText("Drive Straight Offset: " + GlobalData2012.gmDriveStraightOffset);
	}
	
	void setHoopLabels(JLabel label1, JLabel label2, JLabel label3, boolean detection, int aXCoord, int aYCoord) {
		label1.setText(label1.getName() + " -->");
		label2.setText("Hoop Detected: " + detection);
		label3.setText("Hoop Coordinates: (" + aXCoord + ", " + aYCoord + ")");
	}
	
	void setGyroLabels() {
		mGyro.setText("Gyro -->");
		mGyroBias.setText("Bias: " + GlobalData2012.gmGyroBiasVoltage);
		mGyroAngles.setText("Angles Horiz/Vert: " + GlobalData2012.gmHorizontalGyroAngle + " / " + GlobalData2012.gmVerticalGyroAngle);
	}
	
	void setRobotCoordLabels() {
		mRobotCoordinates.setText("Robot Coordinates -->");
		mRobotCoordinatePair.setText("(" + GlobalData2012.gmRobotXCoord + ", " + GlobalData2012.gmRobotZCoord + ")");
	}
	
	void setShooterLabels() {
		mShooter.setText("Shooter -->");
		mShooterPower.setText("Power: " + GlobalData2012.gmShooterPower);
		mShooterAngle.setText("Angle: " + GlobalData2012.gmShooterAngle);
		mShooterRPM.setText("RPM Actual/Desired: " + GlobalData2012.gmActualShooterRPM + " / " + GlobalData2012.gmDesiredShooterRPM);
		mShooterSpeed.setText("Shooter Speed: " + GlobalData2012.gmShooterCamSpeed + " / " + GlobalData2012.gmShooterUserSpeed);
		mShooterOverride.setText("User Override: " + GlobalData2012.gmShooterSpeedOverride);
	}
	
	void setLimitSwitches() {
		mLimitSwitch.setText("Limit Switch -->");
		mFrontLimitSwitch.setText("Front Switch Up / Down: " + GlobalData2012.gmFrontLimitSwitchUp + " / " + GlobalData2012.gmFrontLimitSwitchDown);
		mBackLimitSwitch.setText("Back Switch Up / Down: " + GlobalData2012.gmBackLimitSwitchUp + " / " + GlobalData2012.gmBackLimitSwitchDown);
	}
	
	void update() {
		setLabel(mJoystickX, GlobalData2012.gmJoystickX);
		setLabel(mJoystickY, GlobalData2012.gmJoystickY);
		setLabel(mEncoderDistance, GlobalData2012.gmEncoderDistance);
		setLabel(mBallVelocity, GlobalData2012.gmComputedBallVelocity);
		setLabel(mDeadzone, GlobalData2012.gmDeadZone);
		
		setHoopLabels(mTopHoop, mTopHoopDetetction, mTopHoopCoordinates,  GlobalData2012.gmTopHoopDetected,  GlobalData2012.gmTopHoopXCoord,  GlobalData2012.gmTopHoopYCoord);
		setHoopLabels(mMidLeftHoop, mMidLeftHoopDetetction, mMidLeftHoopCoordinates, GlobalData2012.gmMidLeftHoopDetected,  GlobalData2012.gmMidLeftHoopXCoord,  GlobalData2012.gmMidLeftHoopYCoord);
		setHoopLabels(mMidRightHoop, mMidRightHoopDetetction, mMidRightHoopCoordinates, GlobalData2012.gmMidRightHoopDetected,  GlobalData2012.gmMidRightHoopXCoord,  GlobalData2012.gmMidRightHoopYCoord);
		setHoopLabels(mBottomHoop, mBottomHoopDetetction, mBottomHoopCoordinates, GlobalData2012.gmBottomHoopDetected,  GlobalData2012.gmBottomHoopXCoord,  GlobalData2012.gmBottomHoopYCoord);
		
		setDTLabels();
		setGyroLabels();
		setRobotCoordLabels();
		setShooterLabels();
		setLimitSwitches();
	}
	
	public void initializeLabels() {
		mJoystickX = new JLabel();
		mJoystickX.setName("Joy X");
		
		mJoystickY = new JLabel();
		mJoystickY.setName("Joy Y");
		
		mDriveTrain = new JLabel();
		mDriveTrainPower = new JLabel();
		mDriveTrainOffset = new JLabel();
		mDriveTrain.setName("DT Power");
		
		mGyro = new JLabel();
		mGyroBias = new JLabel();
		mGyroAngles = new JLabel();
		mGyro.setName("Gyro");
		
		mShooter = new JLabel();
		mShooter = new JLabel();
		mShooterPower = new JLabel();
		mShooterAngle = new JLabel();
		mShooterRPM = new JLabel();
		mShooterSpeed = new JLabel();
		mShooterOverride = new JLabel();
		mShooter.setName("Shooter");
		
		mEncoderDistance = new JLabel();
		mEncoderDistance.setName("Encoder Dist");
		
		mTopHoop = new JLabel();
		mTopHoopDetetction = new JLabel();
		mTopHoopCoordinates = new JLabel();
		mTopHoop.setName("Top Hoop");
		
		mMidLeftHoop = new JLabel();
		mMidLeftHoopDetetction = new JLabel();
		mMidLeftHoopCoordinates = new JLabel();
		mMidLeftHoop.setName("Mid-Left Hoop");
		
		mMidRightHoop = new JLabel();
		mMidRightHoopDetetction = new JLabel();
		mMidRightHoopCoordinates = new JLabel();
		mMidRightHoop.setName("Mid-Right Hoop");
		
		mBottomHoop = new JLabel();
		mBottomHoopDetetction = new JLabel();
		mBottomHoopCoordinates = new JLabel();
		mBottomHoop.setName("Bottom Hoop");
		
		mRobotCoordinates = new JLabel();
		mRobotCoordinatePair = new JLabel();
		mRobotCoordinates.setName("Robot Coordinates");
		
		mBallVelocity = new JLabel();
		mBallVelocity.setName("Ball Velocity");
		
		mDeadzone = new JLabel();
		mDeadzone.setName("Deadzone");
		
		mLimitSwitch = new JLabel();
		mFrontLimitSwitch = new JLabel();
		mBackLimitSwitch = new JLabel();
		mLimitSwitch.setName("Limit Switch");
	}
	
	public void addToPanel(JPanel acPanel) {
		acPanel.setLayout(new GridLayout(12, 3));
		
		acPanel.add(mJoystickX);
		acPanel.add(mJoystickY);
		acPanel.add(new JPanel());
		
		acPanel.add(mDriveTrain);
		acPanel.add(mDriveTrainPower);
		acPanel.add(mDriveTrainOffset);
		
		acPanel.add(mEncoderDistance);
		acPanel.add(mBallVelocity);
		acPanel.add(mDeadzone);
		
		acPanel.add(mGyro);
		acPanel.add(mGyroBias);
		acPanel.add(mGyroAngles);
		
		acPanel.add(mShooter);
		acPanel.add(mShooterPower);
		acPanel.add(mShooterAngle);
		acPanel.add(mShooterRPM);
		acPanel.add(mShooterSpeed);
		acPanel.add(mShooterOverride);
		
		acPanel.add(mRobotCoordinates);
		acPanel.add(mRobotCoordinatePair);
		acPanel.add(new JPanel());
		
		acPanel.add(mTopHoop);
		acPanel.add(mTopHoopDetetction);
		acPanel.add(mTopHoopCoordinates);
		
		acPanel.add(mMidLeftHoop);
		acPanel.add(mMidLeftHoopDetetction);
		acPanel.add(mMidLeftHoopCoordinates);
		
		acPanel.add(mMidRightHoop);
		acPanel.add(mMidRightHoopDetetction);
		acPanel.add(mMidRightHoopCoordinates);
		
		acPanel.add(mBottomHoop);
		acPanel.add(mBottomHoopDetetction);
		acPanel.add(mBottomHoopCoordinates);
		
		acPanel.add(mLimitSwitch);
		acPanel.add(mFrontLimitSwitch);
		acPanel.add(mBackLimitSwitch);
	}
}
