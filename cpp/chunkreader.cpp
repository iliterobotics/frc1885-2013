
#include "chunkreader.h"

namespace net
{
    ChunkReader::ChunkReader(const Chunk* chnk):
        m_chunk(chnk)
    {
        m_cursor = &chnk->front();
    }
    
    ChunkReader::ChunkReader(const Chunk* chnk, const ubyte* cursor):
        m_chunk(chnk)
    {
        if(!m_chunk)
        {
            m_chunk = new Chunk();
            m_cursor = &m_chunk->front();
        }
        else
        {
            putCursor(cursor);
        }
    }
    
    ChunkReader::~ChunkReader()
    {
        if(m_chunk)
        {
            delete m_chunk;
        }
    }
    
    bool ChunkReader::atEnd() const
    {
        return m_cursor + 1 >= cursorMax();
    }
    
    std::string ChunkReader::dumpAsString() const
    {
        const char* fst = reinterpret_cast<const char*>(cursorStart());
        const char* snd = reinterpret_cast<const char*>(cursorMax());
        return std::string(fst, snd);
    }
    
    const Chunk* ChunkReader::takeChunk()
    {
        const Chunk* ret = m_chunk;
        m_chunk = NULL;
        m_cursor = NULL;
        return ret;
    }
    
    bool ChunkReader::putCursor(const ubyte* at)
    {
        if(at == NULL)
        {
            return false;
        }
        else if(at < cursorStart() || cursorOutOfBounds(at))
        {
            return false;
        }
        
        m_cursor = at;
        
        return true;
    }
    
    void ChunkReader::forceCursor(const ubyte* at)
    {
        m_cursor = at;
    }
}