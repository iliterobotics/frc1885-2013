
#ifndef __NetworkingPoC__textchunkwriter__
#define __NetworkingPoC__textchunkwriter__

#include <iostream>

#include "chunkwriter.h"
#include "chunkrwfactory.h"

namespace net
{
    class TextChunkWriter: public ChunkWriter
    {
    public:
        TextChunkWriter();
        
        virtual ~TextChunkWriter();
        
        // -> Overrides. See ChunkWriter for descriptions.
        virtual int writeInt(int32 toWrite);
        virtual int writeByte(byte toWrite);
        virtual int writeDouble(float64 toWrite);
        virtual int writeBoolean(bool8 toWrite);
        virtual int writeString(const std::string* toWrite);
        virtual int writeChunk(const Chunk* toWrite);
        
    protected:
        // -> Constructor yay. This is protected so only the factory can use it.
        TextChunkWriter(Chunk* chnk);
        
    private:
        // -> Writes a prefix and value to the Chunk for us
        template <typename T>
        int genericWrite(const std::string& pfx, const T& thing);
        
        // -> Writes size:String to the Chunk
        int writeSizedRawString(const std::string& str);
        
        // -> So the factory works
        friend ChunkWriter* chunkrw::writerFor(Chunk* chnk);
    };
}

#endif /* defined(__NetworkingPoC__textchunkwriter__) */
