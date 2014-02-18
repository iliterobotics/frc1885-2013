package ilite.display.modules.display2013;

import ilite.display.StateData;
import ilite.display.interfaces.net.TCPClient;
import ilite.util.lang.Delegator;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;

import java.io.InputStream;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import org.ilite.comm.ChunkReader;
import org.ilite.comm.ChunkReaderException;
import org.ilite.comm.ChunkTransmission;

public class VisionMessageParser extends TCPClient implements IProvider<byte[]> {
	// All the code here may be changed, but be careful about changing Important Code
	private StateData mStateData;
	private int currentChunkSize;
	private ChunkReader currentChunk;
	private final Executor mParsingThreadPool = Executors.newFixedThreadPool(2);
	public Delegator<byte[]> delegator;
	
	public VisionMessageParser(StateData pStateData)
	{
	  mStateData = pStateData;
	}

	@Override
	public void parse(InputStream is) throws Exception
	{
		currentChunk = ChunkTransmission.read(is);
		currentChunkSize = currentChunk.chunkSize();
		updateBandwidth(currentChunkSize);
		mParsingThreadPool.execute(new Runnable(){      
	      @Override
	      public void run(){
	        processVisionChunk();
	      }
	    });
	}

	private void updateBandwidth(int pNumOfBytes)
	{
	  mStateData.addIncomingData(pNumOfBytes);
	}
	
	// IMPORTANT CODE
	
	private void processVisionChunk() {
		try {
			setKinectCameraImage(currentChunk.nextChunk());
			setKinectCameraImageTimestamp(currentChunk.nextDouble());
			setKinectDepthImage(currentChunk.nextChunk());
			setKinectDepthImageTimestamp(currentChunk.nextDouble());
			setDistanceToGoal(currentChunk.nextDouble());
		} catch (ChunkReaderException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private synchronized void setKinectCameraImage(final byte[] bImage) {
		delegator.update(bImage);
	}
	
	private synchronized void setKinectCameraImageTimestamp(final double timestamp) {
		
	}
	
	private synchronized void setKinectDepthImage(final byte[] bImage) {
		
	}
	
	private synchronized void setKinectDepthImageTimestamp(final double timestamp) {
		
	}
	
	private synchronized void setDistanceToGoal(final double distance) {
		
	}
	
	// IMPORTANT CODE END
	
	@Override
	public void addListener(IUpdate<byte[]> pListener) {
		// TODO Auto-generated method stub
		delegator.addListener(pListener);
	}

	@Override
	public void removeListener(IUpdate<byte[]> pListener) {
		// TODO Auto-generated method stub
		delegator.removeListener(pListener);
	}

	@Override
	public byte[] getLatest() {
		// TODO Auto-generated method stub
		return delegator.getLatest();
	}
}
