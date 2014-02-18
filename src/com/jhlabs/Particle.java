package com.jhlabs;

public class Particle
{
  protected int x, y;
  protected int vx, vy;
  public int size;
  public int color = 255;
  public int randomness = 0;
  public int lifetime = -1;
  private ParticleSystem particles;

  public Particle(ParticleSystem particles, int width, int height)
  {
    this.particles = particles;
  }

  public void move(int width, int height)
  {
    if (randomness != 0)
    {
      vx += (int) (Math.random() * randomness) - randomness / 2;
      vy += (int) (Math.random() * randomness) - randomness / 2;
    }
    x += vx;
    y += vy;
    vy += particles.gravity;
    lifetime--;
  }

  /*
   * How to draw circles of small sizes
   */
  public int[] circle1 = { 0, 1 };
  public int[] circle3 = { 0, 1, -1, 3, 0, 1 };
  public int[] circle5 = { -1, 3, -2, 5, -2, 5, -2, 5, -1, 3 };
  public int[] circle7 = { -1, 3, -2, 5, -3, 7, -3, 7, -3, 7, -2, 5, -1, 3 };
  public int[][] circles = { circle1, circle3, circle5, circle7 };

  public void paint(byte[] pixels, int width, int height)
  {
    byte pixel = (byte) color;
    int[] c = circles[Math.min(size, circles.length)];
    int my = (y >> 8) - size;
    for (int i = 0; i < c.length; i += 2, my++)
    {
      if (my < 0)
        continue;
      else if (my >= height)
        break;
      int x1 = Math.max(0, (x >> 8) + c[i]);
      int x2 = Math.min(width - 1, x1 + c[i + 1]);
      int j = my * width + x1;
      for (int mx = x1; mx <= x2; mx++)
        pixels[j++] = pixel;
    }
  }
}