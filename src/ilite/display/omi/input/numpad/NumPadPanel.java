package ilite.display.omi.input.numpad;

import java.awt.GridLayout;

import javax.swing.JPanel;

public class NumPadPanel extends JPanel{
	public NumPadPanel(final INumPad pParent){
		setLayout(new GridLayout(4, 3, 2, 2));
		for(ENumPad np : ENumPad.values()){
			add(new NumberButton(pParent, np));
		}
	}
}
