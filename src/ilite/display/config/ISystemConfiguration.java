package ilite.display.config;

import ilite.util.lang.IUpdate;

public interface ISystemConfiguration {

	public String getString(String pKey);
	
	public Integer getInt(String pKey);
	
	public Double getDouble(String pKey);
	
	public Float getFloat(String pKey);
	
	public Boolean getBool(String pKey);
	
	public void addPropertyChangeListener(String pProperty, IUpdate<String> pListener);
	
	public boolean setConfigItem(String pKey, String pValue);
	
	void writeToFile();
	
}
