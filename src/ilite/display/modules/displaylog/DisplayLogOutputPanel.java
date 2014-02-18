package ilite.display.modules.displaylog;

import ilite.util.logging.LogOutput;

import java.util.LinkedList;

import javax.swing.JTable;
import javax.swing.event.TableModelListener;
import javax.swing.table.TableModel;

public class DisplayLogOutputPanel
{
  private enum LogColumns
  {
    TIME {
      @Override
      public String getValueOf(LogOutput pOutput)
      {
        return DisplayLogModuleModel.mDateFormat.format(pOutput.time);
      }
    },
    LEVEL {
      @Override
      public String getValueOf(LogOutput pOutput)
      {
        return pOutput.level.name();
      }
    },
    THREAD {
      @Override
      public String getValueOf(LogOutput pOutput)
      {
        return pOutput.thread;
      }
    },
    METHOD {
      @Override
      public String getValueOf(LogOutput pOutput)
      {
        return pOutput.method;
      }
    },
    LOG {
      @Override
      public String getValueOf(LogOutput pOutput)
      {
        return pOutput.text;
      }
    };
    
    public abstract String getValueOf(LogOutput pOutput);
  }
  private JTable mList = new JTable();
  private LinkedList<LogOutput> mOutput = new LinkedList<LogOutput>();
  private int mHistoryLength = 1000;
  
  public DisplayLogOutputPanel()
  {
    mList.setModel(new LogModel());
    
    
  }
  
  public void append(LogOutput pOutput)
  {
    mOutput.add(pOutput);
    if(mOutput.size() > mHistoryLength)
    {
      mOutput.removeFirst();
    }
  }
  
  private class LogModel implements TableModel
  {

    @Override
    public int getRowCount()
    {
      return mOutput.size();
    }

    @Override
    public int getColumnCount()
    {
      return LogColumns.values().length;
    }

    @Override
    public String getColumnName(int columnIndex)
    {
      return LogColumns.values()[columnIndex].name();
    }

    @Override
    public Class<?> getColumnClass(int columnIndex)
    {
      return String.class;
    }

    @Override
    public boolean isCellEditable(int rowIndex, int columnIndex)
    {
      return false;
    }

    @Override
    public Object getValueAt(int rowIndex, int columnIndex)
    {
      return LogColumns.values()[columnIndex].getValueOf(mOutput.get(rowIndex));
    }

    @Override public void setValueAt(Object aValue, int rowIndex, int columnIndex){}
    @Override public void addTableModelListener(TableModelListener l){}
    @Override public void removeTableModelListener(TableModelListener l){}
    
  }
}
