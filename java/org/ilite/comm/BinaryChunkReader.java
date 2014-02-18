package org.ilite.comm;

import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

public class BinaryChunkReader extends ChunkReader {
    private final ByteBuffer m_bytebuf;
    private DataType m_typeCache = null;
    private boolean m_sizeCache = false;

    /**
     * Creates a binary chunk reader from a given byte array.
     * 
     * @param buffer
     *            - buffer to read
     * @throws IllegalArgumentException
     *             if the array is not a binary chunk
     */
    public BinaryChunkReader(byte[] buffer) {
        m_bytebuf = ByteBuffer.wrap(buffer);

        if (!isBinaryChunk(m_bytebuf)) {
            throw new IllegalArgumentException(
                    "Need to be passed a binary chunk.");
        }
    }

    /**
     * Creates a binary chunk reader from a given byte array.
     * 
     * @param buffer
     *            - buffer to read
     * @param offset
     *            - offset from the start of the array to read
     * @param length
     *            - length after the offset to go
     * @throws IllegalArgumentException
     *             if the array is not a binary chunk
     */
    public BinaryChunkReader(byte[] buffer, int offset, int length) {
        m_bytebuf = ByteBuffer.wrap(buffer, offset, length);

        if (!isBinaryChunk(m_bytebuf)) {
            throw new IllegalArgumentException(
                    "Need to be passed a binary chunk.");
        }
    }

    private static boolean isBinaryChunk(ByteBuffer b) {
        if (b == null || b.remaining() < Common.BIN_HEADER.length) {
            return false;
        }

        for (int i = 0; i < Common.BIN_HEADER.length; ++i) {
            if (b.get() != Common.BIN_HEADER[i]) {
                return false;
            }
        }

        return true;
    }

    private static boolean typeHasSize(short s) {
        return (s & 0x8000) != 0;
    }

    private static DataType typeFromBin(short s) {
        return DataType.lookup(s & 0xFF);
    }

    private void invalidateCache() {
        m_typeCache = null;
        m_sizeCache = false;
    }

    @Override
    public DataType nextType() throws InvalidTypeException {
        if (m_typeCache != null) {
            return m_typeCache;
        }

        if (!atEnd()) {
            return null;
        }

        short shrt;

        try {
            shrt = m_bytebuf.getShort();
        } catch (BufferUnderflowException e) {
            return null;
        }

        DataType result = typeFromBin(shrt);

        if (result == null) {
            throw new InvalidTypeException("Invalid type retreived: " + shrt);
        }

        m_sizeCache = typeHasSize(shrt);

        return result;
    }

    @Override
    public Integer nextInt() throws ChunkReaderException {
        DataType nxt = nextType();

        // null == end of stream
        if (nxt == null) {
            return null;
        }

        if (nxt != DataType.Integer) {
            throw new InconsistentTypeException(
                    "Tried to parse Integer. Actual type: " + nxt.toString());
        }

        if (m_sizeCache) {
            throw new ChunkReaderException(
                    "Primitive types can't handle sizes yet.");
        }

        if (m_bytebuf.remaining() < 4) {
            throw new ChunkReaderException(
                    "Not enough bytes to read a full Integer");
        }

        int i = m_bytebuf.getInt();

        invalidateCache();

        return i;
    }

    @Override
    public Byte nextByte() throws ChunkReaderException {
        DataType nxt = nextType();

        // null == end of stream
        if (nxt == null) {
            return null;
        }

        if (nxt != DataType.Byte) {
            throw new InconsistentTypeException(
                    "Tried to parse Byte. Actual type: " + nxt.toString());
        }

        if (m_sizeCache) {
            throw new ChunkReaderException(
                    "Primitive types can't handle sizes yet.");
        }

        if (m_bytebuf.remaining() < 1) {
            throw new ChunkReaderException(
                    "Not enough bytes to read a full Byte");
        }

        byte b = m_bytebuf.get();

        invalidateCache();

        return b;
    }

    @Override
    public Boolean nextBoolean() throws ChunkReaderException {
        DataType nxt = nextType();

        // null == end of stream
        if (nxt == null) {
            return null;
        }

        if (nxt != DataType.Boolean) {
            throw new InconsistentTypeException(
                    "Tried to parse Boolean. Actual type: " + nxt.toString());
        }

        if (m_sizeCache) {
            throw new ChunkReaderException(
                    "Primitive types can't handle sizes yet.");
        }

        if (m_bytebuf.remaining() < 1) {
            throw new ChunkReaderException(
                    "Not enough bytes to read a full Boolean");
        }

        boolean b = m_bytebuf.get() != 0;

        invalidateCache();

        return b;
    }

    @Override
    public Double nextDouble() throws ChunkReaderException {
        DataType nxt = nextType();

        // null == end of stream
        if (nxt == null) {
            return null;
        }

        if (nxt != DataType.Double) {
            throw new InconsistentTypeException(
                    "Tried to parse String. Actual type: " + nxt.toString());
        }

        if (!m_sizeCache) {
            throw new ChunkReaderException(
                    "No size given with Double. Cannot read.");
        }

        if (m_bytebuf.remaining() < 4) {
            throw new ChunkReaderException(
                    "Not enough bytes to read String length");
        }

        int numchars = m_bytebuf.getInt();

        if (numchars < 0) {
            throw new ChunkReaderException("Invalid String size received: "
                    + numchars);
        }

        // Shortcut/micro-optimization
        if (numchars == 0) {
            return 0.0;
        }

        if (m_bytebuf.remaining() < numchars) {
            throw new ChunkReaderException("Insufficient data to supply "
                    + numchars + " chars");
        }

        byte[] arr = new byte[numchars];

        m_bytebuf.get(arr);
        invalidateCache();

        double d = Double.parseDouble(new String(arr, Common.TEXT_CHARSET));

        invalidateCache();

        return d;
    }

    @Override
    public String nextString() throws ChunkReaderException {
        DataType nxt = nextType();

        // null == end of stream
        if (nxt == null) {
            return null;
        }

        if (nxt != DataType.String) {
            throw new InconsistentTypeException(
                    "Tried to parse String. Actual type: " + nxt.toString());
        }

        if (!m_sizeCache) {
            throw new ChunkReaderException(
                    "No size given with String. Cannot read.");
        }

        if (m_bytebuf.remaining() < 4) {
            throw new ChunkReaderException(
                    "Not enough bytes to read String length");
        }

        int numchars = m_bytebuf.getInt();

        if (numchars < 0) {
            throw new ChunkReaderException("Invalid String size received: "
                    + numchars);
        }

        // Shortcut/micro-optimization
        if (numchars == 0) {
            return "";
        }

        if (m_bytebuf.remaining() < numchars) {
            throw new ChunkReaderException("Insufficient data to supply "
                    + numchars + " chars");
        }

        byte[] arr = new byte[numchars];

        m_bytebuf.get(arr);
        invalidateCache();

        return new String(arr, Common.TEXT_CHARSET);
    }

    @Override
    public byte[] nextChunk() throws ChunkReaderException {
        DataType nxt = nextType();

        // null == end of stream
        if (nxt == null) {
            return null;
        }

        if (nxt != DataType.ChunkObject) {
            throw new InconsistentTypeException(
                    "Tried to parse ChunkObject. Actual type: "
                            + nxt.toString());
        }

        if (!m_sizeCache) {
            throw new ChunkReaderException(
                    "No size given with ChunkObject. Cannot read.");
        }

        if (m_bytebuf.remaining() < 4) {
            throw new ChunkReaderException(
                    "Not enough bytes to read chunkobject length");
        }

        int numchars = m_bytebuf.getInt();

        if (numchars < 0) {
            throw new ChunkReaderException(
                    "Invalid ChunkObject size received: " + numchars);
        }

        // Shortcut/micro-optimization
        if (numchars == 0) {
            return new byte[0];
        }

        if (m_bytebuf.remaining() < numchars) {
            throw new ChunkReaderException("Insufficient data to supply "
                    + numchars + " bytes");
        }

        byte[] arr = new byte[numchars];

        m_bytebuf.get(arr);
        invalidateCache();

        return arr;
    }

    @Override
    public boolean atEnd() {
        return m_bytebuf.hasRemaining();
    }
    
    public int chunkSize()
    {
        return m_bytebuf.array().length;
    }
}
