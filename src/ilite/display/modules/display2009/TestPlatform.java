package ilite.display.modules.display2009;

import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.nio.ByteBuffer;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class TestPlatform extends JFrame{
	static final long serialVersionUID = 10;
	Container mcMainContentPane = null;
	
	JTextField		mcFileName;
	JLabel	   		mcFileSaveLabel;
	JPanel	   		mcFirstRow;
	JPanel	   		mcSecondRow;
	JPanel			mcThridRow;
	JButton	   		mcStartFileSaver;
	JButton			mcSaveTestData;
	JButton			mcExit;
	JButton			mcReadTest;
	
	FRC_HostDataSaver mcFRCDataObject = null;
	
	public TestPlatform(){
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		mcMainContentPane = getContentPane();
		mcMainContentPane.setLayout(new BoxLayout(mcMainContentPane,BoxLayout.Y_AXIS));
		
		//Create and Add panels to main frame
		mcFirstRow = new JPanel();
		mcSecondRow = new JPanel(); 
		mcThridRow  = new JPanel();
		mcMainContentPane.add(mcFirstRow);
		mcMainContentPane.add(mcSecondRow);
		//set the layout manager's for the panels to be 
		mcFirstRow.setLayout(new BoxLayout(mcFirstRow,BoxLayout.X_AXIS));
		mcSecondRow.setLayout(new BoxLayout(mcSecondRow,BoxLayout.X_AXIS));
		mcThridRow.setLayout(new BoxLayout(mcThridRow,BoxLayout.X_AXIS));
		
		mcFileSaveLabel = new JLabel("File Name:");
		mcFileName = new JTextField(10);
		mcStartFileSaver = new JButton("Start");
		mcStartFileSaver.addActionListener(new HandleSaveFile(this));	
		
		
		mcSaveTestData = new JButton("save Test data");
		mcSaveTestData.addActionListener(new SaveTestData(this));
		mcExit = new JButton("Exit");
		mcExit.addActionListener(new ExitApp());
		//add elements to the panels
		mcFirstRow.add(mcSaveTestData);
		mcFirstRow.add(mcExit);
		
		
		mcSecondRow.add(mcFileSaveLabel);
		mcSecondRow.add(mcFileName);
		mcSecondRow.add(mcStartFileSaver);
		
		
		//showMessageDialog();
		
		//pack() needs to be the last element in this constructor to size GUI
		//correctly
		pack();
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		TestPlatform lcTest = new TestPlatform();
		lcTest.setVisible(true);

	}
	
	//inner classes to handle button events
	protected class ExitApp implements ActionListener{
		public void actionPerformed(ActionEvent e) { 
			if(mcFRCDataObject != null){
				mcFRCDataObject.STOP();
			}
			try{
				while (mcFRCDataObject.isAlive()) {
					Thread.sleep(20);
				}
			}catch(InterruptedException E){
				//do nothing
			}
			System.out.println("application ending");
			System.exit(0);
		}
	}
	protected class SaveTestData implements ActionListener{
		private JFrame mcFrame;
		public SaveTestData(JFrame aOwningFrame){
			mcFrame = aOwningFrame;
		}
		public void actionPerformed(ActionEvent e) { 
			if(mcFRCDataObject == null){
				JOptionPane.showMessageDialog(mcFrame,"File object not ready");
			}
			else{
				ByteBuffer lcTestData = ByteBuffer.allocate(40);
				for(int i = 0; i < 10; i++){
					lcTestData.putInt(i);
				}
				for(int j = 0;j < 20000; j++){
					mcFRCDataObject.newDatatoSave(lcTestData.array());
				}

			}
		}
	}
	protected class HandleSaveFile implements ActionListener{
		private JFrame mcFrame;
		public HandleSaveFile(JFrame aOwningFrame){
			mcFrame = aOwningFrame;
		}
		public void actionPerformed(ActionEvent e) { 
			
           //check to determine if the file name field is not empty
			String lcFileName = mcFileName.getText();
			if(lcFileName.length() == 0){
				JOptionPane.showMessageDialog(mcFrame,"File name is empty");
			}
			else{
				if(mcFRCDataObject == null){
					mcFRCDataObject = new FRC_HostDataSaver();
					mcFRCDataObject.init(lcFileName);
				}else{
					JOptionPane.showMessageDialog(mcFrame,"A file Saver is already running");
				}
			}
        }

	}

}
