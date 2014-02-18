package ilite.display.modules.display2009;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.RenderedImage;
import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
//import ham90mack.swingUtils.GridBagLayoutFrame;
//import java.util.ArrayList;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class laptopGUI extends JFrame implements ActionListener{
	static final long serialVersionUID = 1234590;

	JLabel ip, socket;
	JPanel mainScreen;
	JTextField ipField, socketField;
	JLabel image;
	
	JButton connectButton;
	JButton saveImage;
	
	static boolean savingData=false;
	
  RobotStateManager mcRobotStateManager;
	
	JTextArea mcDriverStationUDPData;
	JScrollPane mcDriverStationUDPDataScroll;	
	datagramStuff mcDriverStationData;
	tcpImageClient mcCameraImage;
	int mnCount = 0;
	
	JTabbedPane mcTabbedPane;
	JComponent mcCameraPanel;
	JComponent mcUserDataPanel;
	JComponent mcRobotStatePanel;
	//This might screw up everything
	TargetView mcTargetViewPanel;
	DriverVisuals mcDriverVisualsPanel;
	
	UDPDataInterpreter mcDataInterpreter;
	
	boolean mbDisplayData = true;
	int mnNumUDPMsgElementsDisplayLimit = 50;

	protected GridBagLayout mcGblCameraPanel = new GridBagLayout();
	protected GridBagConstraints mcGbc = new GridBagConstraints();
	protected GridBagLayout mcGblUserDataPanel = new GridBagLayout();
  
	
	JButton mcDisplayMoreUDPData;
	JTextField mcNumElementsToParse; 
	 
  Image img=null;
	 
	
	
	class handleUserDataState implements ActionListener{
		public void actionPerformed(ActionEvent e) {
			mbDisplayData = true;	
			mnNumUDPMsgElementsDisplayLimit = 
				Integer.parseInt(mcNumElementsToParse.getText());
		}
	}
	/**
	 * Modifies the variables in the GBL constraints and adds the component to
	 * the content pane so the component is added to the correct place.
	 * 
	 * @param c
	 *            the Component to add to the window
	 * @param y
	 *            the row to put the component in
	 * @param x
	 *            the column to put the component in
	 * @param w
	 *            how many cells wide the component will be
	 * @param h
	 *            how many cells tall the component will be
	 */
   public laptopGUI() {
    super("Robot Host Laptop System");
    
    mcGbc.weightx = 100;
    mcGbc.weighty = 100;
    mcGbc.insets.bottom = 5;
    mcGbc.insets.left = 5;
    mcGbc.insets.right = 5;
    mcGbc.insets.top = 5;
    mcGbc.fill = GridBagConstraints.NONE;
    mcGbc.anchor = GridBagConstraints.NORTHWEST;

    //Create a Tabbed Pane to place for the system
    mcTabbedPane = new JTabbedPane();
    add(mcTabbedPane);

    //create Panels for the Camera and Use data from the Robot 
    mcCameraPanel = makeTextPanel("Camera");
    mcUserDataPanel = makeTextPanel("Raw User Data");
    mcRobotStatePanel = makeTextPanel("Robot State");
    mcDriverVisualsPanel = new DriverVisuals(); 
    mcTargetViewPanel = new TargetView();
    
    //add the Panels to the Tabbed Pane
    mcTabbedPane.addTab("Driver Visuals", mcDriverVisualsPanel);
    mcTabbedPane.addTab("Camera", mcCameraPanel);
    mcTabbedPane.addTab("Raw User Data", mcUserDataPanel);
    mcTabbedPane.addTab("Robot State", mcRobotStatePanel);
    mcTabbedPane.addTab("Target View", mcTargetViewPanel);
    
    // create new layout managers for the Panels
    mcCameraPanel.setLayout(mcGblCameraPanel);
    mcUserDataPanel.setLayout(mcGblUserDataPanel);
    
    
    
    // Add the tabbed pane to the Frame
    add(mcTabbedPane);
    
    
    // The following line enables to use scrolling tabs.
    mcTabbedPane.setTabLayoutPolicy(JTabbedPane.SCROLL_TAB_LAYOUT);

    //call the following functions to add components needed for each of the
    //Panels
    GuiElementsforCameraPanel();
    GuiElementsForUserDataPanel();
      
    mcRobotStateManager = new RobotStateManager(mcRobotStatePanel);
    
    mcDataInterpreter = new UDPDataInterpreter(mcTargetViewPanel, mcRobotStateManager, mcDriverVisualsPanel);
        
    pack();
    setVisible(true);
    setLocationRelativeTo(null);
    
    mcDriverStationData = new datagramStuff(this);
    mcDriverStationData.start();
    
    System.out.println("Laptop GUI constructor done");

  }
	protected void addCameraPanel(Component c, int y, int x, int w, int h) {
		// //setup constraints
		mcGbc.gridx = x - 1;
		mcGbc.gridy = y - 1;
		mcGbc.gridwidth = w;
		mcGbc.gridheight = h;
		// apply constraints to layout
		mcGblCameraPanel.setConstraints(c, mcGbc);
		// add new element to Panel
		mcCameraPanel.add(c);
	}
	
  protected void addUserDataPanel(Component c, int y, int x, int w, int h)
  {
    //setup constraints
    mcGbc.gridx = x - 1;
    mcGbc.gridy = y - 1;
    mcGbc.gridwidth = w;
    mcGbc.gridheight = h;
    // apply constraints to layout
    mcGblUserDataPanel.setConstraints(c, mcGbc);
    // add new element to Panel
    mcUserDataPanel.add(c);
  }  
    
  protected void GuiElementsforCameraPanel() {
    ip = new JLabel("IP");
    socket = new JLabel("Port:");

    ipField = new JTextField(8);
    ipField.setText("10.18.85.2");
    socketField = new JTextField(8);
    socketField.setText("1180");
    
    image = new JLabel();
    
    connectButton = new JButton("Connect");
    connectButton.addActionListener(this);
    saveImage = new JButton("Save image");
    saveImage.addActionListener(new CaptureCameraListener());
    
    

    // gbc.anchor = GridBagConstraints.WEST;

    addCameraPanel(ip, 1, 1, 1, 1);
    addCameraPanel(ipField, 1, 2, 1, 1);
    addCameraPanel(socket, 1, 3, 1, 1);
    addCameraPanel(socketField, 1, 4, 1, 1);
    addCameraPanel(image,2,1,4,1);
    addCameraPanel(connectButton,1,5,1,1);
    addCameraPanel(saveImage,2,5,1,1);
    
    // gbc.insets.left = 40;
    // gbc.insets.right = 40;
    
  }
	
	protected void GuiElementsForUserDataPanel()
	{
		mcDriverStationUDPData = new JTextArea(15, 60);
		mcDriverStationUDPData.setEditable(false);
    mcDriverStationUDPDataScroll = new JScrollPane(mcDriverStationUDPData);
		addUserDataPanel(mcDriverStationUDPDataScroll, 1, 1, 1, 1);
		
		mcDisplayMoreUDPData = new JButton("Display another Set of Data");
		addUserDataPanel(mcDisplayMoreUDPData, 2, 1, 1, 1);
		mcDisplayMoreUDPData.addActionListener(new handleUserDataState());
		
		mcNumElementsToParse = new JTextField(10);
		mcNumElementsToParse.setText(Integer.toString(mnNumUDPMsgElementsDisplayLimit));
		addUserDataPanel(mcNumElementsToParse,2,2,10,1);

	}
   
	public void newJpegImage(byte[] bytes) 
	{
     img=null;
     
        try {
          // Read from an input stream
          ByteArrayInputStream data = new ByteArrayInputStream(bytes);
          InputStream is = new BufferedInputStream(data);
          img = ImageIO.read(is);
          image.setIcon(new ImageIcon(img));
        
          repaint();
          
        } catch (IOException e) {
          // TODO: handle exception!
        }
		// setVisible(true);
	}

	public void newUDPData(byte[] aaUserData) {


		
		mnCount++;

		//System.out.println("Received " + aaUserData.length
		//		+ "bytes from Driver Station");
		
		if(mbDisplayData== true){
			//mbDisplayData = false;
      ByteBuffer lcByteBuffer = ByteBuffer.wrap(aaUserData);
      ByteBuffer lcDataBuffer = ByteBuffer.wrap(aaUserData, 26, aaUserData.length-26);
      ByteBuffer lcFilePrint = lcDataBuffer.asReadOnlyBuffer();
			
			mcDataInterpreter.interpretData(lcDataBuffer);
			
			if(savingData==true && isValid(aaUserData))
			{
				System.out.println("Sending save data");
				//TODO _JAK
//		      mcDriverVisualsPanel.dataPanel.controlPanel.dataSaver.newDatatoSave(aaUserData);
			}
        
			
			
			int lcCurrentInt;

			// TODO: Remove The byteorder change if data is coming in as we expect
			// it to
			// set the byte buffer to the correct order
			// informs the byte buffer the order data is coming in, in aaUserData
			// This will allow the existing infrastructure to automatically handle
			// conversions for us
			//lcByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
			mcDriverStationUDPData.setText("");
			mcDriverStationUDPData.setText("Received " + aaUserData.length
          + "bytes from Driver Station Displaying " + mnNumUDPMsgElementsDisplayLimit
            + " : count = " + mnCount + "\n");
			int cnt = 0;
			while (lcByteBuffer.remaining() >= 4) {
        lcCurrentInt = lcByteBuffer.getInt();
				mcDriverStationUDPData.append(Integer.toHexString(lcCurrentInt)
						+ " ");
				cnt++;

				if (cnt == 10) {
          mcDriverStationUDPData.append("\n");
          cnt = 0;
				}
				// mcDriverStationUDPData.append(" ");
			}
		}
		// mcDriverStationUDPData.setText(lcDataDisplay.toString());
		//pack();

	}

	protected JComponent makeTextPanel(String text) {
		JPanel panel = new JPanel(false);
		/*
		 * JLabel filler = new JLabel(text);
		 * filler.setHorizontalAlignment(JLabel.CENTER); panel.setLayout(new
		 * GridLayout(1, 1)); panel.add(filler);
		 */
		return panel;
	}
	
  public void actionPerformed(ActionEvent e) 
  {
    if(e.getSource()==connectButton&&mcCameraImage == null)
    {
      mcCameraImage = new tcpImageClient(ipField.getText(),new Integer(socketField.getText()),this);
      mcCameraImage.start();
    } 
  }
  public class CaptureCameraListener implements ActionListener
  {
    public void actionPerformed(ActionEvent e)
    {
      /*
      BufferedImage whatever=null;
      try
      {
        whatever = ImageIO.read(new File("testing.jpg"));
      }
      catch(Exception except)
      {
        except.printStackTrace();
      }
      
      img=whatever;*/
      
      if(img!=null)
      {
        synchronized(img)
        {
          try
          {
            ImageIO.write((RenderedImage)img,"PNG",new File("cameraOutput"+System.currentTimeMillis()+".png"));
          }  
          catch(Exception except)
          {
             except.printStackTrace();
          } 
        }
      }
    }   
  }
                     
	/*
	 * public Image byteArrayToImage(byte[] byteArrayIn){ MemoryStream ms = new
	 * MemoryStream(byteArrayIn); Image returnImage = Image.FromStream(ms);
	 * return returnImage; }
	 * 
	 * public static byte[] getBytesFromFile(File file) throws IOException {
	 * InputStream is = new FileInputStream(file); // Get the size of the file
	 * long length = file.length();
	 * 
	 * if (length > Integer.MAX_VALUE){ // File is too large } // Create the
	 * byte array to hold the data byte[] bytes = new byte[(int)length]; // Read
	 * in the bytes int offset = 0; int numRead = 0; while (offset <
	 * bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >=
	 * 0) { offset += numRead; } // Ensure all the bytes have been read in if
	 * (offset < bytes.length) { throw new IOException("Could not completely
	 * read file "+file.getName()); } // Close the input stream and return bytes
	 * is.close(); return bytes; }
	 */
	 
	 private boolean isValid(byte[] array)
	 {
	   int offset=26;
	   
     if(array[offset+0]==(byte)0xCA&&array[offset+1]==(byte)0xFE&&array[offset+2]==(byte)0xBE&&array[offset+3]==(byte)0xAD)
     {  
       if(array[offset+4]==(byte)0xDE&&array[offset+5]==(byte)0xAD&&array[offset+6]==(byte)0xFA&&array[offset+7]==(byte)0xCE)
         return true;
       else
         return false;
     }
     else
       return false;
	 }
	public static void main(String[] args) throws IOException {
		laptopGUI laptop = new laptopGUI();
	  //laptop.setBounds(0,0,825,650);
	  laptop.pack();

	}
	
  
}