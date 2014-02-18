package ilite.display.omi;

import ilite.util.gui.components.OutputTextArea;
import ilite.util.lang.IUpdate;
import ilite.util.logging.LogOutput;
import ilite.util.logging.Logger;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.JFrame;

public class PreDisplayLogFrame implements IUpdate<LogOutput>
{
  private JFrame mLogWindow = new JFrame();
  private final OutputTextArea mOutputArea = new OutputTextArea();
  
  public PreDisplayLogFrame()
  {
    Logger.registerOutputListener(this);
    mLogWindow.addWindowListener(new WindowAdapter()
    {

      @Override
      public void windowClosing(WindowEvent arg0)
      {
        Logger.deregisterOutputListener(PreDisplayLogFrame.this);
      }

      @Override
      public void windowClosed(WindowEvent arg0)
      {
        Logger.deregisterOutputListener(PreDisplayLogFrame.this);
      }

    });

    mLogWindow.setContentPane(mOutputArea.getTextArea());
    mLogWindow.setExtendedState(JFrame.MAXIMIZED_BOTH);
    mLogWindow.pack();
    mLogWindow.setVisible(true);
  }

  @Override
  public void update(LogOutput pOutput)
  {
    mOutputArea.append(pOutput.text);
  }
  
  public void setVisible(boolean pVisible)
  {
    mLogWindow.setVisible(pVisible);
    if(pVisible == false)
    {
      mLogWindow.dispose();
      Logger.deregisterOutputListener(this);
    }
  }

  public void setDefaultCloseOperation(int exitOnClose)
  {
    mLogWindow.setDefaultCloseOperation(exitOnClose);
  }

}
