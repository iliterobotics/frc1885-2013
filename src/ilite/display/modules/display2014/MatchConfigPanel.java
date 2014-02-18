package ilite.display.modules.display2014;

import static javax.swing.SpringLayout.EAST;
import static javax.swing.SpringLayout.HORIZONTAL_CENTER;
import static javax.swing.SpringLayout.NORTH;
import static javax.swing.SpringLayout.SOUTH;
import static javax.swing.SpringLayout.WEST;
import ilite.util.gui.SpringUtils;
import ilite.util.lang.Delegator;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.StringTokenizer;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.SpringLayout;
import javax.swing.SwingConstants;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

public class MatchConfigPanel extends Delegator<List<String>>
{
  private DefaultListModel<String> mExistingMatches = new DefaultListModel<>();
  private JList<String> mMatches = new JList<>(mExistingMatches);
  private JPanel mMainPanel = new JPanel();
  private static final String[] sElimMatches = new String[]{
    "QF1", "QF2", "QF3", "SF1", "SF2", "SF3", "F1", "F2", "F3"
  };

  private JTextField yearInputField = new JTextField("2014");
  private JTextField eventInputField = new JTextField(20);
  private JTextField mMatchInputField = new JTextField(4);
  private JButton addButton = new JButton("Add Qualification Match");
  private JButton elimButton = new JButton("Generate Elimination Matches");
  private JButton remButton = new JButton("Remove Selected Match");
  private JButton saveButton = new JButton("Save to File");
  private JButton loadButton = new JButton("Load from File");
  private JLabel mOutputFileLabel = new JLabel("", SwingConstants.RIGHT);
  private static final int inset = 5;
  private final String mRootDataFolder;
  private String mEventDataFolder = "";
  public static final String sMatchFileName = "Matches.txt";
  
  public MatchConfigPanel(String pRootDataFolder)
  {
    mRootDataFolder = pRootDataFolder;
    SpringLayout sl = new SpringLayout();
    mMainPanel.setLayout(sl);
    JPanel input = getInputPanel();
    JPanel list = getListPanel();
    JPanel ctrl = getControlPanel();
    mMainPanel.add(input);
    mMainPanel.add(list);
    mMainPanel.add(ctrl);
    
    int ctrlheight = 50;
    sl.putConstraint(NORTH, input, inset, NORTH, mMainPanel);
    sl.putConstraint(NORTH, list, inset, NORTH, mMainPanel);
    sl.putConstraint(WEST, input, inset, WEST, mMainPanel);
    sl.putConstraint(EAST, input, -inset, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(WEST, list, inset, HORIZONTAL_CENTER, mMainPanel);
    sl.putConstraint(EAST, list, -inset, EAST, mMainPanel);
    sl.putConstraint(SOUTH, input, -(ctrlheight+2*inset), SOUTH, mMainPanel);
    sl.putConstraint(SOUTH, list, -(ctrlheight+2*inset), SOUTH, mMainPanel);
    sl.putConstraint(NORTH, ctrl, inset, SOUTH, input);
    SpringUtils.stickToSides(sl, ctrl, inset, mMainPanel);
    sl.putConstraint(SOUTH, ctrl, -inset, SOUTH, mMainPanel);
    setupListeners();
    
    mMainPanel.setPreferredSize(new Dimension(1024, 768));
    mMainPanel.setBorder(BorderFactory.createTitledBorder("Match Configuration"));
    input.setBorder(BorderFactory.createTitledBorder("Match Input"));
    list.setBorder(BorderFactory.createTitledBorder("Current Matches"));
    updateMatchFile();
  }
  
  private JPanel getInputPanel()
  {
    SpringLayout sl = new SpringLayout();
    JPanel p = new JPanel(sl);
    
    int mult = 5;
    SpringUtils.SpringLayoutHelper helper = new SpringUtils.SpringLayoutHelper(sl, inset, p);
    addToInputPanel(helper, p, new JLabel("Event: ", SwingConstants.CENTER), 1);
    addToInputPanel(helper, p, eventInputField, 1);
    addToInputPanel(helper, p, new JLabel("Year: ", SwingConstants.CENTER), 1);
    addToInputPanel(helper, p, yearInputField, 1);
    addToInputPanel(helper, p, new JLabel("Qual Match Number:", SwingConstants.CENTER), mult);
    addToInputPanel(helper, p, mMatchInputField, 1);
    addToInputPanel(helper, p, addButton, 1);
    addToInputPanel(helper, p, new JLabel("Match Output File:", SwingConstants.CENTER), mult);
    addToInputPanel(helper, p, new JLabel(mRootDataFolder), 1);
    addToInputPanel(helper, p, mOutputFileLabel, 1);
    addToInputPanel(helper, p, elimButton, mult);
    
    return p;
  }
  
  private static void addToInputPanel(
      SpringUtils.SpringLayoutHelper pHelper, JPanel pPanel, Component pComponent, int pMult)
  {
    pPanel.add(pComponent);
    pHelper.stickToSides(pComponent);
    pHelper.sequenceTopToBottom(pComponent, pMult);
  }
  
  private JPanel getControlPanel()
  {
    SpringLayout sl = new SpringLayout();
    JPanel p = new JPanel(sl);
    p.add(loadButton);
    p.add(saveButton);
    
    sl.putConstraint(WEST, loadButton, inset, WEST, p);
    sl.putConstraint(EAST, loadButton, -inset, HORIZONTAL_CENTER, p);
    sl.putConstraint(WEST, saveButton, inset, HORIZONTAL_CENTER, p);
    sl.putConstraint(EAST, saveButton, -inset, EAST, p);
    SpringUtils.stickToTopBottom(sl, loadButton, inset, p);
    SpringUtils.stickToTopBottom(sl, saveButton, inset, p);
    return p;
  }
  
  private JPanel getListPanel()
  {
    SpringLayout sl = new SpringLayout();
    JPanel p = new JPanel(sl);
    JScrollPane jsp = new JScrollPane(mMatches,
        JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, 
        JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
    p.add(jsp);
    p.add(remButton);
    
    sl.putConstraint(NORTH, jsp, inset, NORTH, p);
    SpringUtils.stickToSides(sl, jsp, inset, p);
    sl.putConstraint(SOUTH, jsp, -(50+inset*2), SOUTH, p);
    sl.putConstraint(NORTH, remButton, inset, SOUTH, jsp);
    sl.putConstraint(SOUTH, remButton, -inset, SOUTH, p);
    SpringUtils.stickToSides(sl, remButton, inset, p);
    
    return p;
  }
  
  private void setupListeners()
  {
    elimButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        for(String s : sElimMatches)
        {
          mExistingMatches.addElement(s);
        }
      }
    });
    addButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        mExistingMatches.addElement("Q" + mMatchInputField.getText());
        mMatchInputField.setText("");
      }
    });
    remButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        int i = mMatches.getSelectedIndex();
        if(i > 0)
        {
          mExistingMatches.removeElementAt(i);          
        }
      }
    });
    saveButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        StringBuilder sb = new StringBuilder();
        Enumeration<String> elements = mExistingMatches.elements(); 
        while(elements.hasMoreElements())
        {
          sb.append(elements.nextElement());
          sb.append(',');
        }
        String path = mRootDataFolder + mEventDataFolder;
        File dirs = new File(path);
        if(!dirs.exists())
        {
          dirs.mkdirs();
        }
        try(BufferedWriter writer = new BufferedWriter(new FileWriter(new File(path, sMatchFileName))))
        {
          writer.write(sb.toString());
          writer.close();
        } catch (IOException e1)
        {
          e1.printStackTrace();
        }
      }
    });
    loadButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        mExistingMatches.removeAllElements();
        String path = mRootDataFolder + mEventDataFolder;
        File f = new File(path, sMatchFileName);
        if(f.exists() && f.canRead())
        {
          for(String s : parseMatchesFile(f))
          {
            mExistingMatches.addElement(s);
          }
        }
      }
    });
    DocumentListener dl = new DocumentListener()
    {
      @Override public void removeUpdate(DocumentEvent e){updateMatchFile();}      
      @Override public void insertUpdate(DocumentEvent e){updateMatchFile();}      
      @Override public void changedUpdate(DocumentEvent e){updateMatchFile();}
    };
    eventInputField.getDocument().addDocumentListener(dl);
    yearInputField.getDocument().addDocumentListener(dl);
  }
  
  private void updateMatchFile()
  {
    mEventDataFolder = eventInputField.getText() + yearInputField.getText();
    mOutputFileLabel.setText(mEventDataFolder + File.separator + sMatchFileName); 
  }
  
  public JPanel getDisplayPanel()
  {
    return mMainPanel;
  }
  
  public static List<String> parseMatchesFile(File f)
  {
    List<String> results = new ArrayList<>();
    try(BufferedReader reader = new BufferedReader(new FileReader(f)))
    {
      String s = reader.readLine();
      if(s != null && s.length() > 0)
      {
        StringTokenizer st = new StringTokenizer(s, ",");
        while(st.hasMoreTokens())
        {
          String match = st.nextToken();
          if(match != null)
          {
            results.add(match);
          }
        }
      }
    } catch (FileNotFoundException e1)
    {
      e1.printStackTrace();
    } catch (IOException e2)
    {
      e2.printStackTrace();
    }
    return results;
  }
}
