package ilite.display.modules.display2012;

import ilite.display.interfaces.net.TCPClient;

import java.awt.Graphics;
import java.awt.GridLayout;

import javax.swing.JLabel;
import javax.swing.JPanel;

@SuppressWarnings("serial")
public class ControlPanel extends JPanel {
	ButtonControl mContiuousConnectButton;
	ButtonControl mConnectButton;
	JLabel[] mStatusLabels;
	TCPClient mHeldClient;
	
	public ControlPanel(TCPClient newClient) {
		mHeldClient = newClient;
		mStatusLabels = new JLabel[4];
		mStatusLabels[0] = new JLabel();
		mStatusLabels[1] = new JLabel();
		mStatusLabels[2] = new JLabel();
		
		this.setLayout(new GridLayout(2, 3));
		
		this.add(mStatusLabels[0]);
		this.add(mStatusLabels[1]);
		this.add(mStatusLabels[2]);
		
		mConnectButton = new ButtonControl(mHeldClient, this, true);
		this.add(new JLabel());
		mContiuousConnectButton = new ButtonControl(mHeldClient, this, false);
	}
	
	public void updateLabels() {
		mStatusLabels[0].setText("Connect Active: " + ButtonControl.mNormalConnect);
		mStatusLabels[1].setText("Continuous Connect Active: " + ButtonControl.mAttemptingConnect);
		mStatusLabels[2].setText("Connect Status: " + (mHeldClient.isConnected() ? "Connected" : "Not Connected") );
	}
	
	public void paint(Graphics g)

	{
		g.fillRect(0, 0, 10, 10);
	}
}
