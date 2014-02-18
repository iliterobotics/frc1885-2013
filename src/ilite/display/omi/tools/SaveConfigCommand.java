package ilite.display.omi.tools;

import java.awt.event.ActionEvent;

public class SaveConfigCommand extends AbstractCommand
{

  @Override
  public void actionPerformed(ActionEvent e)
  {
    System.err.println("SAVE FUNCTION ONLY ENABLED ON CONFIGURATION PANEL");
  }

  @Override
  public String getName()
  {
    return "SAVE";
  }

}
