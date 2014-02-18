package ilite.display.modules.display2014;


import static javax.swing.SpringLayout.*;
import ilite.display.camera.AxisCameraImpl2014;
import ilite.display.interfaces.net.EConnectionState;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;
import ilite.util.physics.concepts.Hertz;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.net.MalformedURLException;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.SpringLayout;

public class CameraFeedConfigPanel
{
  private JLabel iplabel = new JLabel("IP Address:", JLabel.RIGHT);
  private JLabel passlabel = new JLabel("Password:", JLabel.RIGHT);
  private JLabel userlabel = new JLabel("User:", JLabel.RIGHT);
  private JLabel mFrameRateLabel = new JLabel("Frame Rate (Hz):", JLabel.RIGHT);
  private JLabel mConnectionStateLabel = new JLabel("Connection Status:",JLabel.RIGHT);
  private JTextField mUsernameField = new JTextField("root");
  private JPasswordField mUserpassField = new JPasswordField();
  private JTextField mCameraIpField = new JTextField("10.18.85.21");
  private JTextField mFrameRateField = new JTextField("20");
  private JButton mConnectButton = new JButton("Connect");
  private JButton mDisconnectButton = new JButton("Disconnect");
  private JLabel mConnectionState = new JLabel(EConnectionState.DISCONNECTED.name());
  private AxisCameraImpl2014 mCamera = null;
  private final Timer mConnectionWatchdog = new Timer("Camera Connection Watchdog");
  private ILog mLog = Logger.createLog(CameraFeedConfigPanel.class);
  private boolean mIsEnabled = false;
  private final JPanel mMainPanel = new JPanel();
  
  public CameraFeedConfigPanel()
  {
    mUserpassField.setEchoChar('*');
    mUserpassField.setText("team1885");
    
    mConnectButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        if(mCamera != null)
        {
          try
          {
            mCamera.setIpAddress(mCameraIpField.getText());
            mCamera.setFrameRate(Hertz.fromRate(Float.parseFloat(mFrameRateField.getText())));
            mCamera.setUserPass(mUsernameField.getText(), new String(mUserpassField.getPassword()));
            mCamera.connect();
          } catch (MalformedURLException e1)
          {
            mLog.error(e1.getMessage());
            mLog.exception(e1);
          }
        }
      }
    });
    mDisconnectButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        if(mCamera != null && mCamera.getConnectionState().isConnected())
        {
          mCamera.disconnect();
        }
      }
    });
    mConnectionWatchdog.scheduleAtFixedRate(new ConnectionWatchdogTask(), 500, 500);
    
    SpringLayout sl = new SpringLayout();
    mMainPanel.setLayout(sl);
    mMainPanel.setBorder(BorderFactory.createTitledBorder("Override Camera Connection"));
    for(JComponent c : new JComponent[]{mUsernameField, mUserpassField, 
        mCameraIpField, mFrameRateField, mConnectButton, mDisconnectButton,
        iplabel, passlabel, userlabel, mFrameRateLabel, mConnectionStateLabel,
        mConnectionState})
    {
      c.setEnabled(false);
      mMainPanel.add(c);
    }
    mMainPanel.setEnabled(false);
    int pad = 5;
    sl.putConstraint(NORTH, iplabel, pad, NORTH, mMainPanel);
    sl.putConstraint(NORTH, mCameraIpField, pad, NORTH, mMainPanel);
    sl.putConstraint(WEST, iplabel, pad, WEST, mMainPanel);
    sl.putConstraint(EAST, iplabel, -pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, mCameraIpField, pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, mCameraIpField, -pad, EAST, mMainPanel);
    
    sl.putConstraint(NORTH, userlabel, pad, SOUTH, mCameraIpField);
    sl.putConstraint(NORTH, mUsernameField, pad, SOUTH, mCameraIpField);
    sl.putConstraint(WEST, userlabel, pad, WEST, mMainPanel);
    sl.putConstraint(EAST, userlabel, -pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, mUsernameField, pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, mUsernameField, -pad, EAST, mMainPanel);
    
    sl.putConstraint(NORTH, passlabel, pad, SOUTH, mUsernameField);
    sl.putConstraint(NORTH, mUserpassField, pad, SOUTH, mUsernameField);
    sl.putConstraint(WEST, passlabel, pad, WEST, mMainPanel);
    sl.putConstraint(EAST, passlabel, -pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, mUserpassField, pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, mUserpassField, -pad, EAST, mMainPanel);
    
    sl.putConstraint(NORTH, mFrameRateLabel, pad, SOUTH, mUserpassField);
    sl.putConstraint(NORTH, mFrameRateField, pad, SOUTH, mUserpassField);
    sl.putConstraint(WEST, mFrameRateLabel, pad, WEST, mMainPanel);
    sl.putConstraint(EAST, mFrameRateLabel, -pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, mFrameRateField, pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, mFrameRateField, -pad, EAST, mMainPanel);
    
    sl.putConstraint(NORTH, mConnectButton, pad, SOUTH, mFrameRateField);
    sl.putConstraint(NORTH, mDisconnectButton, pad, SOUTH, mFrameRateField);
    sl.putConstraint(WEST, mConnectButton, pad, WEST, mMainPanel);
    sl.putConstraint(EAST, mConnectButton, -pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, mDisconnectButton, pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, mDisconnectButton, -pad, EAST, mMainPanel);
    
    sl.putConstraint(NORTH, mConnectionStateLabel, pad, SOUTH, mConnectButton);
    sl.putConstraint(NORTH, mConnectionState, pad, SOUTH, mConnectButton);
    sl.putConstraint(WEST, mConnectionStateLabel, pad, WEST, mMainPanel);
    sl.putConstraint(EAST, mConnectionStateLabel, -pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, mConnectionState, pad, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, mConnectionState, -pad, EAST, mMainPanel);
    
    sl.putConstraint(SOUTH, mMainPanel, pad, SOUTH, mConnectionState);
  }
  
  public JPanel getDisplayPanel()
  {
    return mMainPanel;
  }
  
  /**
   * Sets the connect & disconnect button to enabled if the camera isn't null
   * @param pEnabled
   */
  public void setEnabled(boolean pEnabled)
  {
    for(JComponent c : new JComponent[]{mConnectButton, mDisconnectButton, 
        mConnectionStateLabel, mConnectionState})
    {
      c.setEnabled(pEnabled & mCamera != null);
    }
  }
  
  /**
   * Sets the camera configuration and enables the components which will configure it
   * @param pCamera
   */
  public void setCamera(AxisCameraImpl2014 pCamera)
  {
    for(JComponent c : new JComponent[]{mUsernameField, mUserpassField, 
        mCameraIpField, mFrameRateField, iplabel, passlabel, userlabel, 
        mFrameRateLabel, mConnectButton, mDisconnectButton,
        mConnectionStateLabel, mConnectionState, mMainPanel})
    {
      c.setEnabled(true);
    }
    mCamera = pCamera;
  }

  
  /**
   * @author JesseK
   * Monitors the camera connection and updates the label 
   */
  private class ConnectionWatchdogTask extends TimerTask
  {
    @Override
    public void run()
    {
      if(mCamera != null)
      {
        mConnectionState.setText(mCamera.getConnectionState().name());        
      }
    }
  }
}
