package org.ilite.comm;

import java.nio.ByteBuffer;

public class BinaryChunkWriter extends ChunkWriter {
    /**
     * Creates a new binary chunk writer
     */
    public BinaryChunkWriter() {
        rawWriteChunk(Common.BIN_HEADER);
    }

    /**
     * Creates a new binary chunk writer
     * 
     * @param byteArr
     *            - if null or 0 length, this acts the same as the default
     *            constructor. If it is not given a binary chunk, this will
     *            complain. Otherwise, it will be happy.
     * @throws IllegalArgumentException
     *             on a non-binary chunk being given.
     */
    public BinaryChunkWriter(byte[] byteArr) {
        if (byteArr != null && byteArr.length > 0) {
            rawWriteChunk(byteArr);
        } else {
            if (Common.matches(Common.BIN_HEADER, byteArr)) {
                throw new IllegalArgumentException("Need a binary chunk.");
            }

            rawWriteChunk(Common.BIN_HEADER);
        }
    }

    @Override
    public void writeInt(int toWrite) {
        ByteBuffer bb = ByteBuffer.allocate(4).putInt(toWrite);
        writeTypedChunk(DataType.Integer, bb.array());
    }

    @Override
    public void writeByte(byte toWrite) {
        ByteBuffer bb = ByteBuffer.allocate(1).put(toWrite);
        writeTypedChunk(DataType.Byte, bb.array());
    }

    @Override
    public void writeBoolean(boolean toWrite) {
        final byte b = (byte) (toWrite ? 1 : 0);
        writeTypedChunk(DataType.Boolean, new byte[] { b });
    }

    @Override
    public void writeDouble(double toWrite) {
        String res = Double.toString(toWrite);
        writeTypedChunk(DataType.Double, res.getBytes(Common.TEXT_CHARSET));
    }

    @Override
    public void writeString(String toWrite) {
        if (toWrite == null) {
            return;
        }

        writeTypedChunk(DataType.String, toWrite.getBytes(Common.TEXT_CHARSET));
    }

    @Override
    public void writeChunk(byte[] toWrite) {
        writeTypedChunk(DataType.ChunkObject, toWrite);
    }

    private void writeTypedChunk(DataType type, byte[] toWrite) {
        if (toWrite == null) {
            return;
        }

        if (type == null) {
            throw new IllegalArgumentException("Can't write null type");
        }

        byte[] header = null;

        switch (type) {
        case Boolean:
        case Integer:
        case Byte:
        case Char:
            header = ByteBuffer.allocate(2).putShort(type.byteValue()).array();
            break;

        case Double:
        case String:
        case ChunkObject: {
            ByteBuffer bb = ByteBuffer.allocate(6);
            short tinfo = (short) ((1 << 15) | type.byteValue());
            bb.putShort(tinfo);
            bb.putInt(toWrite.length);
            header = bb.array();
            break;
        }

        case Unreadable:
            throw new IllegalStateException("Can't put unreadable type.");
        }

        assert header != null;

        rawWriteChunk(header);
        rawWriteChunk(toWrite);
    }
}
