package ilite.display.interfaces.net;

import ilite.data.models.RobotDebugMessage;
import ilite.util.logging.ILog;
import ilite.util.logging.Logger;

public class StringMsgDecoder extends AbstractMagicNumberMsgDecoder<RobotDebugMessage>
{
  private ILog mLog = Logger.createLog(StringMsgDecoder.class);
  public StringMsgDecoder(int pMagicNumber)
  {
    super(pMagicNumber);
  }

  @Override
  protected RobotDebugMessage decodeImpl(byte[] pData, int pLength)
  {
    String result = new String(pData);
    result = result.substring(0, result.indexOf('\0'));
    mLog.debug(result);
    //TODO timestamp
    return new RobotDebugMessage(-1, result);
  }
}
