package org.ilite.comm;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Driver {
    private static String byteArrayToHex(byte[] a) {
        StringBuilder sb = new StringBuilder();
        for (byte b : a)
            sb.append(String.format("%02x", b & 0xff));
        return sb.toString();
    }

    private static void printChunk(byte[] bz) {
        if (ChunkHandlerFactory.readerFor(bz) instanceof BinaryChunkReader) {
            System.out.println(byteArrayToHex(bz));
        } else {
            System.out.println(new String(bz, Common.TEXT_CHARSET));
        }
    }

    // Our sole test. Ha. Ha. Ha. Get it?
    private static void soulTest(Class<? extends ChunkWriter> clazz) 
            throws Exception {
        ChunkWriter cw = clazz.newInstance();

        cw.writeInt(12345678);
        cw.writeInt(-12345678);
        cw.writeByte((byte)0x30);
        cw.writeByte((byte)-0x40);
        cw.writeBoolean(true);
        cw.writeDouble(3.1415926);
        cw.writeString("h3rro!");

        ChunkWriter cw2 = clazz.newInstance();

        assert cw2 != null;

        cw2.writeInt(87654321);
        cw2.writeInt(-87654321);
        cw2.writeByte((byte)0x30);
        cw2.writeByte((byte)-0x40);
        cw2.writeBoolean(true);
        cw2.writeDouble(2.7123456);
        cw2.writeString("hello there!");
        cw2.writeChunk(cw.chunkCopy());

        printChunk(cw2.chunkCopy());
        
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        ChunkTransmission.write(baos, cw2);
        
        InputStream bais = new ByteArrayInputStream(baos.toByteArray());

        ChunkReader cr = ChunkTransmission.read(bais);

        System.out.println(cr.nextInt());
        System.out.println(cr.nextInt());
        System.out.println(cr.nextByte());
        System.out.println(cr.nextByte());
        System.out.println(cr.nextBoolean());
        System.out.println(cr.nextDouble());
        System.out.println(cr.nextString());

        byte[] chnk = cr.nextChunk();

        ChunkReader cr2 = ChunkHandlerFactory.readerFor(chnk);

        printChunk(chnk);

        System.out.println(cr2.nextInt());
        System.out.println(cr2.nextInt());
        System.out.println(cr2.nextByte());
        System.out.println(cr2.nextByte());
        System.out.println(cr2.nextBoolean());
        System.out.println(cr2.nextDouble());
        System.out.println(cr2.nextString());
    }

    public static void main(String[] args) throws Exception {
        System.out.println("Running through with BinaryChunkWriter");
        soulTest(BinaryChunkWriter.class);

        System.out.println("Running through with TextChunkWriter");
        soulTest(TextChunkWriter.class);
    }
}
