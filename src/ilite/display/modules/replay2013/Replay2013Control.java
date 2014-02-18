package ilite.display.modules.replay2013;

import ilite.display.IMasterController;
import ilite.display.modules.IModuleControl;

import java.awt.Container;

import javax.swing.JPanel;

public class Replay2013Control implements IModuleControl
{
  
  @Override
  public String getName()
  {
    return "REPLAY 2013";
  }

  @Override
  public void initControl(IMasterController pMaster)
  {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void postApplicationInit()
  {
    
  }

  @Override
  public void initiateShutdownSequence()
  {
    
  }

  @Override
  public Container getDisplayComponent()
  {
    return new JPanel();
  }

}
