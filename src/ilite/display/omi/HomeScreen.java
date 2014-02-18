package ilite.display.omi;

import ilite.display.Constants;
import ilite.display.IMasterController;
import ilite.display.config.ConfigurationPanel;
import ilite.display.config.ISystemConfiguration;
import ilite.display.config.SystemConfiguration;
import ilite.display.modules.Module;
import ilite.util.lang.Delegator;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

import java.awt.CardLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JPanel;

public class HomeScreen
{
	private final List<Module> mModules;
	public static final String sHOME = "HOME_SCREEN";
	public static final String sCONF = "CONFIG_SCREEN";
	private List<String> mLoadedPanels = new ArrayList<String>();
	private JPanel mPanel;
	private CardLayout mCards;
	private static final int sSIZE = 160;
	private Delegator<String> mModuleChangeListeners = new Delegator<String>();
	private ILog mLog = Logger.createLog(HomeScreen.class);

	public HomeScreen(List<Module> pModules, IMasterController pMaster)
	{
		mModules = pModules;
		addModuleChangeListener(new ModuleChangeListener());
		initDisplay(pMaster);
	}
	
	public final void addModuleChangeListener(IUpdate<String> pListener)
	{
		mModuleChangeListeners.addListener(pListener);
	}

	private final void initDisplay(IMasterController pMaster)
	{
		ISystemConfiguration config = pMaster.getSystemConfiguration();
		SystemConfiguration sysconfig = (SystemConfiguration)config;
		mCards = new CardLayout();
		mPanel = new JPanel(mCards);
		JPanel home = generateHomeScreen();
		JComponent configpanel = new ConfigurationPanel(sysconfig).getPanel();
		mPanel.add(home, sHOME);
		mPanel.add(configpanel, sCONF);
		mLoadedPanels.add(sHOME);
		mLoadedPanels.add(sCONF);
		for(Module m : mModules)
		{
			Container c = null;
			String key = null;
			c = m.getControl().getDisplayComponent();
			key = m.getControl().getName();
			if(key == null)
			{
			  key = m.getControl().getClass().getSimpleName();
			}
			mPanel.add(c, key);
			mLoadedPanels.add(key);
		}
	}

	public JPanel getPanel()
	{
		return mPanel;
	}

	public List<String> getLoadedPanels()
	{
		return mLoadedPanels;
	}

	public void setDisplayedModule(String pModule)
	{
		if(!mLoadedPanels.contains(pModule))
		{
			System.err.println("Module " + pModule + " does not exist!");
			return;
		}

		mCards.show(mPanel, pModule);
		
		if(pModule.equalsIgnoreCase(sCONF) || pModule.equalsIgnoreCase(sHOME))
		{
			mModuleChangeListeners.update(null);
		}
	}

	private JPanel generateHomeScreen()
	{
		JPanel p = new JPanel(new GridBagLayout());
		GridBagConstraints gbc = new GridBagConstraints();
		gbc.gridx = 0; gbc.gridy = 0;
		
		int numTilesPerRow = determineNumTilesPerRow(mModules);		
		int size = determineWidestTextWidth(mModules);
		Dimension tile_size = new Dimension((int)(size*1.6), size);
		int pad = (int)(size/1.6);
		Dimension vert_size = new Dimension(pad, size);
		mLog.debug("Tile Size = ",tile_size);
		
		for(Module m : mModules)
		{
		  JButton tile = generateTile(m);
		  tile.setPreferredSize(tile_size);
			p.add(tile, gbc); gbc.gridx++;
			if(gbc.gridx > numTilesPerRow)
			{
			  gbc.gridx = 0;
			  gbc.gridy++;
			  if(gbc.gridy < mModules.size()/numTilesPerRow)
			  {
	        JPanel h_space = new JPanel();
	        h_space.setPreferredSize(new Dimension(pad, pad));
	        p.add(h_space, gbc);
	        gbc.gridy++;
			  }
			}
			else
			{
	      JPanel spacer = new JPanel();
	      spacer.setPreferredSize(vert_size);
	      p.add(spacer, gbc);
	      gbc.gridx++;
			}

		}


		return p;
	}
	
	private int determineNumTilesPerRow(List<Module> pModules)
	{
	  int width = determineWidestTextWidth(pModules);
	  Dimension screensize = Constants.getScreenSize();
	  int x = (int)((1.6/(2.6*width))*screensize.width - (1/1.6));
	  return x;
	}
	
	private int determineWidestTextWidth(List<Module> pModules)
	{
	  int result = 100;
	  JButton b = new JButton(pModules.get(0).getName());
	  FontMetrics fm = b.getFontMetrics(b.getFont());
	  for(Module m : pModules)
	  {
      int s = fm.stringWidth(m.getName());
      result = Math.max(result, s);
	  }
	  return result;
	}
	
	private JButton generateTile(Module pModule)
	{
    String title = pModule.getControl().getClass().getSimpleName();
    JButton tile = new JButton(title);
    tile.addActionListener(new ActionListener() {
      @Override
      public void actionPerformed(ActionEvent ae) {
        JButton b = (JButton)ae.getSource();
        mModuleChangeListeners.update(b.getText());
      }
    });
    return tile;
	}
	
	private class ModuleChangeListener implements IUpdate<String>
	{
		public void update(String pString)
		{
			if(pString != null)
			{
				setDisplayedModule(pString);
			}
		}
	}

}
