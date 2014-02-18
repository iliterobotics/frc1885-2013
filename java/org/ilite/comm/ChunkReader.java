package org.ilite.comm;

public abstract class ChunkReader {

    /**
     * Default constructor. (I know. It's disguised. It really looks like
     * something more like a koala or panda. But it is, in fact, a default
     * constructor)
     */
    protected ChunkReader() {
    }

    /**
     * Determines whether we're at the end of the stream
     * 
     * @return true if we've reached the end. False otherwise
     */
    public abstract boolean atEnd();

    /**
     * Gets the type of the next available piece of data
     * 
     * @return null on end of stream, otherwise the type of the next datum
     * @throws InvalidTypeException
     *             on malformatted/unrecognizable type and/or size of data. Also
     *             if end of stream is reached when trying to parse, this will
     *             be thrown.
     */
    public abstract DataType nextType() throws InvalidTypeException;

    /**
     * Gets the next datum available as an integer. A type check is performed to
     * guarantee everything goes smoothly/as intended
     * 
     * @return null if end of stream is hit cleanly, otherwise an integer.
     * @throws ChunkReaderException
     *             for any reason nextType() would throw an exception, or
     *             inability to parse the result. Or end of stream is
     *             unexpectedly reached.
     */
    public abstract Integer nextInt() throws ChunkReaderException;

    /**
     * Gets the next datum available as an integer. A type check is performed to
     * guarantee everything goes smoothly/as intended
     * 
     * @return null if end of stream is hit cleanly, otherwise an integer.
     * @throws ChunkReaderException
     *             for any reason nextType() would throw an exception, or
     *             inability to parse the result. Or end of stream is
     *             unexpectedly reached.
     */
    public abstract Byte nextByte() throws ChunkReaderException;

    /**
     * Gets the next datum available as an boolean. A type check is performed to
     * guarantee everything goes smoothly/as intended
     * 
     * @return null if end of stream is hit cleanly, otherwise an boolean.
     * @throws ChunkReaderException
     *             for any reason nextType() would throw an exception, or
     *             inability to parse the result. Or end of stream is
     *             unexpectedly reached.
     */
    public abstract Boolean nextBoolean() throws ChunkReaderException;

    /**
     * Gets the next datum available as an double. A type check is performed to
     * guarantee everything goes smoothly/as intended
     * 
     * @return null if end of stream is hit cleanly, otherwise a double.
     * @throws ChunkReaderException
     *             for any reason nextType() would throw an exception, or
     *             inability to parse the result. Or end of stream is
     *             unexpectedly reached.
     */
    public abstract Double nextDouble() throws ChunkReaderException;

    /**
     * Gets the next datum available as an string. A type check is performed to
     * guarantee everything goes smoothly/as intended
     * 
     * @return null if end of stream is hit cleanly, otherwise a string.
     * @throws ChunkReaderException
     *             for any reason nextType() would throw an exception, or
     *             inability to parse the result. Or end of stream is
     *             unexpectedly reached.
     */
    public abstract String nextString() throws ChunkReaderException;

    /**
     * Gets the next datum available as an chunk. A type check is performed to
     * guarantee everything goes smoothly/as intended
     * 
     * @return null if end of stream is hit cleanly, otherwise a chunk.
     * @throws ChunkReaderException
     *             for any reason nextType() would throw an exception, or
     *             inability to parse the result. Or end of stream is
     *             unexpectedly reached.
     */
    public abstract byte[] nextChunk() throws ChunkReaderException;
    
    /**
     * Get size of chunk currently
     */
    public abstract int chunkSize();
}
