package ilite.display.modules.display2009;

import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.concurrent.Semaphore;

import javax.swing.SwingUtilities;

public class FRC_HostDataReader {

	public enum tePlayerState {
		eePlaying, eePause, eeStop, eeResetting;
	}

	private tePlayerState mePlayerState = tePlayerState.eeStop;

	private FileInputStream mcFile = null;

	private DataInputStream mcInDataStream = null;

	private Thread mcThread = null;

	private Semaphore mcPlayingDataSemCnt = null;

	private Semaphore mcActionSem = null;

	private String mcFileName = null;

	private long mnFileSize = 0;

	private long mnTotalRead = 0;

	private DataPlayerPlatform mcGUI;

	public FRC_HostDataReader(DataPlayerPlatform acGUI) {
		mcGUI = acGUI;

		mcPlayingDataSemCnt = new Semaphore(0, true);
		mcActionSem = new Semaphore(1, true);

		mcThread = new Thread(
				new Runnable() {
					public void run(){
						HiddenRun();
					}
				});
		mcThread.start();
	}

	public boolean LoadFile(String aFileName) {
		boolean lbRequestStatus = true;
		// Semaphore is being use here instead of sync. keyword because
		// one of the areas' that needs thread safe protection is within
		// a while loop.
		try {
			mcActionSem.acquire();
		} catch (InterruptedException E) {
			// If semaphore is not acquire - no action can be taken
			return false;
		}
		try {
			File lcGetFileSize = new File(aFileName);
			mnFileSize = lcGetFileSize.length();
			mnTotalRead = 0;
			lcGetFileSize = null;

			mcFileName = new String(aFileName);
			mcFile = new FileInputStream(mcFileName);
			mcInDataStream = new DataInputStream(mcFile);

		} catch (FileNotFoundException E) {
			mcFile = null;
			lbRequestStatus = false;
			System.out.println(E.getMessage());
		}
		mcActionSem.release();

		return lbRequestStatus;

	}

	public boolean play() {
		// Semaphore is being use here instead of sync. keyword because
		// one of the areas' that needs thread safe protection is within
		// a while loop.
		try {
			mcActionSem.acquire();
		} catch (InterruptedException E) {
			// If semaphore is not acquire - no action can be taken
			return false;
		}

		boolean lbRequestStatus = true;
		if (mcFile == null) {
			lbRequestStatus = false;
		} else {
			if (mePlayerState == tePlayerState.eePlaying
					|| mePlayerState == tePlayerState.eeResetting) {
				// ignore
				lbRequestStatus = false;
			} else {
				// set the Semaphores - this will allow the mutlithreaded run()
				// to start playing data from a file
				mePlayerState = tePlayerState.eePlaying;
				mcPlayingDataSemCnt.release();
			}

		}
		mcActionSem.release();
		return lbRequestStatus;
	}

	public boolean pause() {
		// Semaphore is being use here instead of sync. keyword because
		// one of the areas' that needs thread safe protection is within
		// a while loop.
		try {
			mcActionSem.acquire();
		} catch (InterruptedException E) {
			// If semaphore is not acquire - no action can be taken
			return false;
		}

		boolean lbRequestStatus = true;
		if (mcFile == null) {
			lbRequestStatus = false;
		} else {
			// we can only pause if a file is being played
			if (mePlayerState == tePlayerState.eePlaying) {
				try {
					mcPlayingDataSemCnt.acquire();
				} catch (InterruptedException E) {
					// do nothing
				}
				mePlayerState = tePlayerState.eePause;
			}
		}
		mcActionSem.release();
		return lbRequestStatus;
	}

	public boolean stop() {
		// Semaphore is being use here instead of sync. keyword because
		// one of the areas' that needs thread safe protection is within
		// a while loop.
		try {
			mcActionSem.acquire();
		} catch (InterruptedException E) {
			// If semaphore is not acquire - no action can be taken
			return false;
		}

		boolean lbRequestStatus = true;
		if (mcFile == null) {
			lbRequestStatus = false;
		} else {
			if (mePlayerState == tePlayerState.eePlaying) {
				try {
					mcPlayingDataSemCnt.acquire();
				} catch (InterruptedException E) {
					// do nothing
				}
				mePlayerState = tePlayerState.eeStop;
			}

		}

		mcActionSem.release();
		return lbRequestStatus;
	}

	public boolean reset() {
		// Semaphore is being use here instead of sync. keyword because
		// one of the areas' that needs thread safe protection is within
		// a while loop.
		try {
			mcActionSem.acquire();
		} catch (InterruptedException E) {
			// If semaphore is not acquire - no action can be taken
			return false;
		}

		boolean lbRequestStatus = true;
		if (mcFile == null) {
			lbRequestStatus = false;
		} else {
			if (mePlayerState == tePlayerState.eeStop) {
				try {
					mcInDataStream.close();// close old file
					mcFile = new FileInputStream(mcFileName);
					mcInDataStream = new DataInputStream(mcFile);
					mnTotalRead = 0;
				} catch (FileNotFoundException E) {
					mcFile = null;
				} catch (IOException E) {

				}
			}

		}

		mcActionSem.release();
		return lbRequestStatus;
	}

	private void internalReset() {
		try {
			mcInDataStream.close();// close old file
			mcFile = new FileInputStream(mcFileName);
			mcInDataStream = new DataInputStream(mcFile);
			mnTotalRead = 0;
		} catch (FileNotFoundException E) {
			mcFile = null;
		} catch (IOException E) {

		}
	}

	private void HiddenRun() {
		boolean lbReleaseSemNotEOF = true;
		while (true) {
			try {
				// wait for permission to start playing file
				mcPlayingDataSemCnt.acquire();
				lbReleaseSemNotEOF = true;

				if (mePlayerState == tePlayerState.eePlaying) {
					// playOneFRCMsg only returns true when it has played all
					// data in file
					if (playOneFRCMsg()) {
						// lock sem. to make sure data does not change on us
						// while
						// this clean up action is being performed.
						mcActionSem.acquire();
						lbReleaseSemNotEOF = false;// set flag not to release
						internalReset();
						SwingUtilities.invokeLater(new Runnable() {
							public void run() {
								mcGUI.FileLoadedSuccessAction();
								mcGUI.setProgress(0, 100);
							}
						});

						mePlayerState = tePlayerState.eeStop;

						mcActionSem.release();
					}
				}

				// release sem so we can re-acquire next time in loop
				// Needs to be last action in this while loop
				if (lbReleaseSemNotEOF) {
					mcPlayingDataSemCnt.release();
				}
			} catch (InterruptedException E) {
			}
		}
	}

	private boolean playOneFRCMsg() {
		boolean lbEndOfFile = false;
		int lnSizeofMsg;
		int lnSizeofMsgRead = 0;
		int lnMsgReadStatus;
		byte[] laMsgArea = null;

		try {

			lnSizeofMsg = mcInDataStream.readInt();
			mnTotalRead += 4;

			if (lnSizeofMsg == -1) {
				lbEndOfFile = true;
			} else {
				laMsgArea = new byte[lnSizeofMsg];
				while (lnSizeofMsgRead < lnSizeofMsg) {
					lnMsgReadStatus = mcInDataStream.read(laMsgArea,
							lnSizeofMsgRead, lnSizeofMsg - lnSizeofMsgRead);
					if (lnMsgReadStatus == -1) {
						lbEndOfFile = true;
						break;
					} else {
						lnSizeofMsgRead += lnMsgReadStatus;
					}
				}
			}
			// do nothing if we reach the end of the file because the msg is in
			if (lbEndOfFile == false) {
				mnTotalRead += lnSizeofMsgRead;
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						mcGUI.setProgress(mnTotalRead, mnFileSize);
					}
				});
			} else {
				SwingUtilities.invokeLater(new Runnable() {
					public void run() {
						mcGUI.setProgress(mnTotalRead, mnFileSize);
					}
				});
				System.out.println("end of file reach");
			}
		} catch (IOException E) {
			System.out.println("Exception IO: " + E.getMessage());
			lbEndOfFile = true;
		}

		return lbEndOfFile;
	}

}
