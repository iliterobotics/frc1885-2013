package ilite.display.modules.display2013;

import static javax.swing.SpringLayout.EAST;
import static javax.swing.SpringLayout.NORTH;
import static javax.swing.SpringLayout.SOUTH;
import static javax.swing.SpringLayout.WEST;
import ilite.display.IMasterController;
import ilite.display.modules.display2013.data.RobotData2013;
import ilite.display.omi.ConnectionStatusPanel;
import ilite.display.omi.UtilityStatusBar;
import ilite.util.gui.SimpleDataPrinter;
import ilite.util.lang.INotify;
import ilite.util.lang.reflection.DataStructureReader;

import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.DisplayMode;
import java.awt.Font;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Window;
import java.text.DateFormat;
import java.util.Date;

import javax.swing.BorderFactory;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JWindow;
import javax.swing.SpringLayout;
import javax.swing.SwingUtilities;

/**
 * This is the competition panel display.  There's only one panel to display,
 * and this is it.  All display manipulation should happen in this class.
 *
 */
public class CompetitionDisplay2013View implements INotify
{
	private CompetitionDisplay2013Model mDataModel;
	ConnectionStatusPanel mConnectionStatusPanel;
	public final int sPAD_EW = 5;
	public final int sPAD_NS = 5;
	public final int magicNum = 0;
	private JTextField colorField; 
	private String mValue = "00.0";
	AutonomousPanel autoPanel;
	private JLabel tiltLabel = new JLabel(mValue);
	private JLabel wheelLabel = new JLabel(mValue);
	private JLabel bearLabel = new JLabel(mValue);
	private JLabel timeLabel;
	private JPanel mMainPanel;
	private JPanel infoPanel, containPanel, bearPanel, speedPanel;
	private KinectImagePanel mKinectPanel;
	private JWindow popUp;
	private boolean mOverHeight = false;
	private final String LIMIT_UPPER = "2013.robot.overHeightLimitUpper";
	private final String LIMIT_LOWER = "2013.robot.overHeightLimitLower";
	private SimpleDataPrinter mPrinter = null;
	private final IMasterController mMaster;
	private GraphicsDevice mSecondScreen = null;
	
	private CompetitionDisplay2013Control mControl;
	
	public CompetitionDisplay2013View(IMasterController pMaster)
	{
	  mMaster = pMaster;
	  GraphicsDevice[] heads = GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices();
	  for(int i = 0; i < heads.length; i++)
	  {
	    DisplayMode mode = heads[i].getDisplayMode();
	    if(mode.getHeight() == 600 && mode.getWidth() == 800)
	    {
	      mSecondScreen = heads[i];
	    }
	  }
	}

	public Container getDisplayPanel() {
		//creating the panels
		mMainPanel = new JPanel();
		containPanel = new JPanel();
		containPanel.setBorder(BorderFactory.createTitledBorder("Contain Panel"));
		Dimension d = mMaster.getStateData().getParentWindowSize();
		containPanel.setSize(new Dimension(d.width-1050, 250));

//		autoPanel = new AutonomousPanel();
		mKinectPanel = new KinectImagePanel(mDataModel, mMaster.getSystemConfiguration());
		mKinectPanel.setBorder(BorderFactory.createTitledBorder("Kinect Image Panel"));
		
		if(mSecondScreen != null)
		{
		  System.out.println("Initializing Secondary Screen");
		  JFrame kinectscreen = new JFrame();
	    KinectImagePanel offscreenpanel = new KinectImagePanel(mDataModel, mMaster.getSystemConfiguration());
		  kinectscreen.setContentPane(offscreenpanel);
		  mSecondScreen.setFullScreenWindow(kinectscreen);
      kinectscreen.setUndecorated(true);
		  kinectscreen.pack();
		  kinectscreen.setVisible(true);
		  kinectscreen.setExtendedState(JFrame.MAXIMIZED_BOTH);
		}

		infoPanel = new JPanel();
		infoPanel.setBorder(BorderFactory.createTitledBorder("Info Panel"));
//		allyPanel = new JPanel();
		speedPanel = new JPanel();
		bearPanel = new JPanel();
		
		infoPanel.setSize(new Dimension(500, 100));
		infoPanel.setBorder(BorderFactory.createDashedBorder(Color.GREEN));
		
		speedPanel.setSize(new Dimension(500, 100));
		speedPanel.setBorder(BorderFactory.createDashedBorder(Color.GREEN));
		
		bearPanel.setSize(new Dimension(500, 100));
		bearPanel.setBorder(BorderFactory.createDashedBorder(Color.GREEN));

		Font num_font = new Font("Arial", Font.PLAIN, 36);
		Font lab_font = new Font("Arial Narrow", Font.PLAIN, 22);
		JLabel infoLabel = new JLabel("KNT ANG: ");
		JLabel degreeLabel = new JLabel("\u00B0");
		JLabel percentLabel = new JLabel("%");
		JLabel speedLabel= new JLabel("KNT SPD: ");
		JLabel bearingLabel = new JLabel("KNT BRG: ");
		timeLabel = new JLabel("LAST UPDATED AT: ");

    DataStructureReader reader = new DataStructureReader();
    reader.setShowBraces(false);
    reader.setShowShortName(true);
    mPrinter = new SimpleDataPrinter(reader);        
    mPrinter.printObject(new RobotData2013());
		
		tiltLabel.setFont(num_font);
		tiltLabel.setForeground(Color.RED);
		wheelLabel.setFont(num_font);
		wheelLabel.setForeground(Color.RED);
		bearLabel.setFont(num_font);
		bearLabel.setForeground(Color.RED);

		infoLabel.setFont(lab_font);
		speedLabel.setFont(lab_font);
		bearingLabel.setFont(lab_font);
		
		infoPanel.add(infoLabel);
		infoPanel.add(tiltLabel);
		
		speedPanel.add(speedLabel);
		speedPanel.add(wheelLabel);
		speedPanel.add(percentLabel);
		
		bearPanel.add(bearingLabel);
		bearPanel.add(bearLabel);
		
//		 UtilityStatusBar.addComponent(timeLabel);

//		IliteButton goButton = new IliteButton("GO");

		//creating the layout
		SpringLayout sl = new SpringLayout();
		mMainPanel.setLayout(sl);
		SpringLayout sl2 = new SpringLayout();
		containPanel.setLayout(sl2);

		JPanel printer = mPrinter.getPanel();
		printer.setBorder(BorderFactory.createTitledBorder("Data Printer"));
		mMainPanel.add(mKinectPanel);
		mMainPanel.add(printer);

		containPanel.add(infoPanel);
		containPanel.add(speedPanel);
		containPanel.add(bearPanel);

		//laying out the objects
		sl.putConstraint(WEST, mKinectPanel, sPAD_EW , WEST, mMainPanel);
		sl.putConstraint(NORTH, mKinectPanel, 0, NORTH, mMainPanel);
		sl.putConstraint(WEST, printer, sPAD_EW, EAST, mKinectPanel);
		sl.putConstraint(EAST, printer, -sPAD_EW, EAST, mMainPanel);
		sl.putConstraint(NORTH, printer, sPAD_NS, NORTH, mMainPanel);
		sl.putConstraint(SOUTH, printer, -sPAD_NS, SOUTH, mMainPanel);
		
		sl2.putConstraint(NORTH, infoPanel, sPAD_NS, NORTH, containPanel);
		sl2.putConstraint(WEST, infoPanel, sPAD_EW, WEST, containPanel);
		sl2.putConstraint(EAST, infoPanel, -sPAD_EW, EAST, containPanel);
		sl2.putConstraint(NORTH, speedPanel, sPAD_NS, SOUTH, infoPanel);
		sl2.putConstraint(WEST, speedPanel, sPAD_EW, WEST, containPanel);
		sl2.putConstraint(EAST, speedPanel, -sPAD_EW, EAST, containPanel);
		sl2.putConstraint(NORTH, bearPanel, sPAD_NS, SOUTH, speedPanel);
		sl2.putConstraint(WEST, bearPanel, sPAD_EW, WEST, containPanel);
		sl2.putConstraint(EAST, bearPanel, -sPAD_EW, EAST, containPanel);
		
		return mMainPanel;

	}
	
	public void updateHeight()
	{
		if(mOverHeight)
		{
			mMainPanel.setBackground(Color.YELLOW);
		}
		else
			mMainPanel.setBackground(Color.gray);
	}
	
	public void setControl(CompetitionDisplay2013Control pControl)
	{
		mControl = pControl;
	}

	@Override
	public void notifyOfUpdate()
	{
		RobotData2013 data = mControl.getModel().getLatestRobotData();
		mPrinter.printObject(data);
		String date = DateFormat.getDateInstance().format(new Date(data.time));
		UtilityStatusBar.setText(date);
		// XXX HACK -- we're encoding or decoding these values backwards...
		tiltLabel.setText(data.kinect.mKinectWheelSpeed  + "");
		wheelLabel.setText(data.kinect.mKinectAngle+ "%");
		bearLabel.setText(data.kinect.mKinectBearing + "");
		
		mKinectPanel.update(data);
		double mLimit = mMaster.getSystemConfiguration().getDouble(LIMIT_UPPER);
		if(data.sensors.mTiltPotentiometerValue > mLimit || data.pneumatics.mHangLiftState)
		{
			mOverHeight = true;
		}
		else
			mOverHeight =false;
		updateHeight();
	}
	
	public void setDataModel(CompetitionDisplay2013Model pModel)
	{
		mDataModel = pModel;
	}

	public void setSensorValue(final SensorValue pValue)
	{
		mValue = pValue.toString();
		SwingUtilities.invokeLater(new Runnable()
		{

			@Override
			public void run()
			{
				tiltLabel.setText(mValue);
			}
		});
	}

	/*package*/ void initLowerRightWindow()
	{

		Window w = mMaster.getStateData().getParentWindow(); 
		popUp = new JWindow(w); 
		popUp.setLocationRelativeTo(w);
		popUp.setContentPane(containPanel);
		popUp.pack();
		Dimension d = mMaster.getStateData().getParentWindowSize();
		popUp.setSize(new Dimension(d.width- 1045, 256 ));
		popUp.setVisible(true);
		popUp.setLocation(1045, d.height);
		popUp.setAutoRequestFocus(true);
	}

	public void setHopState(SensorValue pHopState)
	{

	}

	public JComponent getOptionsComponent()
	{
		return mConnectionStatusPanel.getPanel();
	}

	/*package*/ void setConnectionStatusPanel(ConnectionStatusPanel pPanel)
	{
		mConnectionStatusPanel = pPanel;
	}

  /*package*/ KinectImagePanel getKinectPanel()
  {
    return mKinectPanel;
  }
}
