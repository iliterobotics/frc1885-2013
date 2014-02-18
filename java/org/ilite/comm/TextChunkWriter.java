package org.ilite.comm;

public class TextChunkWriter extends ChunkWriter {
    private static final char FIELD_DELIM = ':';

    /**
     * Constructor.
     */
    public TextChunkWriter() {
        rawWriteChunk(Common.TXT_HEADER);
    }

    /**
     * Constructor from a byte array. If the given array is null or empty, it
     * will ignore it. Otherwise, it will append to a new array based on the
     * given array.
     * 
     * @throws IllegalArgumentException
     *             if the given chunk is not a text chunk.
     */
    public TextChunkWriter(byte[] byteArr) {
        if (byteArr != null && byteArr.length > 0) {
            if (!isTxtChunk(byteArr)) {
                throw new IllegalArgumentException("Need text chunk.");
            }

            rawWriteChunk(byteArr);
        } else {
            rawWriteChunk(Common.TXT_HEADER);
        }
    }

    private boolean isTxtChunk(byte[] byteArr) {
        if (byteArr.length < Common.TXT_HEADER.length) {
            return false;
        }

        final String s = new String(byteArr, 0, Common.TXT_HEADER.length,
                Common.TEXT_CHARSET);

        return s.equals(Common.TXT_HEADER);
    }

    @Override
    public void writeInt(int toWrite) {
        writeSizedBytes(DataType.Integer.strValue(), Integer.toString(toWrite));
    }

    @Override
    public void writeByte(byte toWrite) {
        writeSizedBytes(DataType.Byte.strValue(), Integer.toString((int)toWrite));
    }

    @Override
    public void writeBoolean(boolean toWrite) {
        final String wstr = (toWrite) ? "1" : "0";
        writeSizedBytes(DataType.Boolean.strValue(), wstr);
    }

    @Override
    public void writeDouble(double toWrite) {
        writeSizedBytes(DataType.Double.strValue(), Double.toString(toWrite));
    }

    @Override
    public void writeString(String toWrite) {
        writeSizedBytes(DataType.String.strValue(), toWrite);
    }

    @Override
    public void writeChunk(byte[] toWrite) {
        writeSizedBytes(DataType.ChunkObject.strValue(), toWrite);
    }

    private void writeSizedBytes(String prefix, String strbytes) {
        writeSizedBytes(prefix, strbytes.getBytes(Common.TEXT_CHARSET));
    }

    /**
     * Writes a sequence of bytes with a given prefix out to the chunk.
     * 
     * @param prefix
     * @param ba
     */
    private void writeSizedBytes(String prefix, byte[] ba) {
        // + 2 covers the two :'s
        String prior = new String(FIELD_DELIM + prefix + FIELD_DELIM);
        final String after = Character.toString(FIELD_DELIM);
        final int totalsz = prior.length() + after.length() + ba.length;

        prior = totalsz + prior;

        rawWriteChunk(prior.getBytes(Common.TEXT_CHARSET));
        rawWriteChunk(ba);
        rawWriteChunk(after.getBytes(Common.TEXT_CHARSET));
    }
}
