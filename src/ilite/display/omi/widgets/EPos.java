package ilite.display.omi.widgets;

import java.awt.BorderLayout;

import javax.swing.SpringLayout;

public enum EPos
{
  NORTH(BorderLayout.NORTH, SpringLayout.NORTH),
  SOUTH(BorderLayout.SOUTH, SpringLayout.SOUTH),
  EAST(BorderLayout.EAST, SpringLayout.EAST),
  WEST(BorderLayout.WEST, SpringLayout.WEST),
  VOID("VOID", "VOID");
  
  public String border_layout(){ return mBorderLayout; }
  public String spring_layout(){ return mSpringLayout; }
  
  private String mBorderLayout;
  private String mSpringLayout;
  private EPos(String pBorderLayout, String pSpringLayout)
  {
    mBorderLayout = pBorderLayout;
    mSpringLayout = pSpringLayout;
  }
}
