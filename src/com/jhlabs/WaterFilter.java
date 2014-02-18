package com.jhlabs;


/**
 * A sort of water-ripple type effect
 */
class WaterFilter extends Filter
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
    for (int y = 1; y < height - 1; y++)
    {
      int index = y * width;
      index++;
      for (int x = 1; x < width - 1; x++)
      {
        int n = (byte) (((in[index - 1] & 0xff) + (in[index + 1] & 0xff)
            + (in[index - width] & 0xff) + (in[index + width] & 0xff)) / 2 - (out[index] & 0xff));
        n -= (byte) (n >> 6);
        out[index] = (byte) Math.max(Math.min(n, 255), 0);
        // out[index] = (byte)ImageMath.clamp(n, 0, 255);
        index++;
      }
      index++;
    }
  }

  public String toString()
  {
    return "Ripple";
  }
}