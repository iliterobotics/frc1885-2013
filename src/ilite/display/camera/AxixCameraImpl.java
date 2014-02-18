package ilite.display.camera;

public class AxixCameraImpl
{
  private static final String endl = "\r\n";
  private int mFrameRate = 30;
  private int mCompression = 60;
  private int mWidth = 640;
  private int mHeight = 480;
  private int mRotation = 0;
  private int mColor = 1;
  private int mColorLevel = 50;
  private String mAgent = "FRC";
  private String mAccept = "*/*";
  private String mAuth = "lJDOkZSQw=="; // Username 'FRC', password 'FRC'.

  private static String sCONNECT_URI = "GET /axis-cgi/mjpg/video.cgi?resolution=320x240 HTTP/1.1\r\n" +
  "User-Agent: testprog/1.1\r\n" +
  "Accept: */*\r\n" +
  "Host: 10.5.37.11\r\n" +
  "Connection: Keep-Alive\r\n\r\n";
//  requestString = "GET /axis-cgi/mjpg/video.cgi"
//      "?des_fps=" Q(CAMERA_FRAMES_PER_SECOND)
//      "&compression=" Q(CAMERA_COMPRESSION)
//      "&resolution=" Q(CAMERA_WIDTH) "x" Q(CAMERA_HEIGHT)
//      "&rotation=" Q(CAMERA_ROTATION)
//      "&color=1"
//      "&colorlevel=" Q(CAMERA_COLOR_LEVEL)
//      " HTTP/1.1\r\n"
//      "User-Agent: DriverVision\r\n"
//      "Accept: */*\r\n"
//      "Connection: Keep-alive\r\n"
//      "DNT: 1\r\n"
//      "Authorization: Basic " CAMERA_AUTHENTICATION "\r\n\r\n";
  
  public String getConnectionString()
  {
    StringBuilder sb = new StringBuilder();
    sb.append("/axis-cgi/mjpg/video.cgi");
    sb.append("?des_fps=").append(mFrameRate);
    sb.append("&compression=").append(mCompression);
    sb.append("&resolution").append(mWidth).append("x").append(mHeight);
    sb.append("&rotation=").append(mRotation);
    sb.append("&color=").append(mColor);
    sb.append("&colorlevel=").append(mColorLevel);
    sb.append(" HTTP/1.1").append(endl);
    sb.append("User-Agent: DriverVision").append(endl);
    sb.append("Accept: ").append(mAccept).append(endl);
    sb.append("Connection: Keep-alive").append(endl);
    sb.append("DNT: 1").append(endl);
//    sb.append("Authorization: Basic ").append(mAuth).append(endl);
    sb.append(endl);
    return sb.toString();
  }
}
