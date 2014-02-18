package ilite.data.driver;

public interface IRemoteDataDriver extends IDataDriver
{
  public void startServer();
  
  public void stopServer();
  
  public void simulateDisconnect();
}
