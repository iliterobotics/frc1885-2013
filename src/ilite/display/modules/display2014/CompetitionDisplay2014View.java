package ilite.display.modules.display2014;

import ilite.display.Constants;
import ilite.display.IMasterController;
import ilite.display.camera.AxisCameraImpl2014;
import ilite.display.config.ISystemConfiguration;
import ilite.display.modules.IModuleControl;
import ilite.display.modules.display2014.data.Data2014;
import ilite.display.modules.display2014.overlays.LauncherStateOverlay;
import ilite.display.modules.display2014.overlays.PositionOverlay;
import ilite.display.modules.display2014.overlays.RollerIntakeOverlay;
import ilite.display.modules.display2014.overlays.SpeedOverlay;
import ilite.display.modules.display2014.overlays.TuskStateOverlay;
import ilite.display.omi.CameraImagePanel;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTabbedPane;
import javax.swing.SpringLayout;

/**
 * @author JesseK
 * 
 * TODO list
 *  - Add java console panel to tabs
 *  - Remove old image writer from file listeners once a new file is selected
 *
 */
public class CompetitionDisplay2014View implements IModuleControl, IUpdate<Data2014>
{
  
	/**@author Natalie
	 * @Version 1.0.1
	 * Creates the main competition display screen, with a secondary panel designed to 
	 * Note: add change listeners for sliders and update components. Missing diagnostics panel
	 **/

	private IMasterController mMaster = null;
	private JPanel mMainPanel, mCamPanel, mCam2Panel;
	private CameraImagePanel mCamImg, mCam2Img;
	private int slPAD_EW, slPAD_NS;
	private BufferedImage mNoVid;
	public final static String sHORIZONTAL_CURSOR = "2012.ui.horizontalCameraCursor";
	public final static String sVERTICAL_CURSOR = "2012.ui.verticalCameraCursor";
	private ISystemConfiguration mSysCfg;
	private final TelemetryTablePanel mTelemetry;
  private RobotDebugPrinter mDebug = new RobotDebugPrinter();
  private MatchConfigPanel matchconfig;
  private ConnectionPanel mConnectionPanel;
  
  static
  {
    Constants.sCONFIG_DIR = DataManager2014.getInstance().getRootDataFolder();
  }
	
	public CompetitionDisplay2014View()
	{	
	  mTelemetry = new TelemetryTablePanel();
	}

	private void create()
	{
		//sets the SpringLayoutPadding. Will need to be negative for south and west.
		slPAD_EW = 5;
		slPAD_NS = 5;

		//Configures main panel to fit the size of the screen with room left to spare for frc dashboard.
		mMainPanel = new JPanel();
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		int ssWidth = (int)(screenSize.getWidth());
		int ssHeight = (int)(screenSize.getHeight());
		mMainPanel.setSize(ssWidth, ssHeight-300);

		System.out.println("Main Panel Dimension: " + ssWidth + " x " + (ssHeight-300));

		//Prints image if there is no camera image. Needs to be changed.
		try{
			File test = new File("images/BSOD.jpg");
			mNoVid = ImageIO.read(test);
		}catch(IOException e){
			e.printStackTrace();
			mNoVid = null;
		}

		//Creates the Camera panel
		Dimension d = Constants.getImageSize();
		mCamImg = new CameraImagePanel(630, 405, mNoVid);

		//Adds a horizontal cursor
		final JSlider horz = (mCamImg).addHorizontalCursor(sHORIZONTAL_CURSOR,
				Color.RED);
		int h_value = mSysCfg.getInt(sHORIZONTAL_CURSOR);
		horz.setValue(h_value); //should use h_value

		//Adds a vertical cursor
		final JSlider vert = mCamImg.addVerticalCursor(sHORIZONTAL_CURSOR,
				Color.RED);
		int v_value = mSysCfg.getInt(sVERTICAL_CURSOR);
		vert.setValue(v_value); //should use v_value

		//Creates the containing panel for the camera. Size set to 640 by 480 CONTAINING PANEL ONLY
		mCamPanel = new JPanel();
		mCamPanel.setPreferredSize(new Dimension(640, 480));

		//Creates the overlays based off of location; not yet refactored to be self-sufficient.
		String status = "main";
		mCamImg.addOverlay(new LauncherStateOverlay(5, 352, 100, 50, 5));
		mCamImg.addOverlay(new RollerIntakeOverlay((702/2)-85, 352, status));
		mCamImg.addOverlay(new PositionOverlay(5, 5, "000.0 ft", status));
		mCamImg.addOverlay(new SpeedOverlay(525, 5, "000 mph", status));
		mCamImg.addOverlay(new TuskStateOverlay(525, 352, status));

		//Adds the layered panel overlayss
		JPanel camImg = mCamImg.getPanel();

		//Camera containing panel--adds overlays
		mCamPanel.add(camImg);
		mCamPanel.setBorder(BorderFactory.createTitledBorder("Camera Panel"));


		//Creates JTabbedPane for diagnostics. May be changed depending on what diagnostics are necessary.
		JTabbedPane mDiagnosticsPanel  = new JTabbedPane();
		mDiagnosticsPanel.setPreferredSize(new Dimension(ssWidth/2, ssHeight));

		//Creates 2D array for JTable. Currently has dummy values. Needs to be updated.
//		Object[][] list = {{"Encoder 1A", new Integer(0), new Integer(0)}, {"Encoder 1B", new Integer(0), new Integer(0)},
//				{"Encoder 2A", new Integer(0), new Integer(0)}, {"Encoder 2B", new Integer(0), new Integer(0)}, 
//				{"Pressure Switch", new Integer(0), new Integer(0)}, {"Gyro", new Integer(0), new Integer(0)},
//				{"DT BL", new Integer(0), new Integer(0)}, {"DT BR", new Integer(0), new Integer(0)}, 
//				{"DT FL", new Integer(0), new Integer(0)}, {"DT FR", new Integer(0), new Integer(0)},
//				{"Intake", new Integer(0), new Integer(0)}, {"Compressor", new Integer(0), new Integer(0)},
//				{"Solenoid 1 Spike", new Integer(0), new Integer(0)}, {"Solenoid 2 Spike", new Integer(0), new Integer(0)},
//				{"Solenoid 3 Spike", new Integer(0), new Integer(0)}, {"Solenoid 4 Spike", new Integer(0), new Integer(0)}
//		};

		//Column headings JTable
		String columns[] = {"Name", "Value", "Run. Avg."};

		//Init the JTable
//		JTable dataR = new JTable(list, columns);
//		dataR.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
//		dataR.setSelectionBackground(Color.black);
//		dataR.setSelectionForeground(Color.white);
//		dataR.setFont(new Font("Arial", Font.BOLD, 20));
//		dataR.setRowHeight(25);

		//Creates a scrollpane for diagnostic panel with scrolls as needed
//		JScrollPane diagnostics = new JScrollPane(dataR, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, 
//				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);


		//Creates a second JTable for possible vision diagnostics. Unnecessary/ redundant; likely removed 
//
//		String headers[] = {"Name", "Value", "Run. Avg."};
//
//		JTable dataV = new JTable();
//		dataR.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
//		dataR.setSelectionBackground(Color.black);
//		dataR.setSelectionForeground(Color.white);
//		dataR.setFont(new Font("Arial", Font.BOLD, 20));
//		dataR.setRowHeight(25);

//		JScrollPane visionDiagnostics = new JScrollPane(dataV, JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, 
//				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);

		TelemetryTablePanel robotTelemetry = new TelemetryTablePanel();
		DataManager2014.getInstance().getTelemetryProvider().addListener(robotTelemetry);

    //Main panel receives its components
		mConnectionPanel = new ConnectionPanel();
    mDiagnosticsPanel.addTab("Connection Info", mConnectionPanel);
		mDiagnosticsPanel.addTab("Robot Diagnostics", robotTelemetry.getDisplayPanel());
//		mDiagnosticsPanel.addTab("Vision Diagnostics", visionDiagnostics);

    DataManager2014.getInstance().getDebugMsgProvider().addListener(mDebug);
    mDiagnosticsPanel.add("Robot Debug", mDebug);
		
    matchconfig = new MatchConfigPanel(DataManager2014.getInstance().getRootDataFolder());
    mDiagnosticsPanel.add("Match Config", matchconfig.getDisplayPanel());
		
		
		mMainPanel.add(mCamPanel);
		mMainPanel.add(mDiagnosticsPanel);

		//Main panel receives its layout
		SpringLayout sl = new SpringLayout();
		mMainPanel.setLayout(sl);

		//Layout of the components on main
		sl.putConstraint(SpringLayout.WEST, mCamPanel, slPAD_EW, SpringLayout.WEST, mMainPanel);
		sl.putConstraint(SpringLayout.NORTH, mCamPanel, slPAD_NS, SpringLayout.NORTH, mMainPanel);
		sl.putConstraint(SpringLayout.SOUTH, mCamPanel, -slPAD_NS, SpringLayout.SOUTH, mMainPanel);

		sl.putConstraint(SpringLayout.EAST, mDiagnosticsPanel, -slPAD_EW, SpringLayout.EAST, mMainPanel);
		sl.putConstraint(SpringLayout.NORTH, mDiagnosticsPanel, slPAD_NS, SpringLayout.NORTH, mMainPanel);
		sl.putConstraint(SpringLayout.SOUTH, mDiagnosticsPanel, -slPAD_NS, SpringLayout.SOUTH, mMainPanel);

		sl.putConstraint(SpringLayout.EAST, mCamPanel, -slPAD_EW, SpringLayout.WEST, mDiagnosticsPanel);

		//Init graphics environment for two screen view
		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
		GraphicsDevice lcGraphicSceenDevices[] = ge.getScreenDevices();
		GraphicsDevice lcGogglesHead = null;
		GraphicsConfiguration lcGogglesConfig = null;

		System.out.println("Number of GraphicsDevices: " +lcGraphicSceenDevices.length+ "\nDimensions of Secondary Panels: "
				+ lcGraphicSceenDevices[lcGraphicSceenDevices.length-1].getDisplayMode().getWidth() + " x " 
				+ lcGraphicSceenDevices[lcGraphicSceenDevices.length-1].getDisplayMode().getHeight());

		//Tests number of Graphics devices
		if(lcGraphicSceenDevices.length > 1)
		{
			for(int i = 0; i<lcGraphicSceenDevices.length; i++)
			{
				GraphicsDevice gdeviced = lcGraphicSceenDevices[i];
				if(gdeviced.getDisplayMode().getHeight() == 1024 && gdeviced.getDisplayMode().getWidth() == 1280)
				{
					lcGogglesHead = gdeviced;
					lcGogglesConfig = gdeviced.getDefaultConfiguration();
				}
			}

			int lcgScreenSizeHeight = lcGraphicSceenDevices[1].getDisplayMode().getHeight();
			int lcgScreenSizeWidth = lcGraphicSceenDevices[1].getDisplayMode().getWidth();

			//Sets secondary screen if extant
			if(lcGogglesConfig != null)
			{
				//Creates panel for secondary screens
				mCam2Img = new CameraImagePanel(lcgScreenSizeWidth, lcgScreenSizeHeight, mNoVid);

				//Creates horizontal cursor
				final JSlider h = mCam2Img.addHorizontalCursor(sHORIZONTAL_CURSOR,
						Color.RED);
				int h2_value = mSysCfg.getInt(sHORIZONTAL_CURSOR);
				h.setValue(h2_value); //should use h2_value

				//Creates vertical cursor
				final JSlider v = mCam2Img.addVerticalCursor(sHORIZONTAL_CURSOR,
						Color.RED);
				int v2_value = mSysCfg.getInt(sVERTICAL_CURSOR);
				v.setValue(v2_value); //should use v2_value

				status = "secondary";

				//creates containing panel
				mCam2Panel = new JPanel();

				//Adds overlays to layered panel
				mCam2Img.addOverlay(new LauncherStateOverlay(65, 805, 175, 150, 5));
				mCam2Img.addOverlay(new RollerIntakeOverlay((1024)/2+75, 805, status));
				mCam2Img.addOverlay(new PositionOverlay(65, 15, "00.0", status));
				mCam2Img.addOverlay(new SpeedOverlay(1085, 15, "00.0", status));
				mCam2Img.addOverlay(new TuskStateOverlay(1085, 805, status));

				JPanel cam2Img = mCam2Img.getPanel();

				//adds overlay/layered panel to containing panel
				mCam2Panel.add(cam2Img);
				mCam2Panel.setBorder(BorderFactory.createTitledBorder("Camera Panel"));

				//sets new JFrame for secondary screen
				JFrame hudFrame = new JFrame("Heads-Up Screen",lcGogglesConfig);

				//Sets secondary screen settings
				Canvas c = new Canvas(lcGogglesConfig); 
				Rectangle gcBounds = lcGogglesConfig.getBounds();
				System.out.println(gcBounds);
				hudFrame.getContentPane().add(c);
				hudFrame.setBounds(gcBounds);

				//Init secondary screen(s)
				hudFrame.setPreferredSize(new Dimension(lcgScreenSizeWidth, lcgScreenSizeHeight));
				hudFrame.add(mCam2Panel);
				hudFrame.pack();
				hudFrame.setVisible(true);
				hudFrame.setAlwaysOnTop(true);

			}
		}

	}
	@Override
	public void postApplicationInit() 
	{
    DataManager2014 mgr = DataManager2014.getInstance();
    mgr.getTelemetryProvider().addListener(this);
    mgr.getTelemetryProvider().addListener(mTelemetry);
    mgr.getDebugMsgProvider().addListener(mDebug);
    // mCamImg, mCam2Img;
    IProvider<BufferedImage> cameraStream = mMaster.getDataProviders().getCameraImageProvider();
    if(cameraStream == null)
    {
      AxisCameraImpl2014 camera = new AxisCameraImpl2014();
      mConnectionPanel.getCameraConfigPanel().setCamera(camera);
      cameraStream = camera;
      mMaster.getDataProviders().setCameraImageProvider(camera);
    }
    cameraStream.addListener(mCamImg.getUpdatingImage());
    if(mCam2Img != null)
    {
      cameraStream.addListener(mCam2Img.getUpdatingImage());      
    }
	}

	@Override
	public Container getDisplayComponent() 
	{
		return mMainPanel;
	}

	@Override
	public void initiateShutdownSequence() 
	{
	  DataManager2014.getInstance().disconnectRobot();
	}

	@Override
	public String getName() 
	{
		return "Competition Display 2014";
	}

	@Override
	public void initControl(IMasterController pMaster) 
	{
		//Initalizes panel
		mMaster = pMaster;
		mSysCfg = mMaster.getSystemConfiguration();
    create();
	}

  @Override
  public void update(Data2014 pObject)
  {
    // TODO Auto-generated method stub
    
  }

}
