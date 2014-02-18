package ilite.data.driver;

public interface IDataDriver <Data>
{
  public void start();
  
  public void cancel();

  public Data generate(float pRelativeTimeSeconds);
}
