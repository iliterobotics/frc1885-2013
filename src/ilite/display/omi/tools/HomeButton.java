package ilite.display.omi.tools;

import ilite.display.omi.HomeScreen;

import java.awt.event.ActionEvent;

public class HomeButton extends AbstractCommand
{
  private final HomeScreen mHome;
  
  public String getName()
  {
    return "HOME";
  }
  
  public HomeButton(HomeScreen pHome)
  {
    mHome = pHome;
  }

  @Override
  public void actionPerformed(ActionEvent e)
  {
    mHome.setDisplayedModule(HomeScreen.sHOME);
  }

}
