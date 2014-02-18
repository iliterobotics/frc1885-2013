package ilite.display.modules.display2009;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.BorderFactory;
import javax.swing.JPanel;

public class DriverVisuals extends JPanel
{  
  KeyPanelTest keyPanel;
  DataAndControlPanel dataPanel;
  RadarDisplayPanelTest radarPanel;

  protected GridBagLayout mcGblDriverVisualsPanel;
  protected GridBagConstraints mcGbc;
  
  public DriverVisuals()
  {
    
    //make GBL work
    mcGblDriverVisualsPanel = new GridBagLayout();
    mcGbc = new GridBagConstraints();
    
    mcGbc.weightx = 100;
    mcGbc.weighty = 100;
    mcGbc.insets.bottom = 0;
    mcGbc.insets.left = 0;
    mcGbc.insets.right = 0;
    mcGbc.insets.top = 0;
    mcGbc.fill = GridBagConstraints.NONE;
    mcGbc.anchor = GridBagConstraints.NORTHWEST;
    
    setLayout(mcGblDriverVisualsPanel);
    
    keyPanel = new KeyPanelTest();
    dataPanel = new DataAndControlPanel();
    dataPanel.setBorder(BorderFactory.createLoweredBevelBorder());
    radarPanel = new RadarDisplayPanelTest();

    
    mcGbc.anchor = GridBagConstraints.CENTER;
    add(keyPanel,1,2,1,1);
    add(dataPanel,1,1,1,2);
    add(radarPanel,2,2,1,1);
      
  }
  
  protected void add (Component c, int y, int x, int w, int h)
  {
    //setup constraints
    mcGbc.gridx = x - 1;
    mcGbc.gridy = y - 1;
    mcGbc.gridwidth = w;
    mcGbc.gridheight = h;
    // apply constraints to layout
    mcGblDriverVisualsPanel.setConstraints(c, mcGbc);
    // add new element to Panel
    super.add(c);
  } 
  
  public void setData(DriverVisualsStruct acData)
  {
    dataPanel.setData(acData);
    radarPanel.setData(acData);
    keyPanel.setData(acData);
  }
}