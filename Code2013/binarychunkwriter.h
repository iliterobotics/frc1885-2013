
#ifndef __NetworkingPoC__binarychunkwriter__
#define __NetworkingPoC__binarychunkwriter__

#include "chunkwriter.h"
#include "chunkrwfactory.h"
#include "haxx.h"

namespace net
{
    class BinaryChunkWriter: public ChunkWriter
    {
    public:
        BinaryChunkWriter();
        
        virtual ~BinaryChunkWriter();
        
        // -> Overrides of ChunkWriter.
        virtual int writeInt(int32 toWrite);
        virtual int writeByte(byte toWrite);
        virtual int writeDouble(float64 toWrite);
        virtual int writeBoolean(bool8 toWrite);
        virtual int writeString(const std::string* toWrite);
        virtual int writeChunk(const Chunk* toWrite);
        
    protected:
        // -> Protected for factory use only
        BinaryChunkWriter(Chunk* chnk);
        
    private:
        // -> Writes a primitive value of a given type to the Chunk
        template <typename T>
        int writePrimitive(types::Type ty, const T& val);
        
        // -> Calls writeContainer(types::Type, const Cont&, const ContType&)
        template <class Cont>
        inline int writeContainer(types::Type ty, const Cont& val);
        
        // -> Writes a container for us. Yay!
        template <class Cont, typename ContType>
        typename haxx::_enable_if<sizeof(ContType) == 1, int>::type
        writeContainer(types::Type ty, const Cont& val, const ContType&);
        
        friend ChunkWriter* chunkrw::writerFor(Chunk* chnk);
    };
}

#endif /* defined(__NetworkingPoC__binarychunkwriter__) */
