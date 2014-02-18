
#ifndef NetworkingPoC_chunkwriter_inl_h
#define NetworkingPoC_chunkwriter_inl_h

#include <stdexcept>

#include "chunkwriter.h"
#include "neterror.h"

namespace net
{
    inline Chunk* ChunkWriter::takeChunk()
    {
        Chunk* ret = m_chunk;
        m_chunk = NULL;
        return ret;
    }
    
    /**
     * Writes some binary data to the Chunk. This will take care of sizeof(T) for you.
     * So, this is valid:
     * int z[] = {1, 2, 3};
     * rawWriteChunk(z, 3);
     */
    template <typename T>
    int ChunkWriter::rawWriteChunk(const T* data, std::size_t dataElems)
    {
        if(m_chunk == NULL)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Already took chunk. Cannot use this writer.");
#else
            return error::BADARG;
#endif
        }
        
        if(data == NULL)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Cannot write NULL data.");
#else
            return error::BADARG;
#endif
        }
        
        const ubyte* bytez = reinterpret_cast<const ubyte*>(data);
        m_chunk->insert(m_chunk->end(), bytez, bytez + (dataElems * sizeof(T)));
        
        return error::SUCCESS;
    }
    
    /**
     * This is here as sort-of a microoptimization. We'll want to prefix data,
     * which is akin to inserting at the front of an array. If this array has 400K
     * of image data, this operation is useless, especially when we're just doing it
     * to write the prefix + 400K again. So, this is here so we:
     * A. Don't resize the Chunk twice (we guarantee enough room in one reserve())
     * B. Don't need to insert at the front of a byte array just to get said effect.
     */
    template <typename T, typename V>
    int ChunkWriter::rawWriteChunkWithPrefix(const T* prefix,
                                             std::size_t prefixElems,
                                             const V* data,
                                             std::size_t dataElems)
    {
        if(m_chunk == NULL)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Already took chunk. Cannot use this writer.");
#else
            return error::BADARG;
#endif
        }
        
        if(data == NULL || prefix == NULL)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Cannot write NULL data or prefix.");
#else
            return error::BADARG;
#endif
        }
        
        const std::size_t prefixsz = prefixElems * sizeof(T);
        const std::size_t datasz = dataElems * sizeof(V);
        const ubyte* prefixptr = reinterpret_cast<const ubyte*>(prefix);
        const ubyte* dataptr = reinterpret_cast<const ubyte*>(data);
        
        
        // Ensure a max of one resize happens...
        m_chunk->reserve(m_chunk->size() + (prefixsz + datasz));
        
        // Erryday I'm insertin'
        m_chunk->insert(m_chunk->end(), prefixptr, prefixptr + prefixsz);
        m_chunk->insert(m_chunk->end(), dataptr, dataptr + datasz);
        
        return error::SUCCESS;
    }
}

#endif
