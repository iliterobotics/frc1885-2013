
#ifndef __NetworkingPoC__rwtestcommon__
#define __NetworkingPoC__rwtestcommon__

namespace net
{
    namespace rwtestcommon
    {
        template <typename Rd, typename Wr>
        void testPrimitives();
        
        template <typename Rd, typename Wr>
        void testChunks();
        
        template <typename Rd, typename Wr>
        void testRecursiveChunks();
    }
}

#include "rwtestcommon-inl.h"

#endif /* defined(__NetworkingPoC__rwtestcommon__) */
