package ilite.display.modules.display2013;

import ilite.display.modules.display2013.data.RobotData2013;
import ilite.util.lang.INotify;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class RobotDataFileWriter implements INotify
{

	private String mFilePath = "matchdata\\";
	private final CompetitionDisplay2013Model mModel;
	private final BufferedWriter mWriter;

	public RobotDataFileWriter(CompetitionDisplay2013Model pModel, String pPath) throws IOException
	{

		if(pPath != null)
		{
			mFilePath = pPath;
		}
		mModel = pModel;
		String header = RobotData2013.toCSVStringHeader();
		File dir = new File(mFilePath);
		if(!dir.exists())
		{
			dir.mkdirs();
		}
		
		File f = new File("output" + System.currentTimeMillis() + ".csv");
		if(!f.exists())				f.createNewFile();
		mWriter = new BufferedWriter(new FileWriter(f));
		mWriter.write(header);
		mWriter.newLine();
	}

	public void notifyOfUpdate()
	{
		
		RobotData2013 data = mModel.getLatestRobotData();

		String s = data.toCSVString();
		try {
			mWriter.write(s);
			mWriter.newLine();
		} catch (IOException e) {
		}
		
		System.out.println(s);

	}

	public void closeFile() {
		if(mWriter != null)
		{
			try {
				mWriter.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}
