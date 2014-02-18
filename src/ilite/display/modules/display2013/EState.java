package ilite.display.modules.display2013;

import java.awt.Color;


public enum EState
{
  EXTENDED(Color.GREEN, true),
  RETRACTED(Color.RED, false);
  
  public Color getColor(){ return mColor; }
  public boolean isExtended(){ return mExtended; }
  
  private Color mColor;
  private boolean mExtended;
  
  private EState(Color pColor, boolean pExtended)
  {
    
  }
  
  /*
	public static boolean mHopperState = false, mSpatulaState = false, mDiscLiftState = false, mLiftState= false;
	public static boolean mSpinnerState = false, mPneumaticState = false;
	
	public EState()
	{	}
	
	public void setHopperState(boolean State)		
	{ mHopperState = State; }
	public void setSpatulaState(boolean State)	
	{ mSpatulaState = State; }
	public void setDiscLiftState(boolean State)
	{ mDiscLiftState = State; }
	public void setLiftState(boolean State)
	{ mLiftState = State; }
	public void setSpinnerState(boolean State)
	{ mSpinnerState = State; }
	public void setPneumaticState(boolean State)
	{ mPneumaticState = State; }
	public static boolean getHopperState()
	{ return mHopperState; }
	public static boolean getSpatulaState()
	{ return mSpatulaState; }
	public static boolean getDiscLiftState()
	{ return mDiscLiftState; }
	public static boolean getLiftState()
	{ return mLiftState; }
	public static boolean getSpinnerState()
	{ return mSpinnerState; }
	public static boolean getPneumaticState()
	{ return mPneumaticState; }
	public static Color getAllianceColor()
	{  return null;}
	*/
}
