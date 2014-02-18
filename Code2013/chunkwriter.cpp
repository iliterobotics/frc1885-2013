
#include "chunkwriter.h"

namespace net
{
    ChunkWriter::ChunkWriter():
        m_chunk(new Chunk())
    {}
    
    ChunkWriter::ChunkWriter(Chunk* chnk):
        m_chunk(chnk)
    {
        // if NULL was passed in, ignore it.
        if(m_chunk == NULL)
        {
            m_chunk = new Chunk();
        }
    }
    
    ChunkWriter::~ChunkWriter() {
        if(m_chunk)
        {
            delete m_chunk;
        }
    }
}
