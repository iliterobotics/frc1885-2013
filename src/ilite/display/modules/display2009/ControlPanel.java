package ilite.display.modules.display2009;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ControlPanel extends JPanel
{
  protected GridBagLayout mcGblDriverVisualsPanel;
  protected GridBagConstraints mcGbc;
  
//  FRC_HostDataSaver dataSaver;
  
  JButton rangeButton;
  JButton saveButton;
  JButton closeButton;
  JButton loadData;
  
  JLabel rangeLabel;
  JTextField range;
  
  JLabel turretRangeLabel;
  JTextField turretRange;
  
  static int snRadarRange = 30;
  static int snTurretRange = 10;
  
  public ControlPanel()
  {
    super();
    
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
    
    /*******************************************
    *                                         *
    *                                         *
    *                                         *
    *                                         *
    *                                         *
    *                                         *             
    *           RANGE OF RADAR!!              *
    *                                         *
    *                                         *
    *                                         *
    *                                         *
    *                                         *
    *                                         *
    ******************************************/
    
    range = new JTextField("30",10);
    rangeLabel = new JLabel("Range: ");
    
    turretRange = new JTextField("10",10);
    turretRangeLabel = new JLabel("Range of turret: ");
    
    rangeButton = new JButton("Set");
    rangeButton.addActionListener(new RangeListener());
    
    saveButton = new JButton("Save");
    saveButton.addActionListener(new DataListener());
    closeButton = new JButton("Close");
    closeButton.addActionListener(new DataListener());
    loadData = new JButton("Load Data");
    loadData.addActionListener(new DataListener());
    
    
    
    add(rangeLabel,1,1,2,1);
    add(range,2,1,2,1);
    add(turretRangeLabel,3,1,2,1);
    add(turretRange,4,1,2,1);
    add(rangeButton,5,1,2,1);
    add(saveButton, 6,1,1,1);
    add(closeButton, 6,2,1,1);
    add(loadData,7,1,2,1);
    
    Dimension size = new Dimension(173,263);
    
    setMaximumSize(size);
    setPreferredSize(size);
    setMinimumSize(size);
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
  
  public class RangeListener implements ActionListener
  {
    public void actionPerformed(ActionEvent e)
    {
      snRadarRange = new Integer(range.getText());
      snTurretRange = new Integer(turretRange.getText());
    }
  }
  
  public class DataListener implements ActionListener
  {
    public void actionPerformed(ActionEvent e)
    {
      if(e.getSource()==saveButton)
      {
//        if(dataSaver==null)
        {
          String input;
          int match;
          String fileName;
          
//          dataSaver = new FRC_HostDataSaver();
          
          input=JOptionPane.showInputDialog("Enter match number or file name");
          try
          {
            match = new Integer(input);
            fileName="SaveData\\Data\\Match "+match+" Data.txt";
          }
          catch(NumberFormatException ex)
          {
            if(input.substring(input.length()-4).equals(".txt"))
            {
              fileName="SaveData\\Data\\"+input;
            }
            else
            {
              fileName="SaveData\\Data\\"+input+".txt";
            }
          }
          
          
          /*File file = new File("SaveData\\Data\\"+fileName);
          if(file.exists())
          {
            int overwrite;
            overwrite=JOptionPane.showConfirmDialog(null, "This file already exists. \nWould you like to overwrite?");
            if(overwrite==JOptionPane.YES_OPTION)
            {
              try{file.createNewFile();}
              catch(Exception ex){ex.printStackTrace();}
            }
          }
          else
            try{file.createNewFile();}
            catch(Exception ex){ex.printStackTrace();}*/
            
//          dataSaver.init(fileName);
          
          laptopGUI.savingData=true;
        }
//        else
//        {
//          laptopGUI.savingData=false;
//          dataSaver.STOP(); ///Hammer-time!
//          dataSaver=null;
//          actionPerformed(e);
//        }
        
      }
      else if(e.getSource()==closeButton)
      {
//        dataSaver.STOP();
        laptopGUI.savingData=false;
      }
      else if(e.getSource()==loadData)
      {
        
      }
    }
  }
}

