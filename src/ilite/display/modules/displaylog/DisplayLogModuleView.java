package ilite.display.modules.displaylog;

import ilite.util.gui.components.OutputTextArea;

import java.awt.Container;

import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.SpringLayout;

public class DisplayLogModuleView {
	private JPanel mPanel = new JPanel();
	/*package*/ OutputTextArea mOutput = new OutputTextArea();
	private JComponent mOptionsComponent = null;
	

	public DisplayLogModuleView()
	{
		SpringLayout sl = new SpringLayout();
		JComponent c = mOutput.getTextArea();
		mPanel.setLayout(sl);
		mPanel.add(c);		
		sl.putConstraint(SpringLayout.WEST, c, 0, SpringLayout.WEST, mPanel);
		sl.putConstraint(SpringLayout.EAST, c, 0, SpringLayout.EAST, mPanel);
		sl.putConstraint(SpringLayout.NORTH, c, 0, SpringLayout.NORTH, mPanel);
		sl.putConstraint(SpringLayout.SOUTH, c, 0, SpringLayout.SOUTH, mPanel);
	}

	public Container getDisplayPanel() {
		
		return mPanel;
	}
	
	public JComponent getOptionsComponent()
	{
		return mOptionsComponent;
	}
	
	/* package */ void setOptionsComponent(JComponent pComponent)
	{
		mOptionsComponent = pComponent;
	}
}
