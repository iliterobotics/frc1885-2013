package ilite.display.modules.display2014;

import ilite.data.models.RobotDebugMessage;
import ilite.util.lang.IUpdate;

import javax.swing.BorderFactory;
import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.SpringLayout;

public class RobotDebugPrinter extends JPanel
implements IUpdate<RobotDebugMessage>
{
  JList<RobotDebugMessage> mMessages;
  DefaultListModel<RobotDebugMessage> mDebugMsgModel;
  public RobotDebugPrinter()
  {
    mDebugMsgModel = new DefaultListModel<>();
    mMessages = new JList<>(mDebugMsgModel);
    JScrollPane jsp = new JScrollPane(mMessages, 
        JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, 
        JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
    add(jsp);
    
    SpringLayout sl = new SpringLayout();
    setLayout(sl);
    setBorder(BorderFactory.createTitledBorder("Debug Messages"));
    sl.putConstraint(SpringLayout.WEST, jsp, 5, SpringLayout.WEST, this);
    sl.putConstraint(SpringLayout.EAST, jsp, -5, SpringLayout.EAST, this);
    sl.putConstraint(SpringLayout.NORTH, jsp, 5, SpringLayout.NORTH, this);
    sl.putConstraint(SpringLayout.SOUTH, jsp, -5, SpringLayout.SOUTH, this);
  }
  
  public void setMaxLines(int pMax)
  {
    mDebugMsgModel.setSize(pMax);
  }

  @Override
  public void update(RobotDebugMessage pObject)
  {
    mDebugMsgModel.add(0, pObject);
    mDebugMsgModel.trimToSize();
  }
}
