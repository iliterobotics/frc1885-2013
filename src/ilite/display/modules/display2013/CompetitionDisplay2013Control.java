package ilite.display.modules.display2013;

import ilite.display.Constants;
import ilite.display.IMasterController;
import ilite.display.StateData;
import ilite.display.camera.KinectImage;
import ilite.display.config.ISystemConfiguration;
import ilite.display.interfaces.net.EConnectionState;
import ilite.display.interfaces.net.IRobotConnector;
import ilite.display.modules.IModuleControl;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.display.omi.ConnectionStatusPanel;
import ilite.util.gui.components.EPanelOrientation;
import ilite.util.lang.Notifyer;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Container;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * This class is responsible for registering the hooks
 * to the back-end for message parsing.  It also registers
 * the hooks for data logging. 
 *
 */
public class CompetitionDisplay2013Control implements IModuleControl, IRobotConnector{
	private ISystemConfiguration mConfiguration = null;
	private RobotMessageParser2013 mRobotConnection = null;
	private StateData mStateData = null;
	private CompetitionDisplay2013View mView;
	private CompetitionDisplay2013Model mModel;
	private static double sTestAngle = 0.0;
	private static int sTestTimer = 0;
	private Notifyer mNotificationThread = new Notifyer(true);
	private ILog mLog = Logger.createLog(CompetitionDisplay2013Control.class);
	private AtomicBoolean mImageWriterInit = new AtomicBoolean(false);
	private Long mMatchStartTime = null;
	private IMasterController mMaster = null;

  @Override
  public String getName()
  {
    return "ULTIMATE ASCENT";
  }

	@Override
	public void initControl(IMasterController pMaster) {
	  mMaster = pMaster;
	  mView = new CompetitionDisplay2013View(mMaster);
		ConnectionStatusPanel csp = new ConnectionStatusPanel(EPanelOrientation.DOWN_CENTER, this,false, pMaster.getStateData());
		mView.setControl(this);
		mView.setConnectionStatusPanel(csp);
    mNotificationThread.addNotificationListener(mView);
    mView.setDataModel(mModel);
	}

	/*package*/ CompetitionDisplay2013Model getModel()
	{
		return mModel;
	}

	@Override
	public void postApplicationInit() {
		mConfiguration = mMaster.getSystemConfiguration();
		mView.initLowerRightWindow();
		mStateData = mMaster.getStateData();

    initImageWriter();

		boolean isDebug = mConfiguration.getBool("system.standalonemode");
		if(isDebug)
		{
			mLog.info("Starting Data Driver");
			RobotData2013.startDataDriver(this);
		}

		String path = mConfiguration.getString("robot.data.output.path");
		try {

			final RobotDataFileWriter writer = new RobotDataFileWriter(mModel, path);
			mNotificationThread.addNotificationListener(writer);
			Runtime.getRuntime().addShutdownHook(new Thread(){
				@Override public void run()
				{
					writer.closeFile();
				}
			});
		} catch (IOException e) {
			System.err.println("Non-Fatal Error:");
			e.printStackTrace();
		}
		
	}
	
	@Override
	public void initiateShutdownSequence() {
		// TODO Make sure the global data file writer is closed.		
	}

	public void addRobotData(RobotData2013 data) {
	  if(!mImageWriterInit.get())
	  {
	    initImageWriter();
	  }
	  
	  if(mMatchStartTime == null)
	  {
	    mMatchStartTime = data.time;
	  }
	  data.time -= mMatchStartTime;
	  
		mModel.addRobotData(data);
		mNotificationThread.notifyOfUpdate();
	}

	public void attemptRobotConnection()
	{
		EConnectionState state = mStateData.getRobotConnectionStatus();
		if(state == EConnectionState.ATTEMPTING || state == EConnectionState.ESTABLISHED)
		{
			return;
		}

		mStateData.setRobotConnectionStatus(EConnectionState.ATTEMPTING);

		mRobotConnection = new RobotMessageParser2013(this, mMaster.getStateData());

		mMaster.startConnections();

		Thread t1 = new Thread("ROBOT CONNECTION THREAD")
		{
			@Override public void run()
			{
				String ip = mConfiguration.getString(Constants.sROBOT_CRIO_IP);
				String port = mConfiguration.getString(Constants.sROBOT_CRIO_PORT);
				mRobotConnection.connect(ip, Integer.parseInt(port));
//				MessagePusher.inst.setSocket(mRobotConnection.getSocket());

				boolean bTemp = mRobotConnection.isConnected();
				mLog.debug("Connection From Competition: " + bTemp);

				if(bTemp)
				{
					mLog.debug(EConnectionState.ESTABLISHED);
					mStateData.setRobotConnectionStatus(EConnectionState.ESTABLISHED);
					new Thread(mRobotConnection).start();
				}
				else
				{
					mLog.debug(EConnectionState.ERROR);
					mStateData.setRobotConnectionStatus(EConnectionState.ERROR);
				}
				mLog.debug("Finished Connection Thread");
			}
		};
		t1.start();
	}

	public void disconnectRobot()
	{
	  mMaster.stopConnections();
		Thread t = new Thread("ROBOT DISCONNECT THREAD")
		{
			@Override public void run()
			{
				mRobotConnection.disconnect();
				mStateData.setRobotConnectionStatus(EConnectionState.DISCONNECTED);
			}
		};
		t.start();
	}

	private void initImageWriter()
	{
    mImageWriterInit.set(true);
    String path = mConfiguration.getString("robot.data.output.path");
    
    try
    {
      URL url = new URL(mMaster.getSystemConfiguration().getString("robot.camera.url"));
      String buffer_size = mMaster.getSystemConfiguration().getString("robot.camera.bufferSizeBytes");

      if(buffer_size == null)
      {
        KinectImage kis = new KinectImage(url, mStateData);
        kis.addListener(mView.getKinectPanel().getUpdatingImagePanel().getUpdatingImage());
        new KinectImageWriter(kis, path);
      }
      else
      {
        KinectImage kis = new KinectImage(url, Integer.parseInt(buffer_size), mStateData);
        new KinectImageWriter(kis, path);
      }
    } catch (MalformedURLException e1)
    {
      e1.printStackTrace();
    }
    
	}

  @Override
  public Container getDisplayComponent()
  {
    return mView.getDisplayPanel();
  }
}