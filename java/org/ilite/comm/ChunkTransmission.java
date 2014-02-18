package org.ilite.comm;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;

public class ChunkTransmission {
    public static void write(OutputStream to, ChunkWriter wr)
            throws IOException {
        if (to == null || wr == null) {
            throw new IllegalArgumentException("to or wr is null");
        }

        ByteBuffer bbuf = ByteBuffer.allocate(4);

        bbuf.putInt(wr.chunkSize());

        to.write(bbuf.array());
        wr.writeTo(to);
    }

    public static ChunkReader read(InputStream from) throws IOException,
            ChunkReaderException {
        byte[] buf = new byte[4];

        from.read(buf);

        ByteBuffer bufinterp = ByteBuffer.wrap(buf);

        int chunksize = bufinterp.getInt();

        if (chunksize < 0) {
            throw new ChunkReaderException("Size of chunk is invalid.");
        }

        byte[] chunkbuf = new byte[chunksize];

        // This read should block until the buffer is filled up
        int numread = from.read(chunkbuf);

        if (numread < chunksize) {
            throw new ChunkReaderException(
                    "Not enough bytes received to compliment reported size of "
                            + chunksize + " prior to stream terminating. Got " + numread + " bytes.");
        }

        return ChunkHandlerFactory.readerFor(chunkbuf);
    }
}
