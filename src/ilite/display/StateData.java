package ilite.display;

import ilite.display.interfaces.net.Bandwidth;
import ilite.display.interfaces.net.EConnectionState;
import ilite.util.lang.IUpdate;
import ilite.util.lang.StateDatum;

import java.awt.Dimension;
import java.awt.Window;
import java.util.Timer;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class StateData {

	private StateDatum<EConnectionState> mConnectionState = new StateDatum<EConnectionState>();
	private StateDatum<Bandwidth> mOutgoingBandwidth = new StateDatum<Bandwidth>();
	private StateDatum<Bandwidth> mIncomingBandwidth = new StateDatum<Bandwidth>();
	private StateDatum<Bandwidth> mCameraBandwidth = new StateDatum<Bandwidth>();
	private Window mParentWindow = null;
	private Timer mBandwidthMonitor;

	private ExecutorService mStateDataUpdateThread = Executors.newSingleThreadExecutor();

	public StateData()
	{
		mConnectionState.setData(EConnectionState.DISCONNECTED);
		mBandwidthMonitor = new Timer("State Data Bandwidth Timer");
	}

	/*package*/ void setParentWindow(Window pParent)
	{
		mParentWindow = pParent;
	}
	
	public Dimension getParentWindowSize()
	{
	  return mParentWindow.getSize();
	}

	public Window getParentWindow()
	{
		return mParentWindow;
	}
	
	public void addCameraBandwidthListener(IUpdate<Bandwidth> pListener)
	{
		mCameraBandwidth.addListener(pListener);
	}

	public void addRobotConnectionStatusListener(IUpdate<EConnectionState> pListener)
	{
		mConnectionState.addListener(pListener);
	}

	public void addIncomingBandwidthListener(IUpdate<Bandwidth> pListener)
	{
		mIncomingBandwidth.addListener(pListener);
	}

	public void addOutgoingBandwidthListener(IUpdate<Bandwidth> pListener)
	{
		mOutgoingBandwidth.addListener(pListener);
	}

	public void setRobotConnectionStatus(final EConnectionState pState)
	{
		mStateDataUpdateThread.submit(new Runnable() {			
			@Override
			public void run() {
				mConnectionState.setData(pState);
			}
		});
	}
	
	public void addCameraData(int pNumOfBytes)
	{
		Bandwidth bw = mCameraBandwidth.getData();
		if(bw == null)
		{
			bw = new Bandwidth();
		}
		bw.addBytes(pNumOfBytes);
		mCameraBandwidth.setData(bw);
	}

	public void addIncomingData(int pNumOfBytes)
	{
		Bandwidth bw = mIncomingBandwidth.getData();
		if(bw == null)
		{
			bw = new Bandwidth();
		}
		bw.addBytes(pNumOfBytes);
		mIncomingBandwidth.setData(bw);
	}

	public void addOutgoingData(int pNumOfBytes)
	{
		Bandwidth bw = mOutgoingBandwidth.getData();
		if(bw == null)
		{
			bw = new Bandwidth();
		}
		bw.addBytes(pNumOfBytes);
		mOutgoingBandwidth.setData(bw);
	}

	public EConnectionState getRobotConnectionStatus()
	{
		return mConnectionState.getData();
	}

}
