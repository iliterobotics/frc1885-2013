package ilite.display.modules.display2014;

import ilite.display.modules.display2014.data.Data2014;
import ilite.display.modules.display2014.data.EData2014;
import ilite.util.gui.RepaintClosure;
import ilite.util.lang.IProvider;
import ilite.util.lang.IUpdate;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;
import ilite.util.math.RunningAverage;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JSlider;
import javax.swing.JTable;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.TableModelListener;
import javax.swing.table.TableModel;

public class TelemetryTablePanel
implements TableModel, IUpdate<Data2014>
{
  private JTable mDataTable;
  private JPanel mDisplayPanel;
  private Data2014 mLatestData;
  private final RepaintClosure mRepainter;
  private EDataMode mCurrentMode;
  private ILog mLog = Logger.createLog(TelemetryTablePanel.class);
  private Map<EData2014, RunningAverage<Float>> mRunningAverages = new HashMap<>();
  
  public TelemetryTablePanel()
  {
    final JSlider runningAvgSlider = new JSlider(5, 50, 10);
    runningAvgSlider.setMajorTickSpacing(5);
    runningAvgSlider.setMinorTickSpacing(5);
    runningAvgSlider.setPaintLabels(true);
    runningAvgSlider.setPaintTicks(true);
    runningAvgSlider.addChangeListener(new ChangeListener()
    {
      @Override
      public void stateChanged(ChangeEvent e)
      {
        for(RunningAverage<Float> avg : mRunningAverages.values())
        {
          avg.setRunningAverage(runningAvgSlider.getValue());
        }
      }
    });
    JRadioButton fullDataButton = new JRadioButton("All Telemetry   ");
    JRadioButton usedDataButton = new JRadioButton("Used Telemetry Only");
    fullDataButton.setSelected(true);
    ButtonGroup bg = new ButtonGroup();
    bg.add(fullDataButton);
    bg.add(usedDataButton);
    fullDataButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        setDataMode(EDataMode.FULL);
      }
    });
    usedDataButton.addActionListener(new ActionListener()
    {
      @Override
      public void actionPerformed(ActionEvent e)
      {
        setDataMode(EDataMode.USED);
      }
    });
    mDataTable = new JTable(this);
    mDisplayPanel = new JPanel(new BorderLayout());
    JScrollPane jsp = new JScrollPane(mDataTable);
    jsp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
    jsp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
    mDisplayPanel.add(jsp, BorderLayout.CENTER);
    
    JPanel optionPanel = new JPanel();
    BoxLayout bl = new BoxLayout(optionPanel, BoxLayout.X_AXIS);
    optionPanel.setBorder(BorderFactory.createTitledBorder("Telemetry Table Options"));
    optionPanel.setLayout(bl);
    optionPanel.add(fullDataButton);
    optionPanel.add(usedDataButton);
    optionPanel.add(new JLabel("       Running Average: "));
    optionPanel.add(runningAvgSlider);
    mDisplayPanel.add(optionPanel, BorderLayout.NORTH);
    
    IProvider<Data2014> provider = DataManager2014.getInstance().getTelemetryProvider();
    provider.addListener(this);
    mLatestData = provider.getLatest();
    mRepainter = new RepaintClosure(mDisplayPanel, false);
    setDataMode(EDataMode.FULL);
    
    mDataTable.setAutoResizeMode(JTable.AUTO_RESIZE_ALL_COLUMNS);
    
    for(EData2014 data : Data2014.scUsedFields.values())
    {
      if(data.getFieldClass().equals(Float.class))
      {
        RunningAverage<Float> avg = new RunningAverage<>();
        avg.setRunningAverage(10);
        mRunningAverages.put(data, avg);        
      }
    }
  }
  
  public JPanel getDisplayPanel()
  {
    return mDisplayPanel;
  }
  
  private void setDataMode(EDataMode pMode)
  {
    mCurrentMode = pMode;
    mRepainter.repaint();
  }

  @Override
  public void update(Data2014 pObject)
  {
    for(EData2014 field : Data2014.scUsedFields.values())
    {
      if(field.getFieldClass().equals(Float.class))
      {
        RunningAverage<Float> avg = mRunningAverages.get(field);
        avg.update((Float)pObject.get(field));
      }
    }
    mLog.debug(pObject.toCsvString());
    mLatestData = pObject;
    mRepainter.repaint();
  }
  
  private enum EDataMode
  {
    FULL(EData2014.values().length), 
    USED(Data2014.scUsedFields.size());
    
    private int mLength;
    
    private EDataMode(int pNumFields)
    {
      mLength = pNumFields;
    }
  }

  // ******************************************************
  // Table Model Section
  // ******************************************************
  @Override
  public int getRowCount()
  {
    return mCurrentMode.mLength;
  }
  
  private static final String[] sCOLS = new String[]
  {
    "Type", "Port #", "Data", "Value", "Avg Value"
  };

  @Override
  public int getColumnCount()
  {
    return sCOLS.length;
  }

  @Override
  public String getColumnName(int columnIndex)
  {
    return sCOLS[columnIndex];
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
    String result = "---";
    EData2014 field;
    if(mCurrentMode == EDataMode.FULL)
    {
      field = EData2014.values()[rowIndex];
    }
    else
    {
      field = Data2014.scUsedFields.get(rowIndex);
    }
    switch(columnIndex)
    {
    case 0:
      result = field.getTelemetryType().name();
      break;
    case 1:
      result = Integer.toString(field.getPortNumber());
      break;
    case 2:
      result = field.getDisplayLabel();
      break;
    case 3:
      Object o = mLatestData.get(field);
      if(o != null)
      {
        result = o.toString();
      }
      break;
    case 4:
      RunningAverage<Float> avg = mRunningAverages.get(field);
      if(avg != null)
      {
        result = Double.toString(avg.getRunningAverage());
      }
    }
    return result;
  }

  @Override
  public void setValueAt(Object aValue, int rowIndex, int columnIndex)
  {
    // Read-Only table
  }
  
  private Set<TableModelListener> mTableModelListeners = new HashSet<>();

  @Override
  public void addTableModelListener(TableModelListener l)
  {
    mTableModelListeners.add(l);
  }

  @Override
  public void removeTableModelListener(TableModelListener l)
  {
    mTableModelListeners.remove(l);
  }
  
}
