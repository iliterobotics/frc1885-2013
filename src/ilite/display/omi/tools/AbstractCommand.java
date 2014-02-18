package ilite.display.omi.tools;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.Action;
import javax.swing.ImageIcon;

public abstract class AbstractCommand implements Action
{
  private Set<PropertyChangeListener> mPropertyListeners = new HashSet<PropertyChangeListener>();
  private Map<String, Object> mProperties = new HashMap<String, Object>();
  private boolean mIsEnabled = true;
  
  public abstract String getName();
  
  public ImageIcon getIcon()
  {
    return null;
  }

  @Override
  public Object getValue(String key)
  {
    return mProperties.get(key);
  }

  @Override
  public void putValue(String key, Object value)
  {
    Object old = mProperties.get(key);
    mProperties.put(key, value);
    for(PropertyChangeListener l : mPropertyListeners)
    {
      l.propertyChange(new PropertyChangeEvent(this, key, old, value));
    }
  }

  @Override
  public void setEnabled(boolean b)
  {
    mIsEnabled = b;
  }

  @Override
  public boolean isEnabled()
  {
    return mIsEnabled;
  }

  @Override
  public void addPropertyChangeListener(PropertyChangeListener listener)
  {
    mPropertyListeners.add(listener);
  }

  @Override
  public void removePropertyChangeListener(PropertyChangeListener listener)
  {
    mPropertyListeners.remove(listener);
  }

}
