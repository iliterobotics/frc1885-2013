package ilite.display;

import ilite.display.config.ConfigItem;
import ilite.display.config.ConfigurationXmlParser;
import ilite.display.config.ISystemConfiguration;
import ilite.display.config.SystemConfiguration;
import ilite.display.config.UIOptions;
import ilite.display.modules.Module;
import ilite.display.modules.ModuleXmlParser;
import ilite.display.omi.PreDisplayLogFrame;
import ilite.display.omi.UtilityStatusBar;
import ilite.util.gui.GuiFactory;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.UnsupportedLookAndFeelException;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import Vision.VisionMain;

public class Application implements IMasterController, IApplicationController
{
  private static JFrame sDisplay;
  private static ILog sLog = Logger.createLog(Application.class);
  private List<Module> mModules;
  private ISystemConfiguration mConfiguration;
  private StateData mStateData;
  private final ILog mLog = Logger.createLog(Application.class);
  private final VisionMain mVision = new VisionMain();

  private Application()
  {
  }

  @Override
  public ISystemConfiguration getSystemConfiguration()
  {
    if(mConfiguration == null)
    {
      initSysConfig();
    }
    return mConfiguration;
  }

  public StateData getStateData()
  {
    return mStateData;
  }

  @Override
  public void exit()
  {
    if (sDisplay != null)
    {
      sDisplay.dispose();
    }

    for (Module m : mModules)
    {
      m.getControl().initiateShutdownSequence();
    }
    System.exit(0);
  }

  private void initSysConfig()
  {
    ConfigurationXmlParser cxp = new ConfigurationXmlParser(
        Constants.sCONFIG_DIR, Constants.sCONFIG_FILE);
    try
    {
      cxp.parse();
    } catch (Exception e)
    {
      e.printStackTrace();
      System.exit(-1);
    }

    List<ConfigItem> ci = cxp.getConfigurationItems();
    mConfiguration = new SystemConfiguration(ci);
    Constants.setConfig(mConfiguration);
  }

  private boolean start() throws ParserConfigurationException, SAXException,
      IOException
  {
    try
    {
      UIOptions.inst().setDefaultLookAndFeel();
      UIOptions.inst().setFontSize(12);
    } 
    catch (ClassNotFoundException | InstantiationException | 
        IllegalAccessException | UnsupportedLookAndFeelException e1)
    {
      sLog.exception(e1);
    }

    try
    {
      mVision.startMain();
      DataProviders.INST.setCameraImageProvider(mVision.getUpdater());      
    }
    catch (Throwable t)
    {
      mLog.error("Unable to start vision due to an internal Vision Error");
      t.printStackTrace();
    }

    ModuleXmlParser parser = new ModuleXmlParser(null);
    parser.parse();
    mModules = parser.getModules();
    List<Module> toRemove = new ArrayList<>();
    for (Module m : mModules)
    {
      try
      {
        m.generate(this);
      } catch (Exception e)
      {
        sLog.error("Exception thrown when instantiating a module");
        sLog.exception(e);
        toRemove.add(m);
      }
    }
    mModules.removeAll(toRemove);

    getSystemConfiguration();
    mStateData = new StateData();
    mStateData.setParentWindow(sDisplay);

    sDisplay = new JFrame();
    sDisplay.setTitle("ILITE FRC Display");
    mStateData.setParentWindow(sDisplay);
    JPanel p = new JPanel(new BorderLayout());
    
    Container center = mModules.get(0).getControl().getDisplayComponent();    
    UtilityStatusBar sb = new UtilityStatusBar(this);
    p.add(sb.getPanel(), BorderLayout.SOUTH);
    p.add(center, BorderLayout.CENTER);
    sDisplay.add(p);
    sDisplay.pack();
    sDisplay.setVisible(true);
    sDisplay.setExtendedState(JFrame.MAXIMIZED_BOTH);
    Dimension d = sDisplay.getSize();
    sDisplay.setSize(d.width, d.height - 256);

    for (final Module m : mModules)
    {
      try
      {
        m.getControl().postApplicationInit();
      } catch (Exception e)
      {
        sLog.error("Error during post-initialization of module ", m.getName());
        sLog.exception(e);
      }
    }
    sDisplay.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    return true;

  }

  public static void main(String[] pArgs)
  {
    PreDisplayLogFrame logframe = new PreDisplayLogFrame();
    logframe.setVisible(true);
    Application app = new Application();
    try
    {
      if (app.start())
      {
        logframe.setVisible(false);
      }
    } catch (Exception e)
    {
      sLog.exception(e);
      logframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
    GuiFactory.INST.createAutoRepaintTimer(20, sDisplay);
  }

  @Override
  public DataProviders getDataProviders()
  {
    return DataProviders.INST;
  }

  @Override
  public void startConnections()
  {
  }

  @Override
  public void stopConnections()
  {
  }
}
