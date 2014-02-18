
#ifndef __NetworkingPoC__chunkrwfactory__
#define __NetworkingPoC__chunkrwfactory__

#include <iostream>

#include "chunkwriter.h"
#include "chunkreader.h"

namespace net
{
    namespace chunkrw
    {
        ChunkWriter* writerFor(Chunk* chnk);
        ChunkWriter* preferredWriter();
        
        ChunkReader* readerFor(Chunk* chnk);
    }
}

#endif /* defined(__NetworkingPoC__chunkrwfactory__) */
