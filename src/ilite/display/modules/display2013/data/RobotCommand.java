package ilite.display.modules.display2013.data;

import java.nio.ByteBuffer;

public class RobotCommand
{
	public final byte mMSG_ID, mSYS_ID;
	public final double mVAL_IN;
	
	public RobotCommand(final int pMSG_ID, final int pSYS_ID, final double pVAL_IN)
	{
		mMSG_ID = (byte)pMSG_ID;
		mSYS_ID = (byte)pSYS_ID;
		mVAL_IN = pVAL_IN; 
	}
	public byte[] encode()
	{
		ByteBuffer messageBuffer = ByteBuffer.allocate(10);
		messageBuffer.put(mMSG_ID);
		messageBuffer.put(mSYS_ID);
		messageBuffer.putDouble(mVAL_IN);
		return messageBuffer.array();
	}
	public String toString()
	{
		return ("MSGID: " + mMSG_ID + " SYSID: " + mSYS_ID + " = " + mVAL_IN);
	}
	
}