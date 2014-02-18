package ilite.display.modules.display2012;
import ilite.display.Constants;
import ilite.display.interfaces.fileio.ImageWriter;
import ilite.display.omi.CameraImagePanel;
import ilite.util.image.ImageDecoder;

import java.awt.Color;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

public class DriverStationManagement
{
	private JFrame frame;
	
	@SuppressWarnings("unused")
//	private DiagnosticOutput diag;
	
//	private JPanel mDiagnosticTab;
//	private ControlPanel mMainControlTab;
//	private JPanel mGameViewTab;
	private JPanel mCameraViewTab;
//	private DataLogger mGameInputLogger;
	
	private JTabbedPane mTabbedPane;
	
	private CameraImagePanel mCamDisplay;
	
//	private ZYSideView sideView;
//	private TopDownView birdEyeView;
	
	final int frameWidth = 1000;
	final int frameHeight = 750;
	final int imageWidth = 640;
	final int imageHeight = 480;

	private static final int JPEGMarker = 0xDEADBEEF;
	private static final int DEBUGMarker = 0xCAFEFACE;
	
	private static final String sIP_ADDRESS = "10.18.85.2";
	private static final int sPORT = 1180;
	
	private static final DriverStationManagement instance = new DriverStationManagement();
	public static DriverStationManagement inst(){ return instance; }
	private DriverStationManagement(){}
	
	public void init(){
		
//		mGameInputLogger = new DataLogger();
		frame = new JFrame("Tabs");
		
		mTabbedPane = new JTabbedPane();
		
//		mDiagnosticTab = new JPanel();
//		mMainControlTab = new ControlPanel(inputClient);
//		mGameViewTab = new JPanel();
		mCameraViewTab = new JPanel();
    ImageDecoder decoder = new ImageDecoder();
		
//		diag = new DiagnosticOutput(mDiagnosticTab);
		mCamDisplay = new CameraImagePanel(imageWidth, imageHeight);
		decoder.addListener(mCamDisplay.getUpdatingImage());
    ImageWriter writer = new ImageWriter(Constants.getDataDirectory(), "TESTING", Constants.getImageFormat());
//		sideView = new ZYSideView();
//		birdEyeView = new TopDownView();
		mCamDisplay.addHorizontalCursor("RedHorzCursor", Color.red);
		mCamDisplay.addVerticalCursor("YellowVertCursor", Color.YELLOW);
		mCamDisplay.addVerticalCursor("RedVertCursor", Color.RED);
		
//		mGameViewTab.add(sideView);
//		mGameViewTab.add(birdEyeView);
		
		mCameraViewTab.add(mCamDisplay.getPanel());		

//		mTabbedPane.addTab("Control Tab",  mMainControlTab);
//		mTabbedPane.addTab("Diagnostics Tab",  mDiagnosticTab);
//		mTabbedPane.addTab("Game Tab",  mGameViewTab);
		mTabbedPane.addTab("Camera Tab", mCameraViewTab);
		
		frame.add(mTabbedPane);
		
		frame.setSize(frameWidth, frameHeight);
		frame.setVisible(true);
		frame.setResizable(false);
		
		frame.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.out.println("Terminated.");
//				mGameInputLogger.close();
				System.exit(0);
			}
		});

		MessageParser2012 parser = new MessageParser2012();
		parser.connect(sIP_ADDRESS, sPORT);
//		parser.setDataLogger(mGameInputLogger);
		decoder.addListener(writer);
		parser.registerProcessor(JPEGMarker, decoder);
		parser.registerProcessor(DEBUGMarker, new cRioStringProcessor());
	}
	
	public static void main(String[] args) throws Exception{
		System.out.println("loading...");
		DriverStationManagement.inst().init();
		System.out.println("running");
	}
}