package ilite.display.modules.display2009;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;

import javax.swing.JPanel;

public class DataAndControlPanel extends JPanel
{
  protected GridBagLayout mcGblDriverVisualsPanel;
  protected GridBagConstraints mcGbc;
  
  
  DataDisplayPanelTest dataPanel;
  ControlPanel controlPanel;
  
  public DataAndControlPanel()
  {
    dataPanel = new DataDisplayPanelTest();
    controlPanel = new ControlPanel();
    
    Dimension size = new Dimension(173,713);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
    
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
    mcGbc.anchor = GridBagConstraints.CENTER;
    
    setLayout(mcGblDriverVisualsPanel);
    
    add(dataPanel,1,1,1,1);
    add(controlPanel,2,1,1,1);
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
  }
}