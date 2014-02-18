package ilite.display.omi.input.match;

import java.awt.Color;

public enum EMatchItem {
	MATCH_NUM("Match Num", Color.BLACK),
	RED1("Team 1", Color.RED),
	RED2("Team 2", Color.RED),
	RED3("Team 3", Color.RED),
	BLUE1("Team 1", Color.BLUE),
	BLUE2("Team 2", Color.BLUE),
	BLUE3("Team 3", Color.BLUE),
	RSCORE("Red Score", Color.RED),
	BSCORE("Blue Score", Color.BLUE);
	
	@Override
  public String toString(){ return mLabel; }
	
	public Color getBackground(){ return mBackground; }
	
	private String mLabel;
	private Color mBackground;
	
	private EMatchItem(String pLabel, Color pBackground){
		mLabel = pLabel;
		mBackground = pBackground;
	}
}
