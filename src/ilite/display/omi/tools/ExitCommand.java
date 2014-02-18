package ilite.display.omi.tools;

import ilite.display.IApplicationController;

import java.awt.event.ActionEvent;

public class ExitCommand extends AbstractCommand
{
  private final IApplicationController mAppCtrl;
  public ExitCommand(IApplicationController pController)
  {
    mAppCtrl = pController;
  }

  @Override
  public void actionPerformed(ActionEvent e)
  {
    mAppCtrl.exit();
  }

  @Override
  public String getName()
  {
    return "X";
  }

}
