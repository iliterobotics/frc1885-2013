package com.jhlabs;

/**
 * Blur horizontally
 */
public class BlurHFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    for (int y = 0; y < height; y++)
    {
      int index = y * width;
      out[index] = (byte) (((in[index] & 0xff) + (in[index + 1] & 0xff)) / 3);
      index++;
      for (int x = 1; x < width - 1; x++)
      {
        out[index] = (byte) (((in[index - 1] & 0xff) + (in[index] & 0xff) + (in[index + 1] & 0xff)) / 3);
        index++;
      }
      out[index] = (byte) (((in[index - 1] & 0xff) + (in[index] & 0xff)) / 3);
    }
  }

  public String toString()
  {
    return "Blur Horizontally";
  }
}
