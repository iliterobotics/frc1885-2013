package ilite.display.modules.display2009;

import java.io.IOException;
import java.nio.ByteBuffer;

public class DataSet_Timer implements Runnable {
	
	//Class variable declarations
	private boolean mbRunning;
	private int mnDefaultWaitTime;
	private DataRead mcRead;
	private DataPrint mcPrint;
	private ByteBuffer mcBuffer;
	
	public DataSet_Timer(ByteBuffer acBuffer) {
		mbRunning = true;
		mnDefaultWaitTime = 20;
		mcPrint = new DataPrint();
		mcRead = new DataRead();
		mcBuffer = acBuffer;
		this.run();
	}
	
	//Runs the program.
	public void run() {
		byte[] fileOutput = null;
		while(true) {
			if(mbRunning) {
			    //Writes to file
		        mcPrint.printData(mcBuffer);
		        this.tWait(); //Waits 20 MS (.02 sec)
		        //Reads from file
		        try{
              fileOutput = mcRead.getBytesFromFile(mcPrint.getFile());
		        }
            catch(IOException e)
            {
              System.out.println(e);
            }
		        System.out.println(fileOutput);
			}
		}
	}
	
	//Makes the thread wait aMS (Milliseconfgds)
	public void tWait(int aMS) {
		long startMS = System.currentTimeMillis();
		while(System.currentTimeMillis()<startMS+aMS) {} //Do nada. :D
	}
	
	//No parameters wait. Makes the program wait for mnDefaultWaitTime ms.
	public void tWait() {
		tWait(mnDefaultWaitTime);
	}
	
	//sets default wait time
	public void set(int aWait) {
		mnDefaultWaitTime = aWait;
	}
	
	//Skips noToJump lines in the document
	public void jump(int noToJump) 
	{
		for(int i=0;i<noToJump;i++)
      try{
        mcRead.getBytesFromFile(mcPrint.getFile());
      }
      catch(IOException e)
      {
        System.out.println(e);
      }
	}
	
	//Pauses the program.
	public void pause() {
		mbRunning = false;
	}
	
	public void go() {
		mbRunning = true;
	}

	
}
