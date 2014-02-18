package com.jhlabs;
/**
 * Scroll the image down
 */
public class ShiftDownFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    int i = 0;
    int j = 0;
    for (int x = 0; x < width; x++)
      out[j++] = 0;
    for (int y = 1; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        out[j++] = in[i++];
      }
    }
  }

  public String toString()
  {
    return "Move Down";
  }
}
