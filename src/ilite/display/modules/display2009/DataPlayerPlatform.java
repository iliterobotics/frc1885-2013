package ilite.display.modules.display2009;

import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

public class DataPlayerPlatform extends JFrame {
	static final long serialVersionUID = 11;

	Container mcMainContentPane = null;

	JPanel mcFirstRow;

	JPanel mcSecondRow;

	JPanel mcThridRow;

	JTextField mcFileName;

	JLabel mcFileSaveLabel;

	JButton mcLoadFile;

	JButton mcPlayData;

	JButton mcPausePlay;

	JButton mcStopPlay;

	JButton mcResetToBeginning;

	JProgressBar mcFilePlayProgress;

	FRC_HostDataReader mcDataReader = null;

	public DataPlayerPlatform() {
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		mcMainContentPane = getContentPane();
		mcMainContentPane.setLayout(new BoxLayout(mcMainContentPane,
				BoxLayout.Y_AXIS));

		// Create and Add panels to main frame
		mcFirstRow = new JPanel();
		mcSecondRow = new JPanel();
		mcThridRow = new JPanel();

		mcThridRow.setBorder(BorderFactory.createTitledBorder(BorderFactory
				.createLineBorder(Color.blue), "Data Processed",
				TitledBorder.CENTER, TitledBorder.TOP));

		mcMainContentPane.add(mcFirstRow);
		mcMainContentPane.add(mcSecondRow);
		mcMainContentPane.add(mcThridRow);
		// set the layout manager's for the panels to be
		mcFirstRow.setLayout(new BoxLayout(mcFirstRow, BoxLayout.X_AXIS));
		mcSecondRow.setLayout(new BoxLayout(mcSecondRow, BoxLayout.X_AXIS));
		mcThridRow.setLayout(new BoxLayout(mcThridRow, BoxLayout.Y_AXIS));
		firstRowItems();
		secondRowItems();
		thirdRowItems();

		mcDataReader = new FRC_HostDataReader(this);

		// showMessageDialog();

		// pack() needs to be the last element in this constructor to size GUI
		// correctly
		pack();

	}

	private void firstRowItems() {

		mcFileSaveLabel = new JLabel("File Name:");
		mcFileName = new JTextField();
		mcFileName.setMaximumSize(new Dimension(150, 30));

		mcLoadFile = new JButton("Load File");
		mcLoadFile.addActionListener(new HandleLoadFile(this));

		// add items to the first row
		mcFirstRow.add(mcFileSaveLabel);
		mcFirstRow.add(mcFileName);
		mcFirstRow.add(mcLoadFile);

	}

	private void secondRowItems() {
		mcPlayData = new JButton("Play Data");
		mcPausePlay = new JButton("Pause");
		mcStopPlay = new JButton("Stop");
		mcResetToBeginning = new JButton("Reset");

		mcPlayData.setEnabled(false);
		mcPausePlay.setEnabled(false);
		mcStopPlay.setEnabled(false);
		mcResetToBeginning.setEnabled(false);

		mcPlayData.addActionListener(new HandlePlayButton());
		mcPausePlay.addActionListener(new HandlePauseButton());
		mcStopPlay.addActionListener(new HandleStopButton());
		mcResetToBeginning.addActionListener(new HandleResetButton());

		mcSecondRow.add(mcPlayData);
		mcSecondRow.add(mcPausePlay);
		mcSecondRow.add(mcStopPlay);
		mcSecondRow.add(mcResetToBeginning);
	}

	private void thirdRowItems() {
		mcFilePlayProgress = new JProgressBar();
		mcFilePlayProgress.setMaximum(100);
		mcFilePlayProgress.setValue(0);

		mcFilePlayProgress.setStringPainted(true);

		mcThridRow.add(mcFilePlayProgress);
	}

	public void FileLoadedSuccessAction() {
		mcPlayData.setEnabled(true);
		mcPausePlay.setEnabled(false);
		mcStopPlay.setEnabled(false);
		mcResetToBeginning.setEnabled(false);
	}

	private void buttonPlayAction() {
		mcPlayData.setEnabled(false);
		mcPausePlay.setEnabled(true);
		mcStopPlay.setEnabled(true);
		mcResetToBeginning.setEnabled(false);
	}

	private void buttonPauseAction() {
		mcPlayData.setEnabled(true);
		mcPausePlay.setEnabled(false);
		mcStopPlay.setEnabled(false);
		mcResetToBeginning.setEnabled(false);
	}

	private void buttonStopAction() {
		mcPlayData.setEnabled(true);
		mcPausePlay.setEnabled(false);
		mcStopPlay.setEnabled(false);
		mcResetToBeginning.setEnabled(true);
	}

	private void buttonResetAction() {
		mcPlayData.setEnabled(true);
		mcPausePlay.setEnabled(false);
		mcStopPlay.setEnabled(false);
		mcResetToBeginning.setEnabled(false);
	}

	public void setProgress(long anDone, long anTotal) {
		float lnDone = anDone;
		float lnTotal = anTotal;
		float lnPercentage;

		if (anTotal != 0) {
			lnPercentage = (lnDone / lnTotal) * 100;
		} else {
			lnPercentage = 0;
		}
		int lnDisplayNumber = (int) lnPercentage;

		mcFilePlayProgress.setValue(lnDisplayNumber);
		// mcFilePlayProgress.setString(INTERGER.);

	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// 
		DataPlayerPlatform lcTest = new DataPlayerPlatform();
		lcTest.setVisible(true);

	}

	// inner classes to handle listners
	protected class HandleLoadFile implements ActionListener {
		private JFrame mcFrame;

		public HandleLoadFile(JFrame aOwningFrame) {
			mcFrame = aOwningFrame;
		}

		public void actionPerformed(ActionEvent e) {

			// check to determine if the file name field is not empty
			String lcFileName = mcFileName.getText();
			if (lcFileName.length() == 0) {
				JOptionPane.showMessageDialog(mcFrame, "File name is empty");
			} else {
				// TODO
				if (mcDataReader.LoadFile(lcFileName)) {
					FileLoadedSuccessAction();
				} else {
					JOptionPane.showMessageDialog(mcFrame,
							"Unable to find/load file");
				}
			}
		}

	}

	protected class HandlePlayButton implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			if (mcDataReader.play()) {
				buttonPlayAction();
			}
		}
	}

	protected class HandlePauseButton implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			if (mcDataReader.pause()) {
				buttonPauseAction();
			}
		}
	}

	protected class HandleStopButton implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			if (mcDataReader.stop()) {
				buttonStopAction();
			}
		}
	}

	protected class HandleResetButton implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			if (mcDataReader.reset()) {
				buttonResetAction();
				mcFilePlayProgress.setValue(0);
			}
		}
	}

}
