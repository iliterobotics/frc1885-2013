
#ifndef __NetworkingPoC__textchunkreader__
#define __NetworkingPoC__textchunkreader__

#include <iostream>

#include "chunk.h"
#include "chunkreader.h"
#include "chunkrwfactory.h"

namespace net
{
    class TextChunkReader: public ChunkReader
    {
    public:
        TextChunkReader(const Chunk* chnk);
        
        virtual ~TextChunkReader();
                
        virtual int nextInt(int* into);
        virtual int nextByte(byte* into);
        virtual int nextBoolean(bool* into);
        virtual int nextDouble(double* into);
        virtual int nextString(std::string* into);
        virtual int nextChunk(Chunk* into);
        
        virtual int nextType(types::Type* into) const;
        
    protected:
        TextChunkReader(const Chunk* chnk, const ubyte* cursor);
        
        int nextType(const ubyte** cursorAfter,
                     std::size_t* sizeAfter,
                     types::Type* into) const;
        
    private:
        template <typename T>
        int nextSomething(types::Type expected, T* into);
        
        template <typename T>
        int nextComplexSomething(types::Type expected, T* outvar);
        
        void advanceCursor(const ubyte* min);
        
        friend ChunkReader* chunkrw::readerFor(Chunk* chnk);
    };
}

#endif /* defined(__NetworkingPoC__textchunkreader__) */
