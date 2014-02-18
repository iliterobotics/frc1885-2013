package ilite.display.modules.display2012;

import ilite.display.interfaces.fileio.DataLogger2012;
import ilite.display.interfaces.net.TCPClient;
import ilite.util.lang.Delegator;
import ilite.util.lang.IUpdate;

import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

public class MessageParser2012 extends TCPClient{

	private byte[] maHeaderDataRegion;
	private byte[] maMsgSizeDataRegion;
	private byte[] maMessageDataRegion;
	
	private int dataType;
	private int messageSize;
	private Map<Integer, Delegator<byte[]>> mProcessors = new HashMap<Integer, Delegator<byte[]>>();
	private DataLogger2012 mGameInputLogger;
		
	public void setDataLogger(DataLogger2012 pLogger)
	{
	  mGameInputLogger = pLogger;
	}

	@SuppressWarnings("unchecked")
	public void registerProcessor(int pDataMarker, IUpdate<byte[]> pProcessor)
	{
		Delegator<byte[]> del = mProcessors.get(pDataMarker);
		if(del == null)
		{
			del = new Delegator<byte[]>();
		}
		del.addListener(pProcessor);
		mProcessors.put(pDataMarker, del);
	}
	
	private void init() {
		maHeaderDataRegion = new byte[4];
		maMsgSizeDataRegion = new byte[4];
		maMessageDataRegion = new byte[150000];
	}

	@Override
	public void parse(InputStream is) throws Exception {
		readInput (is, 4, maHeaderDataRegion);
		readInput (is, 4, maMsgSizeDataRegion);
		
		messageSize = ByteBuffer.wrap(maMsgSizeDataRegion).getInt();				
		dataType = ByteBuffer.wrap(maHeaderDataRegion).getInt();
		
		readInput (is, messageSize, maMessageDataRegion);
		
		mGameInputLogger.write(maHeaderDataRegion, maMsgSizeDataRegion, maMessageDataRegion);
		
		Delegator<byte[]> delegator = mProcessors.get(dataType);
		if(delegator != null)
		{
			delegator.update(maMessageDataRegion);
		}
		else
		{
			System.out.println("Unrecognized data marker: " + dataType);
		}
		
		init();
		
	}
}
