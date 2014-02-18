package org.ilite.comm;

public class ChunkReaderException extends Exception {
    private static final long serialVersionUID = -9163522096180797916L;

    public ChunkReaderException() {
    }

    public ChunkReaderException(String message) {
        super(message);
    }

    public ChunkReaderException(String message, Throwable cause) {
        super(message, cause);
    }

    public ChunkReaderException(Throwable cause) {
        super(cause);
    }
}
