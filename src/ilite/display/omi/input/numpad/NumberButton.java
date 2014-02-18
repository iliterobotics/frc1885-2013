package ilite.display.omi.input.numpad;

import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;

public class NumberButton extends JButton implements ActionListener{
	private final static Dimension sButtonSize = new Dimension(50, 50);
	
	private INumPad mListener;
	private ENumPad mButton;
	
	public NumberButton(INumPad pListener, ENumPad pButton){
		super(pButton.toString());
		mListener = pListener;
		mButton = pButton;
		setSize(sButtonSize);
		setPreferredSize(sButtonSize);
		setMaximumSize(sButtonSize);
		setMinimumSize(sButtonSize);
//		setBackground(GuiConstants.background);
//		setForeground(Color.WHITE);
		addActionListener(this);
	}

	@Override
	public void actionPerformed(ActionEvent ae) {
		if(ae.getSource().equals(this)){
			mListener.performButtonPressed(mButton);
		}
	}
}
