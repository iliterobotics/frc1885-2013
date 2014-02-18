package ilite.display.omi;

import ilite.display.StateData;
import ilite.display.interfaces.net.EConnectionState;
import ilite.display.interfaces.net.IRobotConnector;
import ilite.display.omi.components.IliteExpandingPanel;
import ilite.util.gui.components.EPanelOrientation;
import ilite.util.lang.IUpdate;

import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.SpringLayout;

public class ConnectionStatusPanel extends IliteExpandingPanel
{
  private JLabel mConnectionStatusLabel = new JLabel(EConnectionState.DISCONNECTED.name());
  private JButton mConnectButton = new JButton("CONNECT");
  private JButton mDisconnectButton = new JButton("DISCONNECT");
  private JLabel connectionLabel = new JLabel("Robot Connection Status: ");
  private boolean mShowHelpingLabel = true;

  public ConnectionStatusPanel(EPanelOrientation pOrientation,
      final IRobotConnector pControl, boolean pShowHelpingLabel, StateData pStateData)
  {
    super(pOrientation);
    mShowHelpingLabel = pShowHelpingLabel;

    mPanel.setPreferredSize(getPanelPreferredSize());

    SpringLayout sl = new SpringLayout();
    SpringLayout psl = new SpringLayout();
    mPanel.setLayout(sl);
    mPopupPanel.setLayout(psl);

    mPanel.add(mConnectionStatusLabel);
    mPopupPanel.add(mConnectButton);
    mPopupPanel.add(mDisconnectButton);
    if(mShowHelpingLabel)
    {
      mPanel.add(connectionLabel);
      sl.putConstraint(SpringLayout.WEST, connectionLabel, INSET,
          SpringLayout.WEST, mPanel);
      sl.putConstraint(SpringLayout.VERTICAL_CENTER, connectionLabel, 0,
          SpringLayout.VERTICAL_CENTER, mPanel);
      sl.putConstraint(SpringLayout.WEST, mConnectionStatusLabel, INSET,
          SpringLayout.EAST, connectionLabel);
    }
    else
    {
      sl.putConstraint(SpringLayout.WEST, mConnectionStatusLabel, INSET,
          SpringLayout.WEST, mPanel);
    }


    sl.putConstraint(SpringLayout.VERTICAL_CENTER, mConnectionStatusLabel, 0,
        SpringLayout.VERTICAL_CENTER, mPanel);

    psl.putConstraint(SpringLayout.SOUTH, mConnectButton, -INSET,
        SpringLayout.SOUTH, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mConnectButton, INSET,
        SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.EAST, mConnectButton, -INSET,
        SpringLayout.EAST, mPopupPanel);

    psl.putConstraint(SpringLayout.SOUTH, mDisconnectButton, -INSET,
        SpringLayout.NORTH, mConnectButton);
    psl.putConstraint(SpringLayout.WEST, mDisconnectButton, INSET,
        SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.EAST, mDisconnectButton, -INSET,
        SpringLayout.EAST, mPopupPanel);

    mConnectButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent arg0)
      {
        mConnectButton.setEnabled(false);
        mDisconnectButton.setEnabled(true);
        pControl.attemptRobotConnection();
      }
    });

    mDisconnectButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent arg0)
      {
        mConnectButton.setEnabled(true);
        mDisconnectButton.setEnabled(false);
        pControl.disconnectRobot();
      }
    });

    pStateData.addRobotConnectionStatusListener(new ConnectionStatusUpdater());
    setConnectionState(pStateData.getRobotConnectionStatus());
  }

  protected Dimension getPopupPreferredSize()
  {
    
    int w = Math.max(mPanel.getPreferredSize().width, mDisconnectButton.getPreferredSize().width);
    return new Dimension(w,
        mDisconnectButton.getPreferredSize().height * 2 + 3 * INSET);
  }

  @Override
  protected Dimension getPanelPreferredSize()
  {
    FontMetrics mMetrics = connectionLabel.getFontMetrics(connectionLabel
        .getFont());
    int w = mMetrics.stringWidth(EConnectionState.DISCONNECTED.name());
    if(mShowHelpingLabel)
    {
      w += mMetrics.stringWidth(connectionLabel.getText());
    }
    return new Dimension(w + 4 * INSET, (int) mMetrics.getHeight() + 2 * INSET);
  }

  private void setConnectionState(EConnectionState pState)
  {
    mConnectionStatusLabel.setText(pState.name());
    mConnectionStatusLabel.setForeground(pState.color());
    boolean enableConnectButton = (pState == EConnectionState.DISCONNECTED || pState == EConnectionState.ERROR);
    mConnectButton.setEnabled(enableConnectButton);
    mDisconnectButton.setEnabled(!enableConnectButton);
  }

  private class ConnectionStatusUpdater implements IUpdate<EConnectionState>
  {
    @Override
    public void update(EConnectionState pState)
    {
      setConnectionState(pState);
    }

  }

}
