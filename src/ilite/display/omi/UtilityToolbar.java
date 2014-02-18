package ilite.display.omi;

import static javax.swing.SpringLayout.EAST;
import static javax.swing.SpringLayout.VERTICAL_CENTER;
import static javax.swing.SpringLayout.WEST;
import ilite.display.Constants;
import ilite.display.IApplicationController;
import ilite.display.IMasterController;
import ilite.display.config.ISystemConfiguration;
import ilite.display.config.UIOptions;
import ilite.display.omi.tools.AbstractCommand;
import ilite.display.omi.tools.ExitCommand;
import ilite.util.lang.IUpdate;

import java.awt.Dimension;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JToolBar;
import javax.swing.SpringLayout;

public class UtilityToolbar
{
  
  private final JToolBar mJToolbar = new JToolBar();

  private static int sINSET=4;
  
  private static final Dimension sSepDimension = new Dimension(2*sINSET, 32);

  private JPanel mToolbar = new JPanel();
  private JLabel mTitle;
  private JPanel mOptionsPanel = new JPanel();
  private JComponent mLastComponent = null;

  public UtilityToolbar(IMasterController pMaster, IApplicationController pApp)
  {
    ExitCommand exit = new ExitCommand(pApp);
        
    IUpdate<String> organizationChangeListener = new IUpdate<String>()
    {
      @Override public void update(String pString)
      {
        mTitle.setText(pString);
      }
    };

    ISystemConfiguration config = pMaster.getSystemConfiguration();
    config.addPropertyChangeListener(Constants.sORGANIZATION_KEY, organizationChangeListener);
    SpringLayout sl = new SpringLayout();
    mToolbar.setLayout(sl);
    mTitle = new JLabel(config.getString(Constants.sORGANIZATION_KEY));

    JButton out = new JButton(exit);
    out.setText(exit.getName());
    Dimension d = out.getPreferredSize();
    int h = (int)d.getHeight() + sINSET;
    mToolbar.setPreferredSize(new Dimension((int)d.getWidth(), h));
    
    mOptionsPanel.setPreferredSize(new Dimension(250, h));
    mToolbar.add(mTitle);
    mToolbar.add(mOptionsPanel);
    mToolbar.add(out);
    mToolbar.add(mJToolbar);
    
    sl.putConstraint(VERTICAL_CENTER, out, 0, VERTICAL_CENTER, mToolbar);
    sl.putConstraint(VERTICAL_CENTER, mOptionsPanel, 0, VERTICAL_CENTER, mToolbar);
    sl.putConstraint(VERTICAL_CENTER, mTitle, 0, VERTICAL_CENTER,  mToolbar);
    
    sl.putConstraint(EAST, out, -1, EAST, mToolbar);
    sl.putConstraint(EAST, mOptionsPanel, -sINSET, WEST, out);
    sl.putConstraint(EAST, mTitle, -sINSET, WEST, mOptionsPanel);
    

    mToolbar.setBackground(UIOptions.darkBlob);
  }
  
  public void addSeparator()
  {
    mJToolbar.addSeparator(sSepDimension);
  }
  
  public void addTool(AbstractCommand pCommand)
  {
    JButton b = mJToolbar.add(pCommand);
    ImageIcon i = pCommand.getIcon();
    if(i == null)
    {
      b.setText(pCommand.getName());
    }
    else
    {
      b.setIcon(i);
    }
  }
  
  public void setOptionsComponent(JComponent pComponent)
  {
	  if(mLastComponent != null)
	  {
		  mOptionsPanel.remove(mLastComponent);  
	  }
	  if(pComponent != null)
	  {
		  mOptionsPanel.add(pComponent);
	  }
	  mLastComponent = pComponent;
  }
  
  public JPanel getToolbar()
  {
    return mToolbar;
  }
}
