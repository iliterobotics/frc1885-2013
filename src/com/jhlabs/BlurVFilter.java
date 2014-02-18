package com.jhlabs;

/**
 * Blur vertically
 */
public class BlurVFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    for (int x = 0; x < width; x++)
    {
      int index = x;
      out[index] = (byte) (((in[index] & 0xff) + (in[index + width] & 0xff)) / 3);
      index += width;
      for (int y = 1; y < height - 1; y++)
      {
        out[index] = (byte) (((in[index - width] & 0xff) + (in[index] & 0xff) + (in[index
            + width] & 0xff)) / 3);
        index += width;
      }
      out[index] = (byte) (((in[index - width] & 0xff) + (in[index] & 0xff)) / 3);
    }
  }

  public String toString()
  {
    return "Blur Vertically";
  }
}