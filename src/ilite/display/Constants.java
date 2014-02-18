package ilite.display;

import ilite.display.config.ISystemConfiguration;

import java.awt.Dimension;
import java.awt.Toolkit;

public class Constants {
  public static String sCONFIG_DIR = "";
  public static String sCONFIG_FILE = "config.xml";
  
	public static final String sSMALL_FONT_SIZE_KEY = "ui.textsize.small";
	public static final String sLARGE_FONT_SIZE_KEY = "ui.textsize.large";
	public static final String sIMAGE_FORMAT_KEY = "system.output.image.format";
	public static final String sDATA_OUTPUT_DIR_KEY = "system.output.dir";
	public static final String sIMAGE_WIDTH_KEY = "robot.camera.width";
	public static final String sIMAGE_HEIGHT_KEY = "robot.camera.height";
	public static final String sORGANIZATION_KEY = "system.organization";
	public static final String sROBOT_CRIO_IP = "robot.crio.ip";
	public static final String sROBOT_CRIO_PORT = "robot.crio.port";
	public static final String sCAMERA_IP = "robot.camera.ip";
	public static final String sCAMERA_PORT = "robot.camera.port";
	
	private static Dimension sSCREEN_SIZE = null;
	private static Dimension sIMAGE_SIZE = null;
	
	private static ISystemConfiguration sCONFIG = null;
	
	/*package*/ static void setConfig(ISystemConfiguration pConfig)
	{
	  sCONFIG = pConfig;
	}

	public static Dimension getScreenSize()
	{
		if(sSCREEN_SIZE == null)
		{
			sSCREEN_SIZE = Toolkit.getDefaultToolkit().getScreenSize();
		}
		return sSCREEN_SIZE;
	}
	
	public static Dimension getImageSize()
	{
		if(sIMAGE_SIZE == null)
		{
			sIMAGE_SIZE = new Dimension(
					sCONFIG.getInt(sIMAGE_WIDTH_KEY),
					sCONFIG.getInt(sIMAGE_HEIGHT_KEY));
		}
		return sIMAGE_SIZE;
	}
	

	public static float getSmallFontSize() 
	{
		return sCONFIG.getFloat(sSMALL_FONT_SIZE_KEY);
	}

	public static float getLargeFontSize()
	{
		return sCONFIG.getFloat(sLARGE_FONT_SIZE_KEY);
	}
	
	public static String getDataDirectory()
	{
	  return sCONFIG.getString(sDATA_OUTPUT_DIR_KEY);
	}
	
	public static String getImageFormat()
	{
	  return sCONFIG.getString(sIMAGE_FORMAT_KEY);
	}
}
