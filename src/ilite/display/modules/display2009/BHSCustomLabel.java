package ilite.display.modules.display2009;

/*BHS Team 1885 BHSCustomLabel.java
 * 
 * 
 * A panel that is used for floats and integers that change constantly.
 * This only changes the data entered, so that the name of the data isn't flashing
 *    the whole time.
 */

import java.awt.Label;
import java.awt.Panel;


/* BHSCustomLabel(String aLabel):
*   Constructor. Builds the object. 
*   Parameters:
*     aLabel: the name of the data you want to show.
*/
public class BHSCustomLabel extends Panel {
  private float mFData;
  private int mIData;
  private Label mSLabel, mDLabel;
  private boolean mFloatInit, mIntInit;
  
  //Constructors
  
  public BHSCustomLabel(String aLabel) {
	  //Makes new labels.
	  mSLabel = new Label(aLabel);
	  mDLabel = new Label(" "); //Initializes as a ' '
	  
	  //Adds the labels to the panel.
	  add(mSLabel);
	  add(mDLabel);
  }
  
 /*update(int aData) / update(float aData) 
 *   Updates the data in the label.
 *   Parameters:
 *     aData - the new data that you want to use.
 */
  public void update(float aData) {
	  //If the data is different or this hasn't been initialized yet...
	  if(mFData!=aData||!mFloatInit) {
		  if(!mFloatInit)
		      mFloatInit = true;
		  mFData = aData; //Reset the data
		  mDLabel.setText(mFData+""); //Reset the label text
	  }	  
  }
  
/*update(int aData) / update(float aData) 
 *   Updates the data in the label.
 *   Parameters:
 *     aData - the new data that you want to use.
 */
  public void update(int aData) {
	  //If the data is different or this hasn't been initialized yet...
	  if(mIData!=aData||!mIntInit) {
		  if(!mIntInit)
		      mIntInit = true; //We have initialized this value...
		  mIData = aData; //Reset the data
		  mDLabel.setText(mIData+""); //Reset the label text
	  }	  
  }
}
