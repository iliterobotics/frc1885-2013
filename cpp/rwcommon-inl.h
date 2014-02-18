
#ifndef NetworkingPoC_rwcommon_inl_h
#define NetworkingPoC_rwcommon_inl_h

#include "rwcommon.h"
#include "haxx.h"

namespace net
{
    namespace rwcommon
    {
        // If the compiler starts complaining about this function, be SURE that
        // the array you're trying to use with it is a MAX of ID_MAX_SIZE characters.
        // Including the '\0'. i.e. "Hi" is a 3 character array. Because it's
        // {'H', 'i', '\0'}. This does checking for size at compile time. So you -will-
        // get compiler errors for trying to feed it an array that's too big.
        template <std::size_t N>
        inline typename haxx::_enable_if<N <= ID_MAX_SIZE, bool>::type
        idEqual(const Chunk* ch, const char (&arr)[N])
        {
            if(ch == NULL || ch->size() < N)
            {
                return false;
            }
            
            for(std::size_t i = 0; i < N; ++i)
            {
                if(ch->operator[](i) != arr[i])
                {
                    return false;
                }
            }
            
            return true;
        }
        
        inline bool isBinChunk(const Chunk* ch)
        {
            return idEqual(ch, BIN_CHUNK_ID);
        }
        
        inline bool isTextChunk(const Chunk* ch)
        {
            return idEqual(ch, TXT_CHUNK_ID);
        }
    }
}

#endif
