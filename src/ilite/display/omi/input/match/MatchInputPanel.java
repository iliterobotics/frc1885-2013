package ilite.display.omi.input.match;

import ilite.display.omi.input.numpad.ENumPad;
import ilite.display.omi.input.numpad.INumPad;
import ilite.display.omi.input.numpad.NumPadPanel;

import javax.swing.JLabel;
import javax.swing.JPanel;

public class MatchInputPanel extends JPanel implements INumPad{
	private JLabel mUserInputDisplay = new JLabel("");
	private int mUserInputValue = -1;
	
	public MatchInputPanel(){
		add(new NumPadPanel(this));
		add(mUserInputDisplay);
	}

	@Override
	public void performButtonPressed(ENumPad button) {
		switch(button){
		case NULL:
			// do nothing .... this is the only one that does nothing
			break;
		case BKSPC:
			clearUserInput();
			break;
		default:
			String text = mUserInputDisplay.getText().trim() + button.toString().trim();
			mUserInputDisplay.setText(text);
			mUserInputValue = Integer.parseInt(text);
		}
	}
	
	private void clearUserInput(){
		mUserInputDisplay.setText("");
	}
}
