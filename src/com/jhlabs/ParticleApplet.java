/*
 * Copyright Jerry Huxtable 1999
 *
 * Feel free to do anything you like with this code.
 */

package com.jhlabs;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.ColorModel;
import java.awt.image.IndexColorModel;
import java.awt.image.MemoryImageSource;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.JFrame;
import javax.swing.JPanel;

import org.ilite.utils.gui.GuiFactory;

/**
 * An applet which animates a simple particle system.
 */
public class ParticleApplet extends /* Applet */JPanel implements MouseListener
{
  public static void main(String[] pArgs)
  {
    ParticleApplet pa = new ParticleApplet();
    JFrame aFrame = GuiFactory.createanddisplayDefaultJFrame("PARTICLES", pa);
  }

  protected MemoryImageSource source;
  protected Image image;
  protected boolean newImage = true;
  protected ColorModel colorModel;
  protected Thread thread;
  protected byte[] pixels1, pixels2;
  protected int width = 800, height = 600;
  protected Image offscreen;
  protected Graphics offscreenG;
  public Action[] mActions;
  protected Filter[] mFilters;
  // protected Gradient[] gradients;
  private boolean running = false;
  private boolean startAnimation = false;

  public ParticleApplet()
  {
    init();
  }

  public void init()
  {
    setPreferredSize(new Dimension(width, height));
    addMouseListener(this);
    int numFilters = 8;
    mFilters = new Filter[numFilters];
    mFilters[0] = new ShiftDownFilter();
    mFilters[1] = new ShiftUpFilter();
    mFilters[2] = new ConvolveFilter();
    mFilters[3] = new BlurHFilter();
    mFilters[4] = new BlurVFilter();
    mFilters[5] = new ThingFilter();
    mFilters[6] = new WaterFilter();
    mFilters[7] = new ClearFilter();
    mFilters[3].setEnabled(true);
    mFilters[4].setEnabled(true);

    int numActions = 1;
    mActions = new Action[numActions];
    int numParticles = 100;// getIntParameter("numParticles", 100);
    ParticleSystem sys = new ParticleSystem(numParticles, width / 2, height / 2,
        width, height);
    mActions[0] = sys;
    mActions[0].setEnabled(true);
    sys.rate = 100;// getIntParameter("rate", 100);
    sys.speed = 1;// (getIntParameter("speed", 0) << 8) / 10;
    sys.angle = 0;// getIntParameter("angle", 0);
    sys.spread = 360;// getIntParameter("spread", 360);
    sys.gravity = 0;// getIntParameter("gravity", 0);
    sys.color = 255;// getIntParameter("color", 255);
    sys.scatter = 0;// getIntParameter("scatter", 0);
    sys.hscatter = 0;// getIntParameter("hscatter", 0);
    sys.vscatter = 0;// getIntParameter("vscatter", 0);
    sys.randomness = 0;// getIntParameter("randomness", 0);
    sys.size = 2;// getIntParameter("size", 2);
    sys.x = width / 2;// getIntParameter("x", width / 2);
    sys.y = height / 2;// getIntParameter("y", height / 2);
    sys.lifetime = 50;// getIntParameter("lifetime", 50);
    sys.speedVariation = 1;// (getIntParameter("speedVariation", 0) << 8)
    // / 10;
    sys.decay = 0;// getIntParameter("decay", 0);
    startAnimation = true;// getIntParameter("startAnimation", 1) != 0;
    int colormap = 4;// getIntParameter("colormap", 4);
    setGradient(colormap);
    // String actions = getParameter("actions");
    // if (actions != null)
    // {
    // String s = "duchvtwc";
    // for (int i = 0; i < s.length(); i++)
    // filters[i].setEnabled(actions.indexOf(s.charAt(i)) != -1);
    // }
  }

  //
  // private int getIntParameter(String name, int defaultValue)
  // {
  // if (name != null)
  // {
  // try
  // {
  // return Integer.parseInt(getParameter(name));
  // } catch (NumberFormatException e)
  // {
  // }
  // }
  // return defaultValue;
  // }

  public void setGradient(int n)
  {
    // n = Math.max(0, Math.min(n, gradients.length - 1));
    byte[] r = new byte[256];
    byte[] g = new byte[256];
    byte[] b = new byte[256];
    for (int i = 0; i < 256; i++)
    {
      // int rgb = gradients[n].getColor(i/255.0);
      int rgb = ColorHelper.numberToColor(i / 255.0).getRGB();
      r[i] = (byte) ((rgb >> 16) & 0xff);
      g[i] = (byte) ((rgb >> 8) & 0xff);
      b[i] = (byte) (rgb & 0xff);
    }
    colorModel = new IndexColorModel(8, 256, r, g, b);
  }

  public void start()
  {
    new Timer().scheduleAtFixedRate(new Animator(), 1000/45, 1000/45);
  }

  public void stop()
  {
    if (thread != null)
    {
      thread.stop();
      thread = null;
    }
  }

  public void update(Graphics g)
  {
    paint(g);
  }

  public synchronized void paint(Graphics g)
  {
    Dimension size = getSize();
    int w = size.width;
    int h = size.height;
    if (newImage || image == null)
      image = makeImage(w, h);
    g.drawImage(image, 0, 0, this);
    if (!running)
    {
      String s = "Click to Start";
      FontMetrics fm = g.getFontMetrics();
      int x = (size.width - fm.stringWidth(s)) / 2;
      int y = (size.height - fm.getAscent()) / 2;
      g.setColor(Color.white);
      g.drawString(s, x, y);
    }
    notify();
  }

  private Image makeImage(int w, int h)
  {
    if (pixels1 == null)
    {
      int i = 0;
      pixels1 = new byte[width * height];
      pixels2 = new byte[width * height];
      for (i = 0; i < mActions.length; i++)
        if (mActions[i].isEnabled())
          mActions[i].apply(pixels1, width, height);
    } else
    {
      for (int i = 0; i < mFilters.length; i++)
      {
        if (mFilters[i].isEnabled())
        {
          mFilters[i].apply(pixels1, pixels2, width, height);
          byte[] t = pixels1;
          pixels1 = pixels2;
          pixels2 = t;
        }
      }
      for (int i = 0; i < mActions.length; i++)
        if (mActions[i].isEnabled())
          mActions[i].apply(pixels1, width, height);
    }
    newImage = false;
    if (image == null)
    {
      image = createImage(source = new MemoryImageSource(w, h, colorModel,
          pixels1, 0, w));
      source.setAnimated(true);
    } 
    else
    {
      source.newPixels(pixels1, colorModel, 0, w);
    }
    return image;
  }

  private void repaintImage()
  {
    newImage = true;
    repaint();
  }

  public void mousePressed(MouseEvent e)
  {
    requestFocus();
    startAnimation = true;
    if (thread == null && !running)
      start();
    else
      running = false;
  }

  public void mouseReleased(MouseEvent e)
  {
  }

  public void mouseClicked(MouseEvent e)
  {
  }

  public void mouseEntered(MouseEvent e)
  {
  }

  public void mouseExited(MouseEvent e)
  {
  }

  private class Animator extends TimerTask
  {
    @Override
    public void run()
    {
      repaintImage();
    }    
  }
}

class Thing
{
  private boolean enabled;

  public void setEnabled(boolean enabled)
  {
    this.enabled = enabled;
  }

  public boolean isEnabled()
  {
    return enabled;
  }
}

class Action extends Thing
{
  public void apply(byte[] pixels, int width, int height)
  {
  }
}

abstract class Filter extends Action
{
  public void apply(byte[] in, byte[] out, int width, int height)
  {
  }
}

/*
 * class TextAction extends Action { private Font font = new Font("sansserif",
 * Font.PLAIN, 72);
 * 
 * public void apply(byte[] pixels, int width, int height) { } }
 * 
 * class ImageAction extends Action { private Image image;
 * 
 * public ImageAction(Image image) { this.image = image; }
 * 
 * public void apply(byte[] pixels, int width, int height) { } }
 */
