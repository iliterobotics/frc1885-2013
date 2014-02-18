
#include "factorytests.h"

#include "testcommon.h"
#include "../chunkrwfactory.h"
#include "../chunkreader.h"
#include "../chunkwriter.h"
#include "../textchunkreader.h"
#include "../textchunkwriter.h"
#include "../binarychunkreader.h"
#include "../binarychunkwriter.h"

namespace net
{
    namespace factorytests
    {
        namespace
        {
            void testReaderFor()
            {
                // Testing binary writer is matched with binary reader
                BinaryChunkWriter wr;
                
                ChunkReader* rd = chunkrw::readerFor(wr.takeChunk());
                
                ASSERT_NOT_NULL(rd);
                ASSERT_NOT_NULL(dynamic_cast<BinaryChunkReader*>(rd));
                
                delete rd;
                
                // Testing text writer matched up with text reader
                TextChunkWriter tcw;
                
                rd = chunkrw::readerFor(tcw.takeChunk());
                
                ASSERT_NOT_NULL(rd);
                ASSERT_NOT_NULL(dynamic_cast<TextChunkReader*>(rd));
                
                delete rd;
                
                // Testing that, given no (or invalid) input, we get NULL back.
                Chunk basic;
                
                ASSERT_NULL(chunkrw::readerFor(&basic));
                
                basic.assign(30, '\n');
                
                ASSERT_NULL(chunkrw::readerFor(&basic));
                
                ASSERT_NULL(chunkrw::readerFor(NULL));
            }
            
            void testPreferredWriter()
            {
                ChunkWriter* c = chunkrw::preferredWriter();
                
                ASSERT_NOT_NULL(c);
                
                delete c;
            }
            
            void testWriterFor()
            {
                // Checking to see if, given a basic blank chunk,
                // the factory gives us the default writer.
                Chunk basic;
                
                ChunkWriter* cw = NULL;
                
                ASSERT_NOTHROW(cw = chunkrw::writerFor(&basic));
                
                ASSERT_NOT_NULL(cw);
                
                cw->takeChunk();
                
                ChunkWriter* pw = chunkrw::preferredWriter();
                
                // I know it will leak here, but OS will clean up because this calls
                // terminate() directly. So it's not a real problem.
                ASSERT_NOT_NULL(pw);
                
                // Hackish way to test if cw and pw are the same type.
                // If we add more writers/etc, amend this to do it properly.
                bool cwBin = dynamic_cast<BinaryChunkWriter*>(cw) == NULL;
                bool pwBin = dynamic_cast<BinaryChunkWriter*>(pw) == NULL;
                
                ASSERT_EQ(cwBin, pwBin);
                
                delete cw;
                delete pw;
                
                // Now testing if, given a chunk init'ed by a Binary writer,
                // the chosen writer is Binary
                BinaryChunkWriter wr;
                
                Chunk* wrchk = wr.takeChunk();
                
                ChunkWriter* chw = chunkrw::writerFor(wrchk);
                
                ASSERT_NOT_NULL(dynamic_cast<BinaryChunkWriter*>(chw));
                
                delete chw;
                
                // Now testing the same for text writers
                TextChunkWriter twr;
                
                wrchk = twr.takeChunk();
                
                chw = chunkrw::writerFor(wrchk);
                
                ASSERT_NOT_NULL(dynamic_cast<TextChunkWriter*>(chw));
                
                delete chw;
            }
        }
        
        void runTests()
        {
            RUN_TEST(testReaderFor());
            RUN_TEST(testPreferredWriter());
            RUN_TEST(testWriterFor());
        }
    }
}
