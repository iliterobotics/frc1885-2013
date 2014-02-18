
#ifndef __NetworkingPoC__chunkreader__
#define __NetworkingPoC__chunkreader__

#include <string>

#include "chunk.h"
#include "nettypes.h"

namespace net
{
    class ChunkReader
    {
    public:
        ChunkReader(const Chunk* chnk);
        ChunkReader(const Chunk* chnk, const ubyte* cursor);
        
        virtual ~ChunkReader();
        
        inline const Chunk* chunk() { return m_chunk; }
        const Chunk* takeChunk();
        
        std::string dumpAsString() const;
        
        virtual bool atEnd() const;
        
        virtual int nextType(types::Type* into) const = 0;
        
        virtual int nextInt(int* into) = 0;
        virtual int nextByte(byte* into) = 0;
        virtual int nextBoolean(bool* into) = 0;
        virtual int nextDouble(double* into) = 0;
        virtual int nextString(std::string* into) = 0;
        virtual int nextChunk(Chunk* into) = 0;
        
    protected:
        inline const ubyte* cursor() const { return m_cursor; }
        inline const ubyte* cursor() { return m_cursor; }
        inline const ubyte* cursorMax() const { return m_chunk->size() + &m_chunk->operator[](0); }
        inline const ubyte* cursorStart() const { return &m_chunk->front(); }
        inline std::size_t bytesRemaining() const { return static_cast<int>(cursorMax() - cursor()); }
        inline std::size_t chunkSize() const { return m_chunk->size(); }
        inline bool cursorOutOfBounds(const ubyte* at) const { return cursorMax() <= at; }
        
        bool putCursor(const ubyte* at);
        void forceCursor(const ubyte* at);
        
    private:
        const Chunk* m_chunk;
        const ubyte* m_cursor;
    };
}

#endif /* defined(__NetworkingPoC__chunkreader__) */
