package ilite.display.modules.display2013;

import ilite.display.StateData;
import ilite.display.modules.display2013.data.RobotCommand;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;

import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;

import org.ilite.comm.BinaryChunkWriter;
import org.ilite.comm.ChunkTransmission;
import org.ilite.comm.ChunkWriter;

public class MessagePusher implements IUpdate<RobotCommand>{
	private StateData mStateData;
	public static MessagePusher inst;
	
	private Socket mOutputSocket;
	
	public MessagePusher(IProvider<RobotCommand> pDataProvider, StateData pStateData)
	{
		pDataProvider.addListener(this);
		mStateData = pStateData;
	}
	
	public void setSocket(Socket pSocket) {
		mOutputSocket = pSocket;
	}
	
	public void push(final byte barr[]) {
		ChunkWriter cw= new BinaryChunkWriter();
		ByteBuffer buffer= ByteBuffer.wrap(barr);
		cw.writeByte(buffer.get());
		cw.writeByte(buffer.get());
		cw.writeDouble(buffer.getDouble());
		mStateData.addOutgoingData(cw.chunkSize());
		try {
			ChunkTransmission.write(mOutputSocket.getOutputStream(), cw);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void update(RobotCommand pObject) {
		// TODO Auto-generated method stub
		push(pObject.encode());
	}
}
