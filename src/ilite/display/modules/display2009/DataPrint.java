package ilite.display.modules.display2009;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;

public class DataPrint
{
  FileOutputStream fStream;
  File file;
  
  public DataPrint()
  {
    try{
      file = new File("DataOut.txt");
      fStream = new FileOutputStream(file, true);
    }
    
    catch (Exception e){
      System.err.println("First block Error: " + e.getMessage());
    }
  }
  public void printData(ByteBuffer something)
  {    
    try{
    // Create file     
    fStream.write(something.array());
    }
    catch (Exception e){//Catch exception if any
      System.err.println("Second block Error: " + e.getMessage());
      e.printStackTrace();
      System.exit(1);
    }
  }
  
  public void close()
  {
    try{
    fStream.close();
    }
     catch (Exception e){
      System.err.println("Third block Error: " + e.getMessage());
    }
    
  }
  
  public File getFile()
  {
    if(file==null)
      System.out.println("The file is null. >.>");
    return file;
  }
}