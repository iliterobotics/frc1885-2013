package ilite.display.modules.display2013;

import ilite.display.StateData;
import ilite.display.interfaces.net.TCPClient;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.io.InputStream;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import org.ilite.comm.ChunkReader;
import org.ilite.comm.ChunkReaderException;
import org.ilite.comm.ChunkTransmission;

public class RobotMessageParser2013 extends TCPClient {
	private StateData mStateData;
	private CompetitionDisplay2013Control mController;
	private final Executor mParsingThreadPool = Executors.newSingleThreadExecutor();
	private final ILog mLog = Logger.createLog(RobotMessageParser2013.class);

	public RobotMessageParser2013(CompetitionDisplay2013Control pController, StateData pStateData) {
		mController = pController;
		mStateData = pStateData;
	}
	@Override
	public void parse(InputStream is) throws Exception {
		int currentChunkSize;
		ChunkReader currentChunk;

		currentChunk = ChunkTransmission.read(is);
		currentChunkSize = currentChunk.chunkSize();

		mLog.debug(currentChunkSize);

		updateBandwidth(currentChunkSize);

		processRobotChunk(currentChunk);
	}

	private void updateBandwidth(int pNumOfBytes)
	{
		mStateData.addIncomingData(pNumOfBytes);
	}

	private void processRobotChunk(final ChunkReader currentChunk) {
		mParsingThreadPool.execute(new Runnable() {

			@Override
			public void run() {
				try {
					long start = System.currentTimeMillis();
					RobotData2013 tempData = new RobotData2013();
					
					tempData.motors.mDrivetrainPowerLeft = currentChunk.nextDouble();
					tempData.motors.mDrivetrainPowerRight = currentChunk.nextDouble();
					
					tempData.motors.mTiltMotorPower = currentChunk.nextDouble();
					tempData.motors.mLaunchWheelMotorPower = currentChunk.nextDouble();
					tempData.motors.mRollerMotorPower = currentChunk.nextDouble();
					
					tempData.inputs.mGameState = currentChunk.nextByte();
					
					tempData.inputs.mJoystick1_X = currentChunk.nextDouble();
					tempData.inputs.mJoystick1_Y = currentChunk.nextDouble();
					tempData.inputs.mJoystick2_X = currentChunk.nextDouble();
					tempData.inputs.mJoystick2_Y = currentChunk.nextDouble();
					
					tempData.inputs.mKinectSafetySwitchState = currentChunk.nextBoolean();
					
					tempData.inputs.mPresetControlSwitchState = currentChunk.nextByte();
						
					tempData.inputs.mHopperSwitchState = currentChunk.nextBoolean();
					tempData.inputs.mFireButtonDepressed = currentChunk.nextBoolean();
					tempData.inputs.mKickButtonDepressed = currentChunk.nextBoolean();
					tempData.inputs.mHangSafetySwitchState = currentChunk.nextBoolean();
					
					tempData.inputs.mManualSpeedPotValue = currentChunk.nextDouble();
					tempData.inputs.mManualAnglePotValue = currentChunk.nextDouble();
					
					tempData.pneumatics.mCompressorState = currentChunk.nextBoolean();
					tempData.pneumatics.mHangLiftState = currentChunk.nextBoolean();
					tempData.pneumatics.mHopperState = currentChunk.nextBoolean();
					tempData.pneumatics.mSpatulaState = currentChunk.nextBoolean();
					tempData.pneumatics.mLaunchKickerState = currentChunk.nextBoolean();
					
					tempData.kinect.mKinectAngle = currentChunk.nextDouble();
					tempData.kinect.mKinectWheelSpeed = currentChunk.nextDouble();
					tempData.kinect.mKinectBearing = currentChunk.nextDouble();
					
					tempData.sensors.mDrivetrainLeftEncoderValue = currentChunk.nextInt();
					tempData.sensors.mDrivetrainRightEncoderValue = currentChunk.nextInt();
					
					tempData.sensors.mDrivetrainGyroValue = currentChunk.nextDouble();
					tempData.sensors.mTiltPotentiometerValue = currentChunk.nextDouble();
					tempData.sensors.mPressureSensorValue = currentChunk.nextInt();
					
					tempData.mParseTimeMillis = System.currentTimeMillis() - start;

//					mLog.info(tempData.toString());
					mController.addRobotData(tempData);
				} catch (ChunkReaderException e) {
					e.printStackTrace();
				}
			}
		});
	}
}