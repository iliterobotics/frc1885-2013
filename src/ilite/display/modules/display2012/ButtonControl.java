package ilite.display.modules.display2012;

import ilite.display.interfaces.net.TCPClient;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;

public class ButtonControl extends Thread implements ActionListener{
	JButton mButton;
	ControlPanel mPlacementPanel;
	boolean mTypeOfButton;
	
	boolean threadConnect;
	boolean threadContinuousConnect;
	
	static boolean mAttemptingConnect;
	static boolean mNormalConnect;
	
	TCPClient inputClient;
	
	public ButtonControl(TCPClient newClient, ControlPanel newPlacementPanel, boolean newTypeOfButton) {
		mTypeOfButton = newTypeOfButton;
		if(mTypeOfButton) {
			mButton = new JButton("Connect");
		} else {
			mButton = new JButton("Continuous Connect");
		}
		mPlacementPanel = newPlacementPanel;
		mButton.addActionListener(this);
		mPlacementPanel.add(mButton);
		
		mAttemptingConnect = false;
		mNormalConnect = false;
		
		inputClient = newClient;
		
		start();
	}

	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		if(mTypeOfButton) {
			threadConnect = true;
		} else {
			threadContinuousConnect = true;
		}
	}
	
	public void run() {
		while(true) {
			if(threadConnect) {
				tryConnection();
			} 
			if(threadContinuousConnect) {
				tryConnectionContinuously();
			}
			
			mPlacementPanel.updateLabels();
		}
	}
	
	public void tryConnectionContinuously() {
		if(!mNormalConnect) {
			mAttemptingConnect = !mAttemptingConnect;
			
			if(mAttemptingConnect) {
				
				try {
					inputClient.mcClientConnected.acquire();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		threadContinuousConnect = false;
	}
	
	public void tryConnection() {
		if(!mAttemptingConnect) {
			mNormalConnect = true;
			
//			inputClient.connect();
			if(inputClient.isConnected()) {
				try {
					inputClient.mcClientConnected.acquire();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} //wait here until lcClient is done talking with server
			}
			mNormalConnect = false;
		}
		threadConnect = false;
	}
}
