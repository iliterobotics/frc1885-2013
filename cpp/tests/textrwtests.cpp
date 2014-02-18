
#include "textrwtests.h"

#include "testcommon.h"
#include "../textchunkreader.h"
#include "../textchunkwriter.h"
#include "rwtestcommon.h"

namespace net
{
    namespace
    {
        // Workaround because RUN_TEST(testPrimitives<X, Y>()) is interpreted
        // as calling RUN_TEST(a, b) because of the ,. I'll make better macros
        // at some point in the future :p
        inline void testPrimitives()
        {
            rwtestcommon::testPrimitives<TextChunkReader, TextChunkWriter>();
        }
        
        inline void testChunks()
        {
            rwtestcommon::testChunks<TextChunkReader, TextChunkWriter>();
        }
        
        inline void testRecursiveChunks()
        {
            rwtestcommon::testRecursiveChunks<TextChunkReader, TextChunkWriter>();
        }
    }
    
    namespace textrwtests
    {
        void runTests()
        {
            RUN_TEST(testPrimitives());
            RUN_TEST(testChunks());
            RUN_TEST(testRecursiveChunks());
        }
    }
}
