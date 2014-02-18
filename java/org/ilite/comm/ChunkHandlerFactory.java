package org.ilite.comm;

public class ChunkHandlerFactory {
    private static boolean m_binwriter = true;

    /**
     * {@see ChunkHandlerFactory.readerFor(byte[], int, int}
     * 
     * @param chunk
     * @return
     */
    public static ChunkReader readerFor(byte[] chunk) {
        if (chunk == null) {
            return null;
        }

        return readerFor(chunk, 0, chunk.length);
    }

    /**
     * Gets the appropriate ChunkReader given a chunk.
     * 
     * @param chunk
     *            chunk to base the reader off of
     * @param offset
     *            offset in array to start reader at
     * @param length
     *            length of byte array
     * @return null on no suitable reader found, non-null otherwise
     */
    public static ChunkReader readerFor(byte[] chunk, int offset, int length) {
        if (chunk == null) {
            return null;
        }

        if (Common.matches(Common.BIN_HEADER, chunk, offset, length)) {
            return new BinaryChunkReader(chunk, offset, length);
        } else if (Common.matches(Common.TXT_HEADER, chunk, offset, length)) {
            return new TextChunkReader(chunk, offset, length);
        }

        return null;
    }

    /**
     * Sets the preferredWriter to be binary
     */
    public static void setPreferredWriterBinary() {
        m_binwriter = true;
    }

    /**
     * Sets the preferredWriter to be text
     */
    public static void setPreferredWriterText() {
        m_binwriter = false;
    }

    /**
     * Returns our preferred ChunkWriter.
     * 
     * @return preferred ChunkWriter. Should always return non-null
     */
    public static ChunkWriter preferredWriter() {
        if (m_binwriter) {
            return new BinaryChunkWriter();
        }

        return new TextChunkWriter();
    }

    /**
     * Given a byte array, returns an appropriate writer with the array written
     * to it.
     * 
     * @param arr
     *            - array to initialize/match a writer with.
     * @return null if no appropriate writer was found. Otherwise a writer
     *         initialized with the given data is returned.
     */
    public static ChunkWriter writerFor(byte[] arr) {
        if (arr == null || arr.length == 0) {
            return preferredWriter();
        }

        if (Common.matches(Common.BIN_HEADER, arr)) {
            return new BinaryChunkWriter(arr);
        }

        if (Common.matches(Common.TXT_HEADER, arr)) {
            return new TextChunkWriter(arr);
        }

        return preferredWriter();
    }

}
