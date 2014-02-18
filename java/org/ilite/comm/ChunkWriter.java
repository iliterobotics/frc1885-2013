package org.ilite.comm;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public abstract class ChunkWriter {
    private final ByteArrayOutputStream m_buffer = new ByteArrayOutputStream();

    /**
     * De falt constructor
     */
    protected ChunkWriter() {
    }

    /**
     * Initializes the byte array to a copy of the given byte array.
     * 
     * @param byteArr
     *            - what to initialize with
     */
    protected ChunkWriter(byte[] byteArr) {
        if (byteArr != null) {
            rawWriteChunk(byteArr);
        }
    }

    /**
     * Writes an integer to the chunk
     * 
     * @param toWrite
     *            - integer to write
     */
    public abstract void writeInt(int toWrite);

    /**
     * Writes a byte to the chunk
     *
     * @param toWrite
     *            - boolean to write
     */
    public abstract void writeByte(byte toWrite);

    /**
     * Writes a boolean to the chunk
     * 
     * @param toWrite
     *            - boolean to write
     */
    public abstract void writeBoolean(boolean toWrite);

    /**
     * Writes a double to the chunk
     * 
     * @param toWrite
     *            - double to write
     */
    public abstract void writeDouble(double toWrite);

    /**
     * Writes a String to the chunk
     * 
     * @param toWrite
     *            - string to write
     */
    public abstract void writeString(String toWrite);

    /**
     * Writes a chunk to the chunk (yo dawg)
     * 
     * @param toWrite
     *            - chunk to write
     */
    public abstract void writeChunk(byte[] toWrite);

    /**
     * @return Current size of the chunk
     */
    public int chunkSize() {
        return m_buffer.size();
    }

    /**
     * @return a copied version of the chunk's internal byte array
     */
    public byte[] chunkCopy() {
        return m_buffer.toByteArray();
    }

    /**
     * Writes the chunk's internal byte array to an OutputStream. This avoids a
     * copy of a (potentially large) array.
     * 
     * @param to
     *            - Where to write the chunk to
     * @throws IOException
     *             if writing failed
     * @throws IllegalArgumentException
     *             if to is null
     */
    public void writeTo(OutputStream to) throws IOException {
        if (to == null) {
            throw new IllegalArgumentException("to cannot be null");
        }

        m_buffer.writeTo(to);
    }

    /**
     * Writes a given set of bytes to the chunk.
     * 
     * @param bytes
     */
    protected void rawWriteChunk(byte[] bytes) {
        try {
            m_buffer.write(bytes);
        } catch (IOException e) {
            /*
             * This is a dynamically expanding byte array. The Java
             * documentation said that write(byte[]) would not throw an
             * IOException; it just throws such an exception because that's how
             * OutputStream has the method signature set up.
             */
            e.printStackTrace();
        }
    }

    /**
     * Writes a single byte to the chunk. Poor byte. It must be lonely, out
     * there in the cold. Oh well, we'll give it friends! Assuming the chunk is
     * populated, right? It won't be alone, right? Oh lord. Please don't let it
     * be alone. It's gone so long... So long without friends! WHY WOULD YOU DO
     * THAT?! It's 1AM lol.
     * 
     * @param loneByte
     *            - the lone ranger to write
     */
    protected void rawWriteChunk(byte loneByte) {
        m_buffer.write(loneByte);
    }
}
