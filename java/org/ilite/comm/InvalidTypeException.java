package org.ilite.comm;

public class InvalidTypeException extends ChunkReaderException {
    private static final long serialVersionUID = -1652656842835946392L;

    public InvalidTypeException() {
    }

    public InvalidTypeException(String message) {
        super(message);
    }

    public InvalidTypeException(String message, Throwable cause) {
        super(message, cause);
    }

    public InvalidTypeException(Throwable cause) {
        super(cause);
    }
}
