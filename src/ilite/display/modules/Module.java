package ilite.display.modules;

import ilite.display.IMasterController;

public class Module 
{
	private final String mModuleControlClass;
	
	private IModuleControl mModuleControl;
	
	public Module(String pModuleControlClass)
	{
		if(pModuleControlClass == null) throw new IllegalArgumentException("Control class is null");
		mModuleControlClass = pModuleControlClass;
	}
	
	public void generate(IMasterController pMaster) 
	throws InstantiationException, IllegalAccessException, ClassNotFoundException
	{
    mModuleControl = instantiate(mModuleControlClass, IModuleControl.class);
		mModuleControl.initControl(pMaster);
	}
	
	public IModuleControl getControl()
	{
		return mModuleControl;
	}	
	
	public String getName()
	{
	  return mModuleControl.getClass().getSimpleName();
	}
	
	private static <T> T instantiate(final String className, final Class<T> type) 
	throws InstantiationException, IllegalAccessException, ClassNotFoundException
	{
        return type.cast(Class.forName(className).newInstance());
	}

}
