package ilite.display.modules.displaylog;

import ilite.display.omi.components.IliteExpandingPanel;
import ilite.util.gui.components.EPanelOrientation;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ELevel;

import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JToggleButton;
import javax.swing.SpringLayout;

public class DisplayLogOptionsPanel extends IliteExpandingPanel{
  private JComboBox<ELevel> mDebugLevels = new JComboBox<>(ELevel.values());
  private JCheckBox mEnableLevelsBox = new JCheckBox("Show Levels");
  private JCheckBox mEnableTimeBox = new JCheckBox("Show Time");
  private JCheckBox mEnableThreadBox = new JCheckBox("Show Threads");
  private JCheckBox mEnableMethodBox = new JCheckBox("Show Methods");
  private JLabel mMinLevelLabel = new JLabel("Logging Level:");
  private JLabel mLogOptions = new JLabel("Log Options");
  private JToggleButton mClearButton = new JToggleButton("Clear Log");
  private IUpdate<DisplayLogModuleModel.LoggingOptions> mListener = null;

  public DisplayLogOptionsPanel() {
    super(EPanelOrientation.DOWN_LEFT);
    mEnableLevelsBox.addActionListener(new OptionListener());
    mEnableTimeBox.addActionListener(new OptionListener());
    mEnableThreadBox.addActionListener(new OptionListener());
    mEnableMethodBox.addActionListener(new OptionListener());
    mDebugLevels.addActionListener(new OptionListener());
    mClearButton.addActionListener(new OptionListener());
    
    mEnableLevelsBox.setSelected(true);
    mEnableTimeBox.setSelected(true);
    mEnableThreadBox.setSelected(true);
    mEnableMethodBox.setSelected(true);

    mPanel.setPreferredSize(getPanelPreferredSize());		
    mPanel.add(mLogOptions);

    SpringLayout psl = new SpringLayout();
    mPopupPanel.setLayout(psl);
    mPopupPanel.add(mMinLevelLabel);
    mPopupPanel.add(mDebugLevels);
    mPopupPanel.add(mEnableLevelsBox);
    mPopupPanel.add(mEnableTimeBox);
    mPopupPanel.add(mEnableThreadBox);
    mPopupPanel.add(mEnableMethodBox);
    mPopupPanel.add(mClearButton);

    psl.putConstraint(SpringLayout.HORIZONTAL_CENTER, mMinLevelLabel, 0, SpringLayout.HORIZONTAL_CENTER, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mDebugLevels, INSET, SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.EAST, mDebugLevels, -INSET, SpringLayout.EAST, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mEnableLevelsBox, INSET, SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mEnableTimeBox, INSET, SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mEnableThreadBox, INSET, SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mEnableMethodBox, INSET, SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.WEST, mClearButton, INSET, SpringLayout.WEST, mPopupPanel);
    psl.putConstraint(SpringLayout.EAST, mClearButton, -INSET, SpringLayout.EAST, mPopupPanel);

    psl.putConstraint(SpringLayout.NORTH, mMinLevelLabel, INSET, SpringLayout.NORTH, mPopupPanel);
    psl.putConstraint(SpringLayout.NORTH, mDebugLevels, 1, SpringLayout.SOUTH, mMinLevelLabel);
    psl.putConstraint(SpringLayout.NORTH, mEnableLevelsBox, INSET, SpringLayout.SOUTH, mDebugLevels);
    psl.putConstraint(SpringLayout.NORTH, mEnableTimeBox, INSET, SpringLayout.SOUTH, mEnableLevelsBox);
    psl.putConstraint(SpringLayout.NORTH, mEnableThreadBox, INSET, SpringLayout.SOUTH, mEnableTimeBox);
    psl.putConstraint(SpringLayout.NORTH, mEnableMethodBox, INSET, SpringLayout.SOUTH, mEnableThreadBox);
    psl.putConstraint(SpringLayout.NORTH, mClearButton, 2*INSET, SpringLayout.SOUTH, mEnableMethodBox);
  }

  void setOptionsListener(IUpdate<DisplayLogModuleModel.LoggingOptions> pListener)
  {
    mListener = pListener;
  }

  @Override
  protected Dimension getPopupPreferredSize() {
    FontMetrics fm = mEnableLevelsBox.getFontMetrics(mEnableLevelsBox.getFont());
    int w = fm.stringWidth(mMinLevelLabel.getText()) + 2*INSET;
    int h = fm.getHeight() * 6 + mDebugLevels.getHeight() + INSET * 2 + 1;
    return new Dimension(w,h);
  }

  @Override
  protected Dimension getPanelPreferredSize()
  {
    FontMetrics fm = mLogOptions.getFontMetrics(mLogOptions.getFont());
    int wd = fm.stringWidth(mLogOptions.getText());
    return new Dimension(wd + 2*INSET, fm.getHeight() + 2*INSET);
  }

  private void optionsChanged()
  {
    DisplayLogModuleModel.LoggingOptions opt = new DisplayLogModuleModel.LoggingOptions();
    opt.enableLevel = mEnableLevelsBox.isSelected();
    opt.enableMethods = mEnableMethodBox.isSelected();
    opt.enableThreads = mEnableThreadBox.isSelected();
    opt.enableTime = mEnableTimeBox.isSelected();
    opt.level = ELevel.values()[mDebugLevels.getSelectedIndex()];
    opt.clearLog = mClearButton.isSelected();
    mClearButton.setSelected(false);
    mListener.update(opt);
  }

  private class OptionListener implements ActionListener{
    @Override
    public void actionPerformed(ActionEvent e) {
      optionsChanged();			
    }

  }

}
