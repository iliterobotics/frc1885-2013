package com.jhlabs;
/**
 * Do a general convolution on the image (this is much slower than the blur
 * filters)
 */
public class ConvolveFilter extends Filter
{
  protected int[] kernel = { -3, 0, 0, 0, -3, 0, 0, 0, 0, 0, -3, 0, 50, 0, -3,
      0, 0, 0, 0, 0, -3, 0, 0, 0, -3, };
  int target = 40;

  public ConvolveFilter()
  {
  }

  public ConvolveFilter(int[] kernel, int target)
  {
    this.kernel = kernel;
    this.target = target;
  }

  public void apply(byte[] in, byte[] out, int width, int height)
  {
    int index = 0;
    int rows = 5;
    int cols = 5;
    int rows2 = rows / 2;
    int cols2 = cols / 2;

    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        int t = 0;

        for (int row = -rows2; row <= rows2; row++)
        {
          int iy = y + row;
          int ioffset;
          if (0 <= iy && iy < height)
            ioffset = iy * width;
          else
            ioffset = y * width;
          int moffset = cols * (row + rows2) + cols2;
          for (int col = -cols2; col <= cols2; col++)
          {
            int f = kernel[moffset + col];

            if (f != 0)
            {
              int ix = x + col;
              if (!(0 <= ix && ix < width))
                ix = x;
              t += f * (in[ioffset + ix] & 0xff);
            }
          }
        }
        t /= target;
        if (t > 255)
          t = 255;
        out[index++] = (byte) t;
      }
    }
  }

  public String toString()
  {
    return "Convolve";
  }
}