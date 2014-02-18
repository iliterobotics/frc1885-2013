package org.ilite.comm;

public class TextChunkReader extends ChunkReader {
    private static final char FIELD_DELIM = ':';
    private byte[] m_charbuf;
    private int m_buffoffset = 0;
    private DataType m_typeCache = null;
    private byte[] m_remainderCache = null;

    /**
     * Forms TextChunkReader given a buffer.
     * 
     * @param buffer
     *            - buffer to load into the reader
     * @throws IllegalArgumentException
     *             on the chunk not being a valid Text chunk
     */
    public TextChunkReader(byte[] buffer) {
        if (!isTextChunk(buffer)) {
            throw new IllegalArgumentException("Need text chunk.");
        }

        final int off = Common.TXT_HEADER.length;
        final int len = buffer.length - off;

        m_charbuf = chopTop(buffer, off, len);
    }

    /**
     * Forms TextChunkReader given a buffer.
     * 
     * @param buffer
     *            - buffer to load into the reader
     * @throws IllegalArgumentException
     *             on the chunk not being a valid Text chunk
     */
    public TextChunkReader(byte[] buffer, int offset, int length) {
        if (!isTextChunk(buffer, offset, length)) {
            throw new IllegalArgumentException("Need text chunk.");
        }

        final int off = Common.TXT_HEADER.length;
        final int len = buffer.length - off;

        m_charbuf = chopTop(buffer, off, len);
    }

    private static byte[] chopTop(byte[] init, int offset, int length) {
        final byte[] barr = new byte[length];

        System.arraycopy(init, offset, barr, 0, length);

        return barr;
    }

    private static String nextFieldAsString(byte[] arr, int offset, int length) {
        if (arr == null) {
            return null;
        }

        StringBuilder sb = new StringBuilder();

        int len = 5;

        while (offset < arr.length) {
            // If we would go out of bounds, adjust len.
            if (length + offset < len + offset) {
                len = length - offset;
            }

            String s = new String(arr, offset, len, Common.TEXT_CHARSET);

            int iof = s.indexOf(FIELD_DELIM);

            if (iof != -1) {
                if (iof > 0) {
                    sb.append(s.substring(0, iof));
                }
                break;
            }

            sb.append(s);

            offset += len;
        }

        return sb.toString();
    }

    private static boolean isTextChunk(byte[] b) {
        if (b == null) {
            return false;
        }

        return isTextChunk(b, 0, b.length);
    }

    private static boolean isTextChunk(byte[] b, int offset, int length) {
        if (b == null || offset + length < Common.TXT_HEADER.length) {
            return false;
        }

        if (b.length < offset + length) {
            throw new IllegalArgumentException(
                    "Offset + length need to be <= array length");
        }

        for (int i = 0; i < Common.TXT_HEADER.length; ++i) {
            if (Common.TXT_HEADER[i] != b[i + offset]) {
                return false;
            }
        }

        return true;
    }

    private String remainderAsString() {
        if (m_remainderCache == null) {
            return null;
        }

        return new String(m_remainderCache, Common.TEXT_CHARSET);
    }

    private void invalidateCache() {
        m_typeCache = null;
        m_remainderCache = null;
    }

    @Override
    public DataType nextType() throws InvalidTypeException {
        if (m_typeCache != null) {
            return m_typeCache;
        }

        if (atEnd()) {
            return null;
        }

        String sizes = nextFieldAsString(m_charbuf, m_buffoffset,
                m_charbuf.length - m_buffoffset);

        if (sizes.length() >= m_charbuf.length) {
            throw new InvalidTypeException("No size field delimeter found");
        }

        int totsize = 0;

        try {
            // parse the int that should be located at the current cursor.
            totsize = Integer.parseInt(sizes);
        } catch (NumberFormatException e) {
            throw new InvalidTypeException("No (readable) size given.", e);
        }

        String types = nextFieldAsString(m_charbuf, sizes.length()
                + m_buffoffset + 1, totsize);

        // If 'type' goes to the end of the string, we have a problem...
        if (types.length() == totsize - (sizes.length() + 1)) {
            throw new InvalidTypeException(
                    "No delim found. Type cannot be determined.");
        }

        final DataType type = DataType.lookup(types);

        if (type == null) {
            throw new InvalidTypeException("Could not resolve type " + types);
        }

        m_typeCache = type;

        // + 2 rids us of the two :'s
        // If doing this repeatedly (as opposed to keeping track of more
        // indexes) becomes too slow, we can change to keeping track.
        // Until then, though, this is easier. So we'll go with it
        final int remstart = sizes.length() + types.length() + 2;

        // And this grabs for us the value string. The -1 chops off
        // the ':' at the very end.
        // e.x. it gives us the end number so we can parse 12345 out of 
        // 11:int:12345:
        final int remsize = totsize + sizes.length() - remstart - 1;

        m_remainderCache = chopTop(m_charbuf, remstart + m_buffoffset, remsize);

        final int newOffset = m_buffoffset + totsize + sizes.length();

        if (newOffset >= m_charbuf.length) {
            m_charbuf = null;
            m_buffoffset = 0;
        } else {
            m_buffoffset = newOffset;
        }

        return m_typeCache;
    }

    @Override
    public Integer nextInt() throws ChunkReaderException {
        DataType type = nextType();

        if (type == null) {
            return null;
        }

        if (type != DataType.Integer) {
            throw new InvalidTypeException("Integer not parsible from "
                    + type.name());
        }

        /*
         * Exception is thrown here because a type and size have already been
         * parsed. So if it's lacking an integer to back the int type and size,
         * it's a malformed Chunk.
         */
        if (atEnd()) {
            throw new ChunkReaderException(
                    "No Integer found -- end of stream encountered.");
        }

        assert m_remainderCache != null;

        int result = 0;

        try {
            result = Integer.parseInt(remainderAsString());
        } catch (NumberFormatException e) {
            throw new ChunkReaderException("Failed to parse int out of "
                    + m_remainderCache, e);
        }

        invalidateCache();

        return result;
    }

    @Override
    public Byte nextByte() throws ChunkReaderException {
        DataType type = nextType();

        if (type == null) {
            return null;
        }

        if (type != DataType.Byte) {
            throw new InvalidTypeException("Byte not parsible from "
                    + type.name());
        }

        /*
         * Exception is thrown here because a type and size have already been
         * parsed. So if it's lacking an integer to back the int type and size,
         * it's a malformed Chunk.
         */
        if (atEnd()) {
            throw new ChunkReaderException(
                    "No Byte found -- end of stream encountered.");
        }

        assert m_remainderCache != null;

        int result = 0;

        try {
            result = Integer.parseInt(remainderAsString());
        } catch (NumberFormatException e) {
            throw new ChunkReaderException("Failed to parse int out of "
                    + m_remainderCache, e);
        }

        invalidateCache();

        return (byte)result;
    }

    @Override
    public Boolean nextBoolean() throws ChunkReaderException {
        DataType type = nextType();

        if (type == null) {
            return null;
        }

        if (type != DataType.Boolean) {
            throw new InvalidTypeException("Boolean not parsible from "
                    + type.name());
        }

        if (atEnd()) {
            throw new ChunkReaderException(
                    "No Boolean found -- end of stream encountered.");
        }

        assert m_remainderCache != null;

        int result = 0;

        try {
            result = Integer.parseInt(remainderAsString());
        } catch (NumberFormatException e) {
            throw new ChunkReaderException("Failed to parse bool out of "
                    + m_remainderCache, e);
        }

        invalidateCache();

        return result != 0;
    }

    @Override
    public Double nextDouble() throws ChunkReaderException {
        DataType type = nextType();

        if (type == null) {
            return null;
        }

        if (type != DataType.Double) {
            throw new InvalidTypeException("Double not parsible from "
                    + type.name());
        }

        if (atEnd()) {
            throw new ChunkReaderException(
                    "No Double found -- end of stream encountered.");
        }

        assert m_remainderCache != null;

        double result = 0;

        try {
            result = Double.parseDouble(remainderAsString());
        } catch (NumberFormatException e) {
            throw new ChunkReaderException("Failed to parse double out of "
                    + m_remainderCache, e);
        }

        invalidateCache();

        return result;
    }

    @Override
    public String nextString() throws ChunkReaderException {
        DataType type = nextType();

        if (type == null) {
            return null;
        }

        if (type != DataType.String) {
            throw new InvalidTypeException("Integer not parsible from "
                    + type.name());
        }

        if (atEnd()) {
            throw new ChunkReaderException(
                    "No Integer found -- end of stream encountered.");
        }

        assert m_remainderCache != null;

        String result = remainderAsString();

        invalidateCache();

        return result;
    }

    @Override
    public byte[] nextChunk() throws ChunkReaderException {
        DataType type = nextType();

        if (type == null) {
            return null;
        }

        if (type != DataType.ChunkObject) {
            throw new InvalidTypeException("Integer not parsible from "
                    + type.name());
        }

        if (atEnd()) {
            throw new ChunkReaderException(
                    "No Integer found -- end of stream encountered.");
        }

        assert m_remainderCache != null;

        byte[] b = m_remainderCache;

        invalidateCache();

        return b;
    }

    @Override
    public boolean atEnd() {
        /*
         * typeCache and remainderCache will keep the last bit of data for us,
         * so charbuf can be null or 0-length when there's still a field left.
         */
        return (m_charbuf == null || m_charbuf.length == 0)
                && m_typeCache == null;
    }

    public int chunkSize()
    {
        return m_charbuf.length;
    }
}
