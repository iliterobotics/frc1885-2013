package com.jhlabs;
/**
 * A particle system. We use fixed point integer maths with 8 fractional bits
 * everywhere for speed, hence the frequent shifts by 8.
 */
public class ParticleSystem extends Action
{
  public Particle[] particles;
  public int rate = 100;
  public int angle = 0; // 0 degrees is north
  public int spread = 90;
  public int gravity = (1 << 8);
  public int lifetime = 65;
  public int scatter = 0;
  public int hscatter = 0;
  public int vscatter = 0;
  public int x;
  public int y;
  public int speed = 1;
  public int size;
  public int width;
  public int height;
  public int speedVariation = 0;
  public int decay = 0;
  public int randomness = (7 << 8);
  public int color;
  public int numParticles;
  private static int[] sinTable, cosTable;

  static
  {
    sinTable = new int[360];
    cosTable = new int[360];
    for (int i = 0; i < 360; i++)
    {
      double angle = 2 * Math.PI * i / 360;
      sinTable[i] = (int) (256 * Math.sin(angle));
      cosTable[i] = (int) (256 * Math.cos(angle));
    }
  }

  public ParticleSystem(int numParticles, int x, int y, int width, int height)
  {
    this.numParticles = numParticles;
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    particles = new Particle[numParticles];
    for (int i = 0; i < numParticles; i++)
    {
      particles[i] = new Particle(this, width, height);
      newParticle(particles[i]);
    }
  }

  public double gaussian()
  {
    double sum = 0;
    for (int i = 0; i < 12; i++)
    {
      sum += Math.random();
    }
    return (sum - 6) / 3.0;
  }

  public void newParticle(Particle particle)
  {
    particle.color = color;
    particle.size = size;
    particle.lifetime = (int) (Math.random() * lifetime);
    particle.randomness = randomness;
    particle.x = x;
    particle.y = y;
    if (scatter != 0)
    {
      int a = ((int) (Math.random() * 360)) % 360;
      double distance = scatter * Math.random() / 256;
      particle.x += (int) (cosTable[a] * distance);
      particle.y += (int) (-sinTable[a] * distance);
    }
    if (hscatter != 0)
      particle.x += (int) (hscatter * (Math.random() - 0.5));
    if (vscatter != 0)
      particle.y += (int) (vscatter * (Math.random() - 0.5));
    int a = (angle + 450 - spread / 2 + (int) (Math.random() * spread)) % 360;
    int s = speed + (int) (speedVariation * gaussian());
    particle.vx = ((cosTable[a] * s) >> 8);
    particle.vy = -((sinTable[a] * s) >> 8);

    particle.x <<= 8;
    particle.y <<= 8;
  }

  public void apply(byte[] pixels, int width, int height)
  {
    for (int i = 0; i < particles.length; i++)
    {
      Particle p = particles[i];
      if (p.lifetime < 0)
      {
        newParticle(p);
      }
      p.paint(pixels, width, height);
      p.move(width, height);
      p.color -= decay;
      if (p.color < 0)
        p.color = 0;
    }
  }

  public String toString()
  {
    return "Particles";
  }
}