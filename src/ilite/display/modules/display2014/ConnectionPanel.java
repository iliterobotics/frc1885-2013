package ilite.display.modules.display2014;

import static javax.swing.SpringLayout.EAST;
import static javax.swing.SpringLayout.HORIZONTAL_CENTER;
import static javax.swing.SpringLayout.NORTH;
import static javax.swing.SpringLayout.SOUTH;
import static javax.swing.SpringLayout.WEST;
import ilite.display.interfaces.net.EConnectionState;
import ilite.util.physics.concepts.Hertz;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SpringLayout;

public class ConnectionPanel extends JPanel
{
  private final Timer mConnectionWatchdog = new Timer("Robot Connection Watchdog");
  private final JLabel mConnectionLabel = new JLabel(EConnectionState.DISCONNECTED.name());
  private final JButton driverButton = new JButton("Start Driver");
  private final JButton connectButton = new JButton("Connect");
  private final JButton disconnectButton = new JButton("Disconnect");
  private final JLabel iplabel = new JLabel("IP Address:");
  private final JLabel ptlabel = new JLabel("Port:");
  private final JTextField ipfield = new JTextField(16);
  private final JTextField ptfield = new JTextField(16);
  private final JLabel driverRateLabel = new JLabel("Rate (Hz): ");
  private final JTextField driverRateField = new JTextField(10);
  private final JLabel connectStatusLabel = new JLabel("Connection Status:");
  private final CameraFeedConfigPanel mCameraConfig = new CameraFeedConfigPanel();
  
  public ConnectionPanel()
  {    
    ipfield.setText(DataManager2014.sDEFAULT_ROBOT_IP);
    ptfield.setText(Integer.toString(DataManager2014.sDEFAULT_ROBOT_PORT));
    driverRateField.setText("10");

    int pad = 3;
    
    SpringLayout sl = new SpringLayout();
    setLayout(sl);
    JPanel connect = createConnectPanel(pad);
    JPanel driver = createDataDriverPanel(pad);
    JPanel match = createMatchPanel(pad);
    JPanel camera = mCameraConfig.getDisplayPanel();
    
    add(connect);
    add(driver);
    add(match);
    add(camera);
    
    
    sl.putConstraint(NORTH, match, pad, NORTH, this);
    sl.putConstraint(NORTH, connect, pad, NORTH, this);
    sl.putConstraint(WEST, match, pad, WEST, this);
    sl.putConstraint(EAST, match, -pad, HORIZONTAL_CENTER, this);
    sl.putConstraint(WEST, connect, pad, HORIZONTAL_CENTER, this);
    sl.putConstraint(EAST, connect, -pad, EAST, this);
    sl.putConstraint(NORTH, camera, pad, SOUTH, connect);
//    sl.putConstraint(SOUTH, connect, -pad, NORTH, camera);
    sl.putConstraint(WEST, camera, pad, HORIZONTAL_CENTER, this);
    sl.putConstraint(EAST, camera, -pad, EAST, this);
//    sl.putConstraint(SOUTH, camera, -(120+2*pad), SOUTH, this);
    sl.putConstraint(SOUTH, match, -pad, SOUTH, this);
    sl.putConstraint(NORTH, driver, pad, SOUTH, camera);
    sl.putConstraint(SOUTH, driver, -pad, SOUTH, this);
    sl.putConstraint(WEST, driver, pad, HORIZONTAL_CENTER, this);
    sl.putConstraint(EAST, driver, -pad, EAST, this);

    initListeners();
    mConnectionWatchdog.scheduleAtFixedRate(new ConnectionWatchdogTask(), 500, 500);
  }
  
  /**
   * @return the camera configuration panel, usually to set it enabled
   */
  CameraFeedConfigPanel getCameraConfigPanel()
  {
    return mCameraConfig;
  }
  
  private JPanel createConnectPanel(int pad)
  {
    SpringLayout sl = new SpringLayout();
    JPanel connectPanel = new JPanel(sl);
    connectPanel.setBorder(BorderFactory.createTitledBorder("Live Robot Connection"));
    connectPanel.add(iplabel);
    connectPanel.add(ipfield);
    connectPanel.add(ptlabel);
    connectPanel.add(ptfield);
    connectPanel.add(connectButton);
    connectPanel.add(disconnectButton);
    connectPanel.add(mConnectionLabel);
    connectPanel.add(connectStatusLabel);
    sl.putConstraint(NORTH, iplabel, pad, NORTH, connectPanel);
    sl.putConstraint(NORTH, ipfield, pad, NORTH, connectPanel);
    sl.putConstraint(EAST, iplabel, -pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(WEST, ipfield, pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(NORTH, ptlabel, pad, SOUTH, iplabel);
    sl.putConstraint(NORTH, ptfield, pad, SOUTH, ipfield);
    sl.putConstraint(EAST, ptlabel, -pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(WEST, ptfield, pad, HORIZONTAL_CENTER, connectPanel);
    
    sl.putConstraint(NORTH, connectButton, pad, SOUTH, ptfield);
    sl.putConstraint(WEST, connectButton, pad, WEST, connectPanel);
    sl.putConstraint(EAST, connectButton, -pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(NORTH, disconnectButton, pad, SOUTH, ptfield);
    sl.putConstraint(WEST, disconnectButton, pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(EAST, disconnectButton, -pad, EAST, connectPanel);
    
    sl.putConstraint(NORTH, connectStatusLabel, pad, SOUTH, disconnectButton);
    sl.putConstraint(EAST, connectStatusLabel, -pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(NORTH, mConnectionLabel, pad, SOUTH, disconnectButton);
    sl.putConstraint(WEST, mConnectionLabel, pad, HORIZONTAL_CENTER, connectPanel);
    sl.putConstraint(SOUTH, connectPanel, pad, SOUTH, connectStatusLabel);
    return connectPanel;
  }
  
  private JPanel createDataDriverPanel(int pad)
  {
    SpringLayout sl = new SpringLayout();
    JPanel driverControl = new JPanel(sl);
    driverControl.setBorder(BorderFactory.createTitledBorder("Data Driver"));
    driverControl.add(driverRateLabel);
    driverControl.add(driverRateField);
    driverControl.add(driverButton);
    sl.putConstraint(NORTH, driverRateLabel, pad, NORTH, driverControl);
    sl.putConstraint(EAST, driverRateLabel, -pad, HORIZONTAL_CENTER, driverControl);
    sl.putConstraint(NORTH, driverRateField, pad, NORTH, driverControl);
    sl.putConstraint(WEST, driverRateField, pad, HORIZONTAL_CENTER, driverControl);
    sl.putConstraint(NORTH, driverButton, pad, SOUTH, driverRateLabel);
    sl.putConstraint(HORIZONTAL_CENTER, driverButton, 0, HORIZONTAL_CENTER, driverControl);
    return driverControl;
  }
  
  private JPanel createMatchPanel(int pad)
  {
    return new MatchSelectionPanel(pad).getDisplayPanel();
  }
  
  private void initListeners()
  {
    driverButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        DataManager2014.getInstance().setDataDriverRate(
            Hertz.fromRate(Float.parseFloat(driverRateField.getText())));
        if(driverButton.getText().equals("Start Driver"))
        {
          driverButton.setText("Stop Driver");
          DataManager2014.getInstance().setDriverEnabled(true);
        }
        else
        {
          driverButton.setText("Start Driver");
          DataManager2014.getInstance().setDriverEnabled(false);
        }
      }
    });
    connectButton.addActionListener(new ActionListener()
    {      
      @Override
      public void actionPerformed(ActionEvent e)
      {
        String ip = ipfield.getText();
        int port = Integer.parseInt(ptfield.getText());
        DataManager2014.getInstance().connect(ip, port, 1000, DataManager2014.sDEFAULT_RETRY_SLEEP);
      }
    });
    disconnectButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        DataManager2014.getInstance().disconnectRobot();
      }
    });
  }
  
  private class ConnectionWatchdogTask extends TimerTask
  {
    @Override
    public void run()
    {
      mConnectionLabel.setText(
          DataManager2014.getInstance().getConnectionState().name());
    }
  }
}
