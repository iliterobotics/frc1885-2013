package ilite.display.modules.display2009;

//import laptopGUI;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Arrays;

public class datagramStuff extends Thread{

   private laptopGUI laptopTest;
 
    
   public datagramStuff(laptopGUI laptop){
      laptopTest = laptop;
      System.out.println("UDP datagram object FRD DSH Board created");
   }
   
   
   public void run() {
	   System.out.println("UDP datagram Thread running");
		DatagramSocket ds = null;
		try {
			ds = new DatagramSocket(1165);
		} catch (IOException lexception) {
			System.out.println("unable to open UDP port 1165 :"
					+ lexception.getMessage());
			return;// terminating function
		}
		byte[] receivedBuffer = new byte[65507];
		DatagramPacket incoming = new DatagramPacket(receivedBuffer,
				receivedBuffer.length);
		while (true) {
			try {
				ds.receive(incoming); // DatagramSocket
				byte[] result = Arrays.copyOf(incoming.getData(), incoming
						.getLength());
				if(laptopTest != null){
					laptopTest.newUDPData(result);
				}
				else{
				
				}
			} catch (IOException lexception) {
				System.out.println("Problem on receiving UDP Packet :"
						+ lexception.getMessage());
			}
		}
	}
   
   
}