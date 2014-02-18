package ilite.display.modules.command;

import ilite.display.IMasterController;
import ilite.display.modules.IModuleControl;
import ilite.display.modules.display2013.MessagePusher;
import ilite.display.modules.display2013.data.RobotCommand;
import ilite.util.lang.Delegator;
import ilite.util.lang.Notifyer;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.Container;

public class CommandModuleControl implements IModuleControl{
	Delegator<RobotCommand> rcListeners = new Delegator<RobotCommand>();
	private ILog mLog = Logger.createLog(CommandModuleControl.class);
	private CommandModuleView mView;
	private CommandModuleModel mModel;
	private Notifyer mUpdateListeners = new Notifyer(true);
	
	@Override
	public String getName()
	{
	  return "COMMANDS TO ROBOT";
	}
	
	@Override
	public void initControl(IMasterController pMaster) {
	  mView = new CommandModuleView(pMaster);
		mView.setDelegator(rcListeners);
		mView.getList().setModel(mModel);
		rcListeners.addListener(mModel);
		pMaster.getDataProviders().registerRobotCommandProvider(mView);
		MessagePusher.inst = new MessagePusher(pMaster.getDataProviders().getProvider(RobotCommand.class), pMaster.getStateData());
	}

	@Override
	public void postApplicationInit() {
    mUpdateListeners.addNotificationListener(mView);
	}

	@Override
	public void initiateShutdownSequence() {
		// TODO Auto-generated method stub
		
	}

  @Override
  public Container getDisplayComponent()
  {
    return mView.getDisplayPanel();
  }

}
