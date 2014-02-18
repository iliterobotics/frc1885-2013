package ilite.display.modules.display2009;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;

////////////////////////////////////////////////
//Times out. Tried to fix: Ctrl + F timeoutFix//
////////////////////////////////////////////////

public class tcpImageClient extends Thread {
	Socket mcImageSocket = null;

	DataInputStream mcImageStream = null;
	laptopGUI mcGUI = null;
	String mcIPAddr;
	int mnPort;

	public tcpImageClient(String acIPAddr, int anPort, laptopGUI aGUI) {
		System.out.println("Creating TCP Image Client object");
		mcGUI = aGUI;
		mnPort = anPort;
		mcIPAddr = new String(acIPAddr);

	}

	//if we run into problems close the connection and reconnect
	private void resetClientSocket() {
		try {
			if(mcImageSocket != null){
				mcImageSocket.close();
			}
			mcImageSocket = new Socket(InetAddress.getByName(mcIPAddr), mnPort);
			//mcImageSocket.setSoTimeout(0); timeoutFix
			mcImageStream = new DataInputStream(mcImageSocket.getInputStream());
		} catch (UnknownHostException errHost) {
			mcImageSocket = null;
			System.out.println("Error - unable to resolve host");
		} catch (IOException errIO) {
			mcImageStream = null;
			System.out.println("Error - unable create TCP IO stream");
		}

	}

	public void run() {
		System.out.println("Running TCP Image Client Thread");
		byte[] laHeader = new byte[4];
		byte[] laSize = new byte[4];
		//byte[] laImage = new byte[60000];// default
		byte[] laImageUsed;
		int lnByteCnt = 0;
		int lnImageSize = 0;
		ByteBuffer lcByteBuffer = null;
		int ImageCnt = 0;
		
		try {
			mcImageSocket = new Socket(InetAddress.getByName(mcIPAddr), mnPort);
			mcImageStream = new DataInputStream(mcImageSocket.getInputStream());
		} catch (UnknownHostException errHost) {
			mcImageSocket = null;
			System.out.println("Error - unable to resolve host");
		} catch (IOException errIO) {
			mcImageStream = null;
			System.out.println("Error - unable create TCP IO stream");
		}
		
		if (mcImageStream != null) {
			while (true) {
				try {
					// read header section
					lnByteCnt = 0;
					while (lnByteCnt < laHeader.length) {
						lnByteCnt += mcImageStream.read(laHeader, lnByteCnt,
								laHeader.length - lnByteCnt);
					}
					if(laHeader[0] != 0x01      || 
							laHeader[1] != 0x00 ||
							laHeader[2] != 0x00 ||
							laHeader[3] != 0x00)
					{
           System.out.println("Header:"+laHeader[0]+"   "+laHeader[1]+"   "+laHeader[2]+"   "+laHeader[3]);
						System.out.println("Wrong header in TCP Image stream");
						resetClientSocket();
						continue;
					}
					//

					// read size section
					lnByteCnt = 0;
					while (lnByteCnt < laSize.length) {
						lnByteCnt += mcImageStream.read(laSize, lnByteCnt,
								laSize.length - lnByteCnt);
					}
					lcByteBuffer = ByteBuffer.wrap(laSize);
					//lcByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
					lnImageSize = lcByteBuffer.getInt();
					// convert bytes in laSize to a int variable

            laImageUsed = new byte[lnImageSize];

      
          
					// check if laImage is bigger enoough to hold this image
         // if(lnImageSize > laImage.length){
            //laImage = new byte[lnImageSize];
         // }
					// if not create a bigger one
					// read Image section
					lnByteCnt = 0;
					while (lnByteCnt < lnImageSize) {
            lnByteCnt += mcImageStream.read(laImageUsed, lnByteCnt,
								lnImageSize - lnByteCnt);
					}
					//lcByteBuffer = ByteBuffer.wrap(laImage);
          System.out.println("Image size:" + lnImageSize);
					//lcByteBuffer.order(ByteOrder.LITTLE_ENDIAN);
					if(mcGUI != null){

            mcGUI.newJpegImage(laImageUsed);

					}
					// pass data to laptopGUI
				} catch (IOException err) {
					resetClientSocket();

				}
			}
		}
	}

}
