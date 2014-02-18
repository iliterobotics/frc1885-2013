
#ifndef __NetworkingPoC__binarychunkreader__
#define __NetworkingPoC__binarychunkreader__

#include <iostream>

#include "chunkreader.h"
#include "chunkrwfactory.h"

namespace net
{
    class BinaryChunkReader: public ChunkReader
    {
    public:
        BinaryChunkReader(const Chunk* chnk);
        
        virtual ~BinaryChunkReader();
        
        virtual int nextInt(int* into);
        virtual int nextByte(byte* into);
        virtual int nextBoolean(bool* into);
        virtual int nextDouble(double* into);
        virtual int nextString(std::string* into);
        virtual int nextChunk(Chunk* into);
        
        virtual int nextType(types::Type* into) const;
        
    protected:
        BinaryChunkReader(const Chunk* chnk, const ubyte* cursor);
        
        int nextType(const ubyte** cursorAfter,
                     bool* hasSizeInfo,
                     types::Type* into) const;
        
    private:
        template <typename T>
        int consumePrimitive(types::Type asType, T* into);
        
        template <typename T>
        int consumeVariableSize(types::Type asType, T** dataStart, T** dataEnd);
        
        friend ChunkReader* chunkrw::readerFor(Chunk* chnk);
    };
}

#endif /* defined(__NetworkingPoC__binarychunkreader__) */
