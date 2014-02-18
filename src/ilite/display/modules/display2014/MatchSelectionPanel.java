package ilite.display.modules.display2014;

import static javax.swing.SpringLayout.NORTH;
import static javax.swing.SpringLayout.SOUTH;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.List;

import ilite.util.gui.SpringUtils;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.SpringLayout;
import javax.swing.SwingConstants;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class MatchSelectionPanel
{
  private final DefaultListModel<String> mEventsModel = new DefaultListModel<>();
  private final JList<String> mEventsList = new JList<>(mEventsModel);
  private final DefaultListModel<String> mMatchesModel = new DefaultListModel<>();
  private final JList<String> mMatchesList = new JList<>(mMatchesModel);
  private final JButton mPracticeButton = new JButton("Practice Round");
  private final JButton mRefreshEventsButton = new JButton("Refresh Events");
  private final JLabel mOutputFileLabel = new JLabel("", SwingConstants.RIGHT);
  private String mCurrentEvent = "";
  private final JPanel mMainPanel = new JPanel();
  
  public MatchSelectionPanel(int pad)
  {
    SpringLayout sl = new SpringLayout();
    mMainPanel.setLayout(sl);
    mMainPanel.setBorder(BorderFactory.createTitledBorder("Match & Output Folder Selection"));
    SpringLayout sl2 = new SpringLayout();
    JPanel eventPanel = new JPanel(sl2);
    JScrollPane ejsp = new JScrollPane(mEventsList, 
        JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, 
        JScrollPane.HORIZONTAL_SCROLLBAR_NEVER); 
    eventPanel.add(ejsp);
    eventPanel.setBorder(BorderFactory.createTitledBorder("Select an Event"));
    SpringUtils.stickToAllSides(sl2, ejsp, pad, eventPanel);
    mMainPanel.add(eventPanel);
    
    SpringLayout sl3 = new SpringLayout();
    JPanel matchPanel = new JPanel(sl3);
    matchPanel.setBorder(BorderFactory.createTitledBorder("Select Match"));
    JScrollPane jsp = new JScrollPane(mMatchesList, 
            JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED, 
            JScrollPane.HORIZONTAL_SCROLLBAR_NEVER); 
    matchPanel.add(jsp);
    SpringUtils.stickToAllSides(sl3, jsp, pad, matchPanel);
    
    mMainPanel.add(matchPanel);
    mMainPanel.add(mPracticeButton);
    mMainPanel.add(mRefreshEventsButton);
    sl.putConstraint(NORTH, mPracticeButton, pad, NORTH, mMainPanel);
    SpringUtils.stickToSides(sl, mPracticeButton, pad, mMainPanel);
    sl.putConstraint(NORTH, mRefreshEventsButton, 3*pad, SOUTH, mPracticeButton);
    SpringUtils.stickToSides(sl, mRefreshEventsButton, pad, mMainPanel);
    sl.putConstraint(NORTH, eventPanel, pad, SOUTH, mRefreshEventsButton);
    SpringUtils.stickToSides(sl, eventPanel, pad, mMainPanel);
    sl.putConstraint(NORTH, matchPanel, 120+2*pad, SOUTH, mRefreshEventsButton);
    sl.putConstraint(SOUTH, eventPanel, -pad, NORTH, matchPanel);
    SpringUtils.stickToSides(sl, matchPanel, pad, mMainPanel);
    sl.putConstraint(SOUTH, matchPanel, -(50+2*pad), SOUTH, mMainPanel);

    JLabel folderLabel = new JLabel("Output Folder:");
    mOutputFileLabel.setFont(mOutputFileLabel.getFont().deriveFont(12f));
    mMainPanel.add(folderLabel);
    mMainPanel.add(mOutputFileLabel);
    sl.putConstraint(NORTH, folderLabel, pad, SOUTH, matchPanel);
    SpringUtils.stickToSides(sl, folderLabel, pad, mMainPanel);
    sl.putConstraint(NORTH, mOutputFileLabel, pad, SOUTH, folderLabel);
    SpringUtils.stickToSides(sl, mOutputFileLabel, pad, mMainPanel);
    
    
    // Listeners
    final String root = DataManager2014.getInstance().getRootDataFolder();
    mEventsList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    mEventsList.addListSelectionListener(new ListSelectionListener()
    {
      @Override
      public void valueChanged(ListSelectionEvent e)
      {
        mMatchesModel.removeAllElements();
        String event = mEventsList.getSelectedValue();
        if(event != null)
        {
          mCurrentEvent = event;
          File file = new File(root + event, MatchConfigPanel.sMatchFileName);
          if(file.exists() && file.canRead())
          {
            List<String> matches = MatchConfigPanel.parseMatchesFile(file);
            for(String s : matches)
            {
              mMatchesModel.addElement(s);
            }
          }
        }
      }
    });
    mMatchesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
    mMatchesList.setLayoutOrientation(JList.HORIZONTAL_WRAP);
    mMatchesList.addListSelectionListener(new ListSelectionListener()
    {
      @Override
      public void valueChanged(ListSelectionEvent e)
      {
        String match = mMatchesList.getSelectedValue();
        if(match != null)
        {
          String output = DataManager2014.getInstance().setEventAndMatch(mCurrentEvent, match);
          mOutputFileLabel.setText(output);
        }
      }
    });
    mPracticeButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        String event = mEventsList.getSelectedValue();
        String output = DataManager2014.getInstance().setToPractice(event);
        mOutputFileLabel.setText(output);
      }
    });
    mRefreshEventsButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        refreshEventList();
      }
    });
    
    refreshEventList();
  }
  
  public JPanel getDisplayPanel()
  {
    return mMainPanel;
  }
  
  private void refreshEventList()
  {
    mEventsModel.removeAllElements();
    final String root = DataManager2014.getInstance().getRootDataFolder();
    File[] files = new File(root).listFiles();
    if(files != null && files.length > 0)
    {
      for(File f : files)
      {
        if(f.isDirectory())
        {
          String event = f.getPath().replace(root, "");
          mEventsModel.addElement(event);
        }
      }
    }
  }
}
