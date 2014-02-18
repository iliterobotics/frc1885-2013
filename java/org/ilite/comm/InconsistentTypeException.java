package org.ilite.comm;

public class InconsistentTypeException extends ChunkReaderException {
    private static final long serialVersionUID = -4811443674335354104L;

    public InconsistentTypeException() {
    }

    public InconsistentTypeException(String message) {
        super(message);
    }

    public InconsistentTypeException(String message, Throwable cause) {
        super(message, cause);
    }

    public InconsistentTypeException(Throwable cause) {
        super(cause);
    }
}
