
#ifndef NetworkingPoC_receiver_h
#define NetworkingPoC_receiver_h

#include <memory>
#include <vector>

#include "nettypes.h"

namespace net
{
    // Chunks are the base unit of everything.
    typedef std::vector<ubyte> Chunk;
    
    // In case we should require it for whatever reason...
    typedef ubyte ChunkStorageUnit;
}

#endif
