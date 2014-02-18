
#ifndef __NetworkingPoC__ChunkWriter__
#define __NetworkingPoC__ChunkWriter__

#include <iostream>
#include <memory>

#include "chunk.h"

namespace net
{
    class ChunkWriter
    {
    public:
        // -> Default constructor. Makes a chunk for the class to use on its own
        ChunkWriter();
        
        // -> Virtual destructor. Kills given chunk if necessary
        virtual ~ChunkWriter();
        
        // -> Requests an int be written to the underlying Chunk
        virtual int writeInt(int32 toWrite) = 0;
        
        // -> Requests a byte be written to the underlying Chunk
        virtual int writeByte(byte toWrite) = 0;

        // -> Requests a double be written to the underlying Chunk
        virtual int writeDouble(float64 toWrite) = 0;
        
        // -> Requests boolean
        virtual int writeBoolean(bool8 toWrite) = 0;
        
        // -> String.
        virtual int writeString(const std::string* toWrite) = 0;
        
        // -> Requests a chunk be nested into the current chunk.
        virtual int writeChunk(const Chunk* toWrite) = 0;
        
        // -> Convenience methods
        inline int writeString(const std::string& str) { return writeString(&str); }
        inline int writeChunk(const Chunk& toWrite) { return writeChunk(&toWrite); }
        
        // -> Returns the Chunk we're working on. Will not release ownership.
        inline const Chunk* chunk() const { return m_chunk; }
        
        // -> Returns the Chunk we're working on. Releases ownership. Do not use
        // this instance after calling this method.
        inline Chunk* takeChunk();
        
        // -> Convenience: Gives back the size of the chunk currently
        inline std::size_t chunkSize() const { return m_chunk->size(); }
        
    protected:
        // -> Pass in a chunk, this will append to it.
        ChunkWriter(Chunk* chnk);
        
        // -> Writes an element/array of elements to the underlying Chunk.
        template <typename T>
        int rawWriteChunk(const T* data, std::size_t dataElems);
        
        // -> Writes two elements/arrays of elements to the underlying Chunk.
        template <typename T, typename V>
        int rawWriteChunkWithPrefix(const T* prefix,
                                     std::size_t prefixElems,
                                     const V* data,
                                     std::size_t dataElems);
        
    private:
        Chunk* m_chunk;
    };
}

#include "chunkwriter-inl.h"

#endif /* defined(__NetworkingPoC__ChunkWriter__) */
