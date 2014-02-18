package ilite.display.modules.displaylog;

import ilite.display.modules.displaylog.DisplayLogModuleModel.LoggingOptions;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ELevel;

import java.text.SimpleDateFormat;

public class DisplayLogModuleModel implements IUpdate<LoggingOptions>{

	// This class will eventually hold any filtering data
	
	/*package*/ final static SimpleDateFormat mDateFormat = new SimpleDateFormat("kk:mm:ss:SSS");
	private LoggingOptions mOptions = new LoggingOptions();
	
	@Override public void update(LoggingOptions pOptions)
	{
		mOptions = pOptions;
	}
	
	boolean isTimestampEnabled()
	{
		return mOptions.enableTime;
	}
	
	boolean isThreadNameEnabled()
	{
		return mOptions.enableThreads;
	}
	
	boolean isMethodNameEnabled()
	{
		return mOptions.enableMethods;
	}
	
	boolean isLevelEnabled()
	{
		return mOptions.enableLevel;
	}

	ELevel getLowestDebugLevel()
	{
		return mOptions.level;
	}
	
	boolean isClearSet()
	{
	  return mOptions.clearLog;
	}
	
	static class LoggingOptions
	{
		boolean enableThreads = true;
		boolean enableMethods = true;
		boolean enableTime = true;
		boolean enableLevel = true;
		ELevel level = ELevel.DEBUG;
		boolean clearLog = false;
	}
}
