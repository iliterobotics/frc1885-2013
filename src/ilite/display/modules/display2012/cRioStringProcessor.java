package ilite.display.modules.display2012;
import ilite.util.lang.IUpdate;

import java.nio.ByteBuffer;

public class cRioStringProcessor implements IUpdate<byte[]>
{

	@Override
	public void update(byte[] messageInput) {
		ByteBuffer messageBuffer = ByteBuffer.wrap(messageInput);
		
		GlobalData2012.gmJoystickX = messageBuffer.getFloat();
		GlobalData2012.gmJoystickY = messageBuffer.getFloat();
		
		GlobalData2012.gmHorizontalGyroAngle = messageBuffer.getFloat();
		
		GlobalData2012.gmEncoderDistance = messageBuffer.getFloat();
		
		GlobalData2012.gmDriveLeftPower = messageBuffer.getFloat();
		GlobalData2012.gmDriveRightPower = messageBuffer.getFloat();
		GlobalData2012.gmShooterPower = messageBuffer.getFloat();
		
		GlobalData2012.gmTopHoopDetected = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmTopHoopXCoord = messageBuffer.getInt();
		GlobalData2012.gmTopHoopYCoord = messageBuffer.getInt();
		
		GlobalData2012.gmMidLeftHoopDetected = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmMidLeftHoopXCoord = messageBuffer.getInt();
		GlobalData2012.gmMidLeftHoopYCoord = messageBuffer.getInt();
		
		GlobalData2012.gmMidRightHoopDetected = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmMidRightHoopXCoord = messageBuffer.getInt();
		GlobalData2012.gmMidRightHoopYCoord = messageBuffer.getInt();
		
		GlobalData2012.gmBottomHoopDetected = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmBottomHoopXCoord = messageBuffer.getInt();
		GlobalData2012.gmBottomHoopYCoord = messageBuffer.getInt();
		
		GlobalData2012.gmRobotXCoord = messageBuffer.getFloat();
		GlobalData2012.gmRobotZCoord = messageBuffer.getFloat();
		
		GlobalData2012.gmActualShooterRPM = messageBuffer.getFloat();
		GlobalData2012.gmDesiredShooterRPM = messageBuffer.getFloat();
		
		GlobalData2012.gmShooterAngle = messageBuffer.getFloat();
		
		GlobalData2012.gmComputedBallVelocity = messageBuffer.getFloat();
		
		GlobalData2012.gmGyroBiasVoltage = messageBuffer.getFloat();
		
		GlobalData2012.gmDeadZone = messageBuffer.getFloat();
		
		GlobalData2012.gmVerticalGyroAngle = messageBuffer.getFloat();
		
		GlobalData2012.gmDriveStraightOffset = messageBuffer.getFloat();
		
		GlobalData2012.gmShooterCamSpeed = messageBuffer.getFloat();
		GlobalData2012.gmShooterUserSpeed = messageBuffer.getFloat();
		GlobalData2012.gmShooterSpeedOverride = messageBuffer.getInt() > 0 ? true: false;
		
		GlobalData2012.gmBackLimitSwitchDown = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmBackLimitSwitchUp = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmFrontLimitSwitchDown = messageBuffer.getInt() > 0 ? true: false;
		GlobalData2012.gmFrontLimitSwitchUp = messageBuffer.getInt() > 0 ? true: false;
		
	}
}
