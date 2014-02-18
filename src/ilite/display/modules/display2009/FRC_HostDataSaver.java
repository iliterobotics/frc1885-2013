package ilite.display.modules.display2009;

import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Vector;
import java.util.concurrent.Semaphore;

public class FRC_HostDataSaver implements Runnable {
	
	//Private varible to indicate to run() function if thread should terminate
	private boolean mbThreadSTOPRunning = false;
	
	//object to access file for saving data
	private FileOutputStream mcFileOut = null;
	//output stream object - provides nice interface for saving data
	private DataOutputStream mcDataOut = null;
	
	//vector list to allow to have a large list (if needed) of data sets to save 
	private Vector<byte[]> mcSaveDataList = null; 
	
	//counting semaphore to informed run() method that 
	private Semaphore mcAvailableDataSem = null;
	//semaphore to make this class thread save - use to protect mSaveDataList
	private Semaphore mcProtectSaveDataMutex = null;
	

	//Thread class object to allow this class to be multithreaded
	private Thread mcThread = null;
	
	/**
	 * This function implements the functionality that allows a  
	 * FRC_HostDataSaver class instance to automaticly save a byte[]
	 * to file for reterival later by using a non-blocking call.  This 
	 * is a public function (due to inheritance); it is not meant to be called
	 * externally. 
	 * @see java.lang.Runnable#run()
	 * @param	NA
	 * @return	void
	 */
	public void run(){
		byte[] lCurrentDataSetToSave = null;
		while(!mbThreadSTOPRunning){
			try{
//				//Wait until we are told that there is something to do
				mcAvailableDataSem.acquire();
				lCurrentDataSetToSave = null;
				
				//remove element from vector- but first make sure the vector is thread safe
				mcProtectSaveDataMutex.acquire();
				if(mcSaveDataList.size() > 0){
					lCurrentDataSetToSave = mcSaveDataList.remove(0);
				}
				//release semaphore around vector
				mcProtectSaveDataMutex.release();
				
				//save data if we have any
				if(lCurrentDataSetToSave != null){
					mcDataOut.writeInt(lCurrentDataSetToSave.length);
					mcDataOut.write(lCurrentDataSetToSave,0,lCurrentDataSetToSave.length);
				}
			}
			catch(InterruptedException E){
				System.out.println("in FRC_HostDataSaver::run() - Int. E::" + E.getMessage());	
			}
			catch(IOException E){
				System.out.println("in FRC_HostDataSaver::run() -IO E::" + E.getMessage());
			}
		}
		
		
		try{
			//make sure that data is save to file
			mcDataOut.flush();
			//close out file
			mcDataOut.close();
			
			mcFileOut.close();
			
			System.out.println("File closed in run()");
			
		}
		catch(IOException E){
			System.out.println("in FRC_HostDataSaver::run() -(end) IO E::" + E.getMessage());
		}
		
	}

	/**
	 * This function initializes the FRC_HostDataSaver class instance.
	 * A FRC_HostDataSaver is not usable until init() is called
	 * @param	aFileName	Name of file to save data to
	 * @return	void
	 */
	//Name:	init
	//
	public void init(String aFileName){
		try{
			//create objects to write data to file
			mcFileOut = new FileOutputStream(aFileName, false);
			mcDataOut = new DataOutputStream(mcFileOut);
			
			//start the thread to write data to file if external caller
			//uses the newDatatoSave() function
			if(mcThread != null){
				mcThread.start();
			}
			
		}
		catch(FileNotFoundException a){
			mcFileOut = null;
			System.out.println("File wasn't found");
		}
	}
	
	/**
	 * Class constructor
	 * @param	NA
	 * 
	 */
	public FRC_HostDataSaver(){
		//create a vector to store byte[] for saving in a multithreaded enviroment
		mcSaveDataList = new Vector<byte[]>();
		
		//Create a counting semaphore for class instance to communicate among 
		//different functions that actions need to be taken
		mcAvailableDataSem = new Semaphore(0,true);
		
		//a semaphore init to 1 equivelent to a MUTEX
		//This mutex protects the vector object in a multithreaded env.
		mcProtectSaveDataMutex = new Semaphore(1,true);
		
		mcThread = new Thread(this);
	}
	 
	/**
	 * @param aData
	 *            array of byte data to be saved - this data can not be altered
	 *            once it has been passed to this function
	 */
	public void newDatatoSave(byte[] aData) {
		// add to vector
		try {
			System.out.println("New Data called by external owner with size = "+
				aData.length); 
			//aquire semaphore to protect the vector object
			mcProtectSaveDataMutex.acquire();
			mcSaveDataList.add(aData);
			//release semaphore now that we are done altering object
			mcProtectSaveDataMutex.release();
			
			// inform run() function that there is data to be saved to a file
			mcAvailableDataSem.release();
		} catch (InterruptedException E) {
			// do not thing for interrupt exception from
			// mProtectSaveDataMutex.acquire();
		}
	}
	
	/**
	 * This function allows the owner of a FRC_HostDataSaver instance to
	 * tell the instance to terminate.  Once STOP() is called, this class
	 * is no longer usable.
	 */
	//Once this function is called this class is no longer usable
	public void STOP(){
		System.out.println("STOp called by external owner");
		//indicate to run() function that it is time to exit
		mbThreadSTOPRunning = true;
		
		//Use the semaphore object to tell the run() there is a message that it
		//needs to operate on.  With mbThreadSTOPRunning = true; earlier in this
		//function run() will use mbThreadSTOPRunning flag to terminate self.
		mcAvailableDataSem.release();
	}
	
	public boolean isAlive(){
		return mcThread.isAlive();
	}

}
