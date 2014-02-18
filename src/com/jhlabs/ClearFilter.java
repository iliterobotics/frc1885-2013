package com.jhlabs;


/**
 * Clears the image to black - you only see the moving particles with this one.
 */
public class ClearFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    int index = 0;
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        out[index] = 0;
        index++;
      }
    }
  }

  public String toString()
  {
    return "Clear";
  }
}
