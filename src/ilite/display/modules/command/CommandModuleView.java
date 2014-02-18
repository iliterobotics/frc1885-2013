package ilite.display.modules.command;

import ilite.display.Constants;
import ilite.display.IMasterController;
import ilite.display.StateData;
import ilite.display.modules.display2013.data.RobotCommand;
import ilite.util.lang.Delegator;
import ilite.util.lang.INotify;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Component;
import java.awt.Container;
import java.awt.FocusTraversalPolicy;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.SpringLayout;

public class CommandModuleView implements KeyListener, IProvider<RobotCommand>, INotify
{
	private final StateData mStateData;
	private ILog mLog = Logger.createLog(CommandModuleView.class);
	private final JPanel mPanel = new JPanel();
	private final static int sPAD_NS = 5, sPAD_EW = 17, sPAD_EW2 = 25;
	public Delegator<RobotCommand> delegator;
	private JList<RobotCommand> sendHistoryList = new JList<RobotCommand>();
	private final JTextField inputMSG = new JTextField(6);
	private final JTextField inputSubsys = new JTextField(6);
	private final JTextField inputValue = new JTextField(6);

	public CommandModuleView(IMasterController pMaster) {

	  mStateData = pMaster.getStateData();
		//creates all the items on the display
		sendHistoryList.setFont(sendHistoryList.getFont().deriveFont(Constants.getSmallFontSize()-8.5f));
		SpringLayout sl = new SpringLayout();
		mPanel.setLayout(sl);
		JLabel MSG_STRUCT = new JLabel("Message Structure: ");
		MSG_STRUCT.setAlignmentX(JLabel.CENTER_ALIGNMENT);
		mPanel.add(MSG_STRUCT);
		JLabel MSG_ID = new JLabel("Message ID: ");
		mPanel.add(MSG_ID);
		mPanel.add(inputMSG);
		inputMSG.addKeyListener(this);
		TextFieldTransferPolicy tftp = new TextFieldTransferPolicy();
		inputMSG.setFocusTraversalPolicy(tftp);
		JLabel SubSys_ID = new JLabel("Subsystem ID: ");
		mPanel.add(SubSys_ID);
		mPanel.add(inputSubsys);
		inputSubsys.addKeyListener(this);
		inputSubsys.setFocusTraversalPolicy(tftp);
		JLabel Value_In = new JLabel("Value: ");
		mPanel.add(Value_In);
		mPanel.add(inputValue);
		inputValue.addKeyListener(this);
		inputValue.setFocusTraversalPolicy(tftp);
		JButton sendThis = new JButton("Send to Robot"); 
		mPanel.add(sendThis);
		JLabel Hist_Out = new JLabel("History: ");
		mPanel.add(Hist_Out);
		mPanel.add(sendHistoryList);
		JButton setThis = new JButton("SET");
		mPanel.add(setThis);
		JButton delThis = new JButton("DEL");
		mPanel.add(delThis);
		JLabel MSG_List = new JLabel("Message ID List");
		mPanel.add(MSG_List);
		JTable ListMSG = new JTable();
		mPanel.add(ListMSG);
		JLabel Sys_List = new JLabel("System ID List");
		mPanel.add(Sys_List);
		JTable ListSYS = new JTable();
		mPanel.add(ListSYS);
		JButton populateThis = new JButton("Populate With Current Values");
		mPanel.add(populateThis);
		JScrollPane listScroll = new JScrollPane(sendHistoryList);
		mPanel.add(listScroll);
		inputMSG.setFocusTraversalKeysEnabled(false);
		inputSubsys.setFocusTraversalKeysEnabled(false);
		//the placing code for each item on the display
		sl.putConstraint(SpringLayout.WEST, MSG_STRUCT, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.NORTH, MSG_STRUCT, sPAD_NS, SpringLayout.NORTH, mPanel);
		sl.putConstraint(SpringLayout.EAST, MSG_STRUCT, 0, SpringLayout.EAST, inputMSG);
		sl.putConstraint(SpringLayout.WEST, MSG_ID, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.VERTICAL_CENTER, MSG_ID, 0, SpringLayout.VERTICAL_CENTER, inputMSG);
		sl.putConstraint(SpringLayout.WEST, SubSys_ID, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.VERTICAL_CENTER, SubSys_ID, 0, SpringLayout.VERTICAL_CENTER, inputSubsys);
		sl.putConstraint(SpringLayout.WEST, Value_In, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.VERTICAL_CENTER, Value_In, 0, SpringLayout.VERTICAL_CENTER, inputValue);
		sl.putConstraint(SpringLayout.WEST, inputMSG, sPAD_EW, SpringLayout.EAST, SubSys_ID);
		sl.putConstraint(SpringLayout.NORTH, inputMSG, sPAD_NS, SpringLayout.SOUTH, MSG_STRUCT);
		sl.putConstraint(SpringLayout.WEST, inputSubsys, sPAD_EW, SpringLayout.EAST, SubSys_ID);
		sl.putConstraint(SpringLayout.NORTH, inputSubsys, sPAD_NS, SpringLayout.SOUTH, inputMSG);
		sl.putConstraint(SpringLayout.WEST, inputValue, sPAD_EW, SpringLayout.EAST, SubSys_ID);
		sl.putConstraint(SpringLayout.NORTH, inputValue, sPAD_NS, SpringLayout.SOUTH, inputSubsys);
		sl.putConstraint(SpringLayout.WEST, sendThis, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.WEST, sendThis, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.EAST, sendThis, 0, SpringLayout.EAST, inputSubsys);
		sl.putConstraint(SpringLayout.NORTH, sendThis, sPAD_NS, SpringLayout.SOUTH, inputValue);
		sl.putConstraint(SpringLayout.WEST, setThis, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.SOUTH, setThis, -sPAD_NS, SpringLayout.SOUTH, mPanel);
		sl.putConstraint(SpringLayout.WEST, listScroll, sPAD_EW, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.SOUTH, listScroll, -sPAD_NS, SpringLayout.NORTH, setThis);
		sl.putConstraint(SpringLayout.EAST, listScroll, 0, SpringLayout.EAST, inputSubsys);
		sl.putConstraint(SpringLayout.HORIZONTAL_CENTER, Hist_Out, 0, SpringLayout.HORIZONTAL_CENTER, listScroll);
		sl.putConstraint(SpringLayout.NORTH, Hist_Out, sPAD_NS, SpringLayout.SOUTH, sendThis);
		sl.putConstraint(SpringLayout.NORTH, listScroll, sPAD_NS, SpringLayout.SOUTH, Hist_Out);
		sl.putConstraint(SpringLayout.SOUTH, delThis, -sPAD_NS, SpringLayout.SOUTH, mPanel);
		sl.putConstraint(SpringLayout.EAST, delThis, 0, SpringLayout.EAST, inputSubsys);
		sl.putConstraint(SpringLayout.EAST, populateThis, -sPAD_EW, SpringLayout.EAST, mPanel);
		sl.putConstraint(SpringLayout.WEST, populateThis, sPAD_EW2, SpringLayout.EAST, delThis);
		sl.putConstraint(SpringLayout.SOUTH, populateThis, -sPAD_NS, SpringLayout.SOUTH, mPanel);
		sl.putConstraint(SpringLayout.EAST, ListSYS, -sPAD_EW, SpringLayout.EAST, mPanel);
		sl.putConstraint(SpringLayout.WEST, ListSYS, sPAD_EW, SpringLayout.HORIZONTAL_CENTER, populateThis);
		sl.putConstraint(SpringLayout.SOUTH, ListSYS, -sPAD_NS, SpringLayout.NORTH, populateThis);
		sl.putConstraint(SpringLayout.NORTH, Sys_List, sPAD_NS, SpringLayout.NORTH, mPanel);
		sl.putConstraint(SpringLayout.HORIZONTAL_CENTER, Sys_List, 0, SpringLayout.HORIZONTAL_CENTER, ListSYS);
		sl.putConstraint(SpringLayout.NORTH, ListSYS, sPAD_NS, SpringLayout.SOUTH, Sys_List);
		sl.putConstraint(SpringLayout.SOUTH, ListMSG, -sPAD_NS, SpringLayout.NORTH, populateThis);
		sl.putConstraint(SpringLayout.EAST, ListMSG, -sPAD_EW, SpringLayout.WEST, ListSYS);
		sl.putConstraint(SpringLayout.WEST, ListMSG, 0, SpringLayout.WEST, populateThis);
		sl.putConstraint(SpringLayout.NORTH, MSG_List, sPAD_NS, SpringLayout.NORTH, mPanel);
		sl.putConstraint(SpringLayout.NORTH, ListMSG, sPAD_NS, SpringLayout.SOUTH, MSG_List);
		sl.putConstraint(SpringLayout.HORIZONTAL_CENTER, MSG_List, 0, SpringLayout.HORIZONTAL_CENTER, ListMSG);
		//jtabbedpane; tab change listener
		sendThis.addActionListener(new ActionListener()
		{
			//enabling the send to robot button
			@Override
			public void actionPerformed(ActionEvent e)
			{
				int msgid =Integer.parseInt(inputMSG.getText());
				int subsysid=Integer.parseInt(inputSubsys.getText());
				double valuein = Double.parseDouble(inputValue.getText());
				RobotCommand cmd = new RobotCommand(msgid, subsysid, valuein);
				mLog.debug(cmd);
				delegator.update(cmd);
			}
		});
		//enabling the set button
		setThis.addActionListener(new ActionListener()
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				int index = sendHistoryList.getSelectedIndex();
				RobotCommand pUpdate = sendHistoryList.getModel().getElementAt(index);
				delegator.update(pUpdate);

			}
		});
		delThis.addActionListener(new ActionListener()
		{
			@Override
			public void actionPerformed(ActionEvent e)
			{
				int index = sendHistoryList.getSelectedIndex();
				CommandModuleModel model = (CommandModuleModel)sendHistoryList.getModel();
				model.removeElementAt(index);
			}
		});


	}
	public JList<RobotCommand> getList()
	{
		return sendHistoryList;
	}
	public void setDelegator(Delegator<RobotCommand> rcListeners)
	{
		delegator = rcListeners;
	}
	
	public Container getDisplayPanel() {
		return mPanel;
	}

	public String getDisplayedTitle() {
		return "Commands";
	}

	@Override
	public void keyPressed(KeyEvent e)
	{
		if(e.getKeyCode() == KeyEvent.VK_TAB)
		{

			this.inputMSG.transferFocus();
			//			this.inputSubsys.selectAll();
		}

	}
	@Override
	public void keyReleased(KeyEvent e)
	{
		// TODO Auto-generated method stub

	}
	@Override
	public void keyTyped(KeyEvent e)
	{


	}

	private class TextFieldTransferPolicy extends FocusTraversalPolicy
	{

		@Override
		public Component getLastComponent(Container arg0)
		{
			return inputValue;
		}

		@Override
		public Component getFirstComponent(Container arg0)
		{
			// TODO Auto-generated method stub
			return inputMSG;
		}

		@Override
		public Component getDefaultComponent(Container arg0)
		{
			return inputMSG;
		}

		@Override
		public Component getComponentBefore(Container arg0, Component c)
		{
			if(c == inputMSG) return getLastComponent(arg0);
			if(c == inputSubsys) return inputMSG;
			return inputSubsys;
		}

		@Override
		public Component getComponentAfter(Container arg0, Component c)
		{
			if(c == inputMSG) return inputSubsys;
			if(c == inputSubsys) return inputValue;
			return getFirstComponent(arg0);
		}
	}

	@Override
	public void addListener(IUpdate<RobotCommand> pListener) {
		delegator.addListener(pListener);
	}
	@Override
	public void removeListener(IUpdate<RobotCommand> pListener) {
		delegator.removeListener(pListener);
	}
	@Override
	public RobotCommand getLatest() {
		return delegator.getLatest();
	}
  @Override
  public void notifyOfUpdate()
  {
    // TODO Auto-generated method stub
    
  }
}
