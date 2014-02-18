package ilite.display.modules;

import ilite.display.IMasterController;

import java.awt.Container;

/**
 * @author JesseK
 *  Requires a default, no parameter constructor
 */
public interface IModuleControl 
{
	public void initControl(IMasterController pMaster);
	
	public void postApplicationInit();
	
	public Container getDisplayComponent();
	
	public void initiateShutdownSequence();
	
	public String getName();
}
