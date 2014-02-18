package ilite.display.modules.display2013;

import java.awt.Color;

public enum EGoals
{
	  ACCESSIBLE(Color.GREEN, true),
	  UNACCESSIBLE(Color.GRAY, false);
	  
	  public Color getColor(){ return mColor; }
	  public boolean isAccessible(){ return mAccess; }
	  
	  private Color mColor;
	  private boolean mAccess;
	  
	  private EGoals(Color pColor, boolean pAccess)
	  {
	    mColor =  pColor;
	    mAccess = pAccess;
	  }
	  public static EGoals getFromAccess(boolean canAccess)
	  {
		  if(canAccess)
			 return ACCESSIBLE;
		  else
			 return UNACCESSIBLE;
	  }
}
