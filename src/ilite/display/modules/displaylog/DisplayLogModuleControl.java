package ilite.display.modules.displaylog;

import ilite.display.IMasterController;
import ilite.display.modules.IModuleControl;
import ilite.display.modules.displaylog.DisplayLogModuleModel.LoggingOptions;
import ilite.util.lang.IUpdate;
import ilite.util.logging.LogOutput;
import ilite.util.logging.Logger;

import java.awt.Container;
import java.util.List;

import javax.swing.JPanel;

public class DisplayLogModuleControl implements IModuleControl, IUpdate<LogOutput>{
	private DisplayLogModuleView mView = new DisplayLogModuleView();
	private DisplayLogModuleModel mModel = new DisplayLogModuleModel();
	DisplayLogOptionsPanel mOptions = new DisplayLogOptionsPanel();
	private static final String OPEN_BRACKET = "[", CLOSE_BRACKET = "] ";

	@Override
	public void initControl(IMasterController pMaster) {	
	  
	}
  
  @Override
  public String getName()
  {
    return "DISPLAY LOG";
  }
  
  public JPanel getOptionsPanel()
  {
    return mOptions.getInternalPanel();
  }

	@Override
	public void postApplicationInit() {
		mOptions.setOptionsListener(new LogOptionsUpdater());
		List<LogOutput> hist = Logger.getRecentLogs();
		for(LogOutput log : hist)
		{
			update(log);
		}
		Logger.registerOutputListener(this);	
	}

	@Override
	public void initiateShutdownSequence() {
		
	}
	
	@Override
  public void update(LogOutput log)
	{
		if(log.level.ordinal() < mModel.getLowestDebugLevel().ordinal())
		{
			return;
		}
		StringBuilder sb = new StringBuilder();
		if(mModel.isTimestampEnabled())
		{
			sb.append(OPEN_BRACKET);
			sb.append(DisplayLogModuleModel.mDateFormat.format(log.time));
			sb.append(CLOSE_BRACKET);
		}
		if(mModel.isLevelEnabled())
		{
			sb.append(OPEN_BRACKET);
			sb.append(log.level);
			sb.append(CLOSE_BRACKET);
		}
		if(mModel.isThreadNameEnabled())
		{
			sb.append(OPEN_BRACKET);
			sb.append(log.thread);
			sb.append(CLOSE_BRACKET);
		}
		if(mModel.isMethodNameEnabled())
		{
			sb.append(OPEN_BRACKET);
			sb.append(log.method);
			sb.append(CLOSE_BRACKET);
		}
		sb.append(log.text);
		mView.mOutput.append(sb.toString());
	}
	
  @Override
  public Container getDisplayComponent()
  {
    return mView.getDisplayPanel();
  }

  private class LogOptionsUpdater implements IUpdate<DisplayLogModuleModel.LoggingOptions>
  {
    @Override
    public void update(LoggingOptions pOptions)
    {
      mModel.update(pOptions);
      if(mModel.isClearSet())
      {
        mView.mOutput.clear();
      }
    }
    
  }
}
