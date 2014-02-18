package com.jhlabs;

public class ThingFilter extends ConvolveFilter
{
  /*
   * protected static int[] kernel = { 10, 0, 10, 0, 10, 0, 0, 0, 0, 0, 0, 10,
   * 0, 10, 0, 0, 0, 0, 0, 0, 10, 0, 10, 0, 10, };
   */
  protected static int[] kernel = { 10, 0, 0, 0, 10, 0, 10, 0, 10, 0, 0, 0, 10,
      0, 0, 0, 10, 0, 10, 0, 10, 0, 0, 0, 10, };

  public ThingFilter()
  {
    super(kernel, 120);
  }

  public String toString()
  {
    return "Thing";
  }
}