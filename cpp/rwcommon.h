
#ifndef __NetworkingPoC__rwcommon__
#define __NetworkingPoC__rwcommon__

#include <string>

#include "chunk.h"

namespace net
{
    namespace rwcommon
    {
        static const std::size_t ID_MAX_SIZE = 4ul;
        static const char BIN_CHUNK_ID[] = "BIN";
        static const char TXT_CHUNK_ID[] = "TXT";
        
        template <std::size_t N>
        inline std::size_t arrsize(const char (&)[N]) { return N; }
        
        inline bool isBinChunk(const Chunk* ch);
        inline bool isTextChunk(const Chunk* ch);
    }
}

#include "rwcommon-inl.h"

#endif /* defined(__NetworkingPoC__rwcommon__) */
