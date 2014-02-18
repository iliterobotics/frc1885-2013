package ilite.display.modules;

import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;

import org.ilite.comm.ChunkReader;
import org.ilite.comm.ChunkReaderException;
import org.ilite.comm.ChunkTransmission;

public class ConnectionTestDriver {
	Socket mClientSocket;
	ServerSocket mServerSocket;
	public Semaphore mcClientConnected;
	private int currentChunkSize;
	private ChunkReader currentChunk;
	private final Executor mParsingThreadPool = Executors.newFixedThreadPool(1);

	private boolean mConnectionStatus;

	public boolean isConnected(){ return mConnectionStatus; }

	public static void main(String[] args) {
		new ConnectionTestDriver().attemptRobotConnection();
	}

	void reportConnection() {
		try {
			mConnectionStatus = mClientSocket.isConnected();
			System.out.println("Connection: " + mConnectionStatus);
		} catch (NullPointerException e) {
			mConnectionStatus = false;
		}
	}

	public void attemptRobotConnection()
	{
		Thread t = new Thread("ROBOT CONNECTION THREAD")
		{
			@Override public void run()
			{
				String ip = "10.18.85.2";
				int port = 1180;
				establishConnection(ip, port);

				reportConnection();

				if(isConnected()) new Thread(new Runnable() {
					public void run() {
						// TODO Auto-generated method stub
						while(true) {
							System.out.println("Attempting to find Input Stream");
							InputStream lnDataFromServer = null;
							try {
								lnDataFromServer = mClientSocket.getInputStream();
								System.out.println("Input Stream Found");
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}//create reader to read data from client

							try {
								parse(lnDataFromServer);
							} catch (Exception e) {
								break;
							}
							reportConnection();
						}
						mcClientConnected.release();
						disconnect();
					}
				}).start();
			}
		};
		t.start();
	}
	
	public void disconnect()
	{
		try {
			mClientSocket.close();
		} catch (IOException e) {
		}
	}

	void establishConnection(String pIpAddress, int pPort) {
		try {
			mServerSocket = new ServerSocket(pPort, 2, null);
			System.out.println("Server Attempt Accept");
			mClientSocket = mServerSocket.accept();
			System.out.println(mClientSocket);
			System.out.println("Server Accept");

			//mcClientSocket = new Socket(InetAddress.getByName(pIpAddress), pPort);//create socket to connect to server
			mcClientConnected = new Semaphore(0,true);//create "mutex" semaphore so user knows when Client is done
		} catch (UnknownHostException e) {
			System.out.println("Error Connecting to " + pIpAddress + ":" + pPort + " UHE");
			return;
		} catch (IOException e) {
			System.out.println("Error Connecting to " + pIpAddress + ":" + pPort + " IOE");
			return;
		}
		System.out.println("Client Established");
	}

	public void parse(InputStream is) throws Exception {
		currentChunk = ChunkTransmission.read(is);
		currentChunkSize = currentChunk.chunkSize();
		System.out.println(currentChunkSize);
		processRobotChunk();
	}

	private double[] getDoublesFromChunk(int size) throws ChunkReaderException {
		double[] d = new double[size];

		for( int i = 0; i < size; i++) {
			d[i] = currentChunk.nextDouble();
		}

		return d;
	}

	private void processRobotChunk() {
		try {
			setDrivetrainPID(getDoublesFromChunk(3));
			setShooterPID(getDoublesFromChunk(3));
			setShooterPower(currentChunk.nextDouble());
			setShooterAngle(currentChunk.nextDouble());
			setDrivetrainPowerLeft(currentChunk.nextDouble());
			setDrivetrainPowerRight(currentChunk.nextDouble());
			setHangLiftState(currentChunk.nextBoolean());
			setDiscLiftState(currentChunk.nextBoolean());
			setHopperState(currentChunk.nextBoolean());
			setSpatulaState(currentChunk.nextBoolean());
			setLaunchKickerState(currentChunk.nextBoolean());
			setActivateVisionControl(currentChunk.nextBoolean());
			setTargetX(currentChunk.nextInt());
			setTargetY(currentChunk.nextInt());
			setTargetWidth(currentChunk.nextInt());
			setTargetHeight(currentChunk.nextInt());

		} catch (ChunkReaderException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private void setDrivetrainPID(final double[] pidConstants) {
		System.out.println("DT P: " + pidConstants[0]);
		System.out.println("DT I: " + pidConstants[1]);
		System.out.println("DT D: " + pidConstants[2]);
	}

	private void setShooterPID(final double[] pidConstants) {
		System.out.println("Shooter P: " + pidConstants[0]);
		System.out.println("Shooter I: " + pidConstants[1]);
		System.out.println("Shooter D: " + pidConstants[2]);
	}

	private void setShooterPower(final double power) {
		System.out.println("Shooter Power: " + power);
	}

	private void setShooterAngle(final double angle) {
		System.out.println("Shooter Angle: " + angle);
	}

	private void setDrivetrainPowerLeft(final double power) {
		System.out.println("DTL Power: " + power);
	}

	private void setDrivetrainPowerRight(final double power) {
		System.out.println("DTR Power: " + power);
	}

	private synchronized void setHangLiftState(final boolean state) {
		System.out.println("Hang Lift State: " + state);
	}

	private synchronized void setDiscLiftState(final boolean state) {
		System.out.println("Disc Lift State: " + state);
	}

	private synchronized void setHopperState(final boolean state) {
		System.out.println("Hopper State: " + state);
	}

	private synchronized void setSpatulaState(final boolean state) {
		System.out.println("Spatula State: " + state);
	}

	private synchronized void setLaunchKickerState(final boolean state) {
		System.out.println("Launcher Kicker State: " + state);
	}

	private void setActivateVisionControl(final boolean state) {
		System.out.println("Activate Vision Control: " + state);
	}

	private void setTargetX(final int xVal) {
		System.out.println("Target X: " + xVal);
	}

	private void setTargetY(final int yVal) {
		System.out.println("Target Y: " + yVal);
	}

	private void setTargetWidth(final int widthVal) {
		System.out.println("Target Width: " + widthVal);
	}

	private void setTargetHeight(final int heightVal) {
		System.out.println("Target Height: " + heightVal);
	}
	
}
