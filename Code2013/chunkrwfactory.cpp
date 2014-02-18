
#include "chunkrwfactory.h"
#include "textchunkreader.h"
#include "textchunkwriter.h"
#include "binarychunkreader.h"
#include "binarychunkwriter.h"
#include "rwcommon.h"
#include <stdexcept>

namespace net
{
    namespace chunkrw
    {
        ChunkWriter* writerFor(Chunk* chnk)
        {
            if(chnk == NULL || chnk->empty())
            {
                return preferredWriter();
            }
            else if(rwcommon::isBinChunk(chnk))
            {
                return new BinaryChunkWriter(chnk);
            }
            else if(rwcommon::isTextChunk(chnk))
            {
                return new TextChunkWriter(chnk);
            }
            
            return NULL;
        }
        
        ChunkWriter* preferredWriter()
        {
            // TODO: This should say something like
            // if debug:
            //   return TextChunkWriter()
            // else:
            //   return BinaryChunkWriter()
            return new BinaryChunkWriter();
        }

        ChunkReader* readerFor(Chunk* chnk)
        {
            if(rwcommon::isBinChunk(chnk))
            {
                return new BinaryChunkReader(chnk);
            }
            else if(rwcommon::isTextChunk(chnk))
            {
                return new TextChunkReader(chnk);
            }
            
            return NULL;
        }
    }
}
