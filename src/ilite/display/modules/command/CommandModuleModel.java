package ilite.display.modules.command;

import ilite.display.modules.display2013.data.RobotCommand;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.util.ArrayList;

import javax.swing.DefaultListModel;
import javax.swing.event.ListDataListener;

public class CommandModuleModel extends DefaultListModel<RobotCommand>
implements IUpdate<RobotCommand>{
	ArrayList<ListDataListener> listeners = new ArrayList<ListDataListener>();
	ArrayList<RobotCommand> history = new ArrayList<RobotCommand>();
	private ILog mLog = Logger.createLog(CommandModuleModel.class);
	
	@Override
	public void update(RobotCommand pObject)
	{
		mLog.debug("Received Update ",pObject);
		this.addElement(pObject);
		
	}
	
	public void remove(RobotCommand pCommand)
	{
		
	}

}
