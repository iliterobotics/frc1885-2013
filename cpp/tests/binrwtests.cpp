
#include "binrwtests.h"

#include <cmath>

#include "testcommon.h"
#include "../binarychunkreader.h"
#include "../binarychunkwriter.h"
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
            rwtestcommon::testPrimitives<BinaryChunkReader, BinaryChunkWriter>();
        }
        
        inline void testChunks()
        {
            rwtestcommon::testChunks<BinaryChunkReader, BinaryChunkWriter>();
        }
        
        inline void testRecursiveChunks()
        {
            rwtestcommon::testRecursiveChunks<BinaryChunkReader, BinaryChunkWriter>();
        }
    }
    
    namespace binrwtests
    {
        void runTests()
        {
            RUN_TEST(testPrimitives());
            RUN_TEST(testChunks());
            RUN_TEST(testRecursiveChunks());
        }
    }
}
