package ilite.display.modules.display2014;

import ilite.display.config.UIOptions;
import ilite.display.modules.displaylog.DisplayLogModuleControl;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.UnsupportedLookAndFeelException;

public class IntegrationDisplay2014
{
  private final JPanel mMainPanel;
  private final JTabbedPane mTabs;
  private TelemetryTablePanel mTelemetryPanel;
  
  public IntegrationDisplay2014()
  {
    mTelemetryPanel = new TelemetryTablePanel();
    mMainPanel = new JPanel(new BorderLayout());
    mTabs = new JTabbedPane();
    mTabs.add("Telemetry Table", mTelemetryPanel.getDisplayPanel());
    mTabs.add("Connection Config", new ConnectionPanel());
    
    RobotDebugPrinter debug = new RobotDebugPrinter();
    DataManager2014.getInstance().getDebugMsgProvider().addListener(debug);
    mTabs.add("Robot Debug Messages", debug);

    DisplayLogModuleControl log = new DisplayLogModuleControl();
    mTabs.add("Java Console Log", getLogPanel(log));
    log.postApplicationInit();
    
    String rootData = DataManager2014.getInstance().getRootDataFolder();
    mTabs.add("Match Config", new MatchConfigPanel(rootData).getDisplayPanel());
    
    mMainPanel.add(mTabs, BorderLayout.CENTER);
  }
  
  public JPanel getDisplayPanel()
  {
    return mMainPanel;
  }
  
  private JPanel getLogPanel(DisplayLogModuleControl pLogger)
  {
    JPanel log = new JPanel(new BorderLayout());
    log.add(pLogger.getDisplayComponent(), BorderLayout.CENTER);
    JPanel options = pLogger.getOptionsPanel();
    options.setBorder(BorderFactory.createTitledBorder("Log Options"));
    options.setPreferredSize(new Dimension(200, 1024));
    log.add(options, BorderLayout.WEST);
    return log;
  }

  public static void main(String[] pArgs)
  {
    try
    {
      UIOptions.inst().setDefaultLookAndFeel();
      UIOptions.inst().setFontSize(18);
    } 
    catch (ClassNotFoundException | InstantiationException | 
        IllegalAccessException | UnsupportedLookAndFeelException e1)
    {
      e1.printStackTrace();
    }
    
    IntegrationDisplay2014 InT = new IntegrationDisplay2014();
    final JFrame f = new JFrame("Integration 2014");
    f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    f.setContentPane(InT.getDisplayPanel());
    f.pack();
    f.setSize(new Dimension(1200, 768));
    f.setVisible(true);
  }
}
