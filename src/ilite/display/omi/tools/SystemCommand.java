package ilite.display.omi.tools;

import ilite.display.omi.HomeScreen;

import java.awt.event.ActionEvent;

public class SystemCommand extends AbstractCommand
{
  private final HomeScreen mHome;
  
  public SystemCommand(HomeScreen pHome)
  {
    mHome = pHome;
  }

  @Override
  public void actionPerformed(ActionEvent e)
  {
    mHome.setDisplayedModule(HomeScreen.sCONF);
  }

  @Override
  public String getName()
  {
    return "CONFIG";
  }

}
