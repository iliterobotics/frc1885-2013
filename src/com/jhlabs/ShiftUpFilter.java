package com.jhlabs;

/**
 * Scroll the image up
 */
public class ShiftUpFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    int i = width * height - 1;
    int j = i;
    for (int x = 0; x < width; x++)
      out[j--] = 0;
    for (int y = 1; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        out[j--] = in[i--];
      }
    }
  }

  public String toString()
  {
    return "Move Up";
  }
}
