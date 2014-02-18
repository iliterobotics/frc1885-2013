
#ifndef NetworkingPoC_rwtestcommon_inl_h
#define NetworkingPoC_rwtestcommon_inl_h

#include "../chunkwriter.h"
#include "../chunkreader.h"

namespace net
{
    namespace rwtestcommon
    {
        void writePrimitives(ChunkWriter* to);
        void readPrimitives(ChunkReader* from);
        
        template <typename Rd, typename Wr>
        void testPrimitives()
        {
            Wr bcw;
            
            writePrimitives(&bcw);
            
            Chunk* c = bcw.takeChunk();
            
            Rd cr(c);
            
            readPrimitives(&cr);
        }
        
        template <typename Rd, typename Wr>
        void testChunks()
        {
            Wr bcw;
            
            writePrimitives(&bcw);
            
            Wr topbcw;
            
            topbcw.writeChunk(bcw.chunk());
            
            Rd topbcr(topbcw.takeChunk());
            
            Chunk rdchnk;
            
            ASSERT_SUCCESS(topbcr.nextChunk(&rdchnk));
            
            Rd bcr(&rdchnk);
            
            readPrimitives(&bcr);
            
            bcr.takeChunk();
        }
        
        template <typename Rd, typename Wr>
        void testRecursiveChunks()
        {
            Wr bcw;
            
            writePrimitives(&bcw);
            
            // Write itself to itself
            bcw.writeChunk(bcw.chunk());
            
            Rd topbcr(bcw.takeChunk());
            
            readPrimitives(&topbcr);
            
            Chunk rdchnk;
            
            ASSERT_SUCCESS(topbcr.nextChunk(&rdchnk));
            
            Rd bcr(&rdchnk);
            
            readPrimitives(&bcr);
            
            bcr.takeChunk();
        }
    }
}

#endif
