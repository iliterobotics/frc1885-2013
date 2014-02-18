package com.jhlabs;

/**
 * Zoom the image out to the left and right
 */
public class ZoomInVFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    int i = 0;
    int j = 0;
    int height2 = height / 2;
    for (int x = 0; x < width; x++)
      out[j++] = 0;
    j = width;
    for (int y = 1; y < height2; y++)
    {
      for (int x = 0; x < width; x++)
      {
        out[j++] = in[i++];
      }
    }
    i += 2 * width;
    for (int y = 1; y < height2; y++)
    {
      for (int x = 0; x < width; x++)
      {
        out[j++] = in[i++];
      }
    }
    for (int x = 0; x < width; x++)
      out[j++] = 0;
  }

  public String toString()
  {
    return "Move In Vertical";
  }
}
