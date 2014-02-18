
#ifndef NetworkingPoC_binaryencoding_inl_h
#define NetworkingPoC_binaryencoding_inl_h

#include <arpa/inet.h>

#include "binaryencoding.h"

#include "common.h"

namespace net
{
    namespace binencoding
    {
        /**
         * Convert a type identifier(Integer, Double, Byte), etc to its representation
         * in bytes.
         */
        inline EncodedType typeToBytes(types::Type t, bool8 sizeIncluded)
        {
            EncodedType result = !!sizeIncluded;
            
            result = (result << SIZE_INCLUDED_SHL) | static_cast<int32>(t);
            
            return result;
        }
        
        /**
         * Converts a binary encoded type to a Type. Ignores size information.
         */
        inline types::Type bytesToType(EncodedType binType)
        {
            return static_cast<types::Type>(binType & types::TypeMask);
        }
        
        /**
         * Converts an EncodedType to a normal types::Type. Tells if size was included
         * as well.
         */
        inline types::Type bytesToType(EncodedType binType, bool* sizeIncluded)
        {
            if(sizeIncluded)
            {
                *sizeIncluded = !!(binType & SIZE_INCLUDED_MASK);
            }
            
            return bytesToType(binType);
        }
        
        // Here be dragons. This is the insanely ugly code that deals with byte order.
        // And floating point byte order. And hacks along those lines. Additionally,
        // I (ab)use templates to make it go vroom vroom at the cost of compile time.
        // Proceed with caution.
        
        // Also, interesting part's first. The rest is just repetitive.
        
        template <typename From, typename To>
        inline To convertRaw(From from)
        {
            return *reinterpret_cast<To*>(&from);
        }
        
#if NET_USE_LITTLE_ENDIAN        
        // little endian == not NBO == do stuff
        template <typename T>
        inline T toNetByteOrder(T arg)
        {
            switch(sizeof(T))
            {
                case 1: return arg;
                case 2: return ntohs(arg);
                case 4: return ntohl(arg);
                case 8:
                {
                    const int64 conv = convertRaw<T, int64>(arg);
                    
                    return convertRaw<int64, T>(((conv & 0xFFL)               << 56) |
                                                ((conv & 0xFF00L)             << 40) |
                                                ((conv & 0xFF0000L)           << 24) |
                                                ((conv & 0xFF000000L)         << 8)  |
                                                ((conv & 0xFF00000000LL)       >> 8)  |
                                                ((conv & 0xFF0000000000LL)     >> 24) |
                                                ((conv & 0xFF000000000000LL)   >> 40) |
                                                ((conv & 0xFF00000000000000LL) >> 56));
                }
            }
        }
        
        template <typename T>
        inline T toHostByteOrder(T arg)
        {
            // Flip it.
            return toNetByteOrder(arg);
        }
#elif NET_USE_BIG_ENDIAN
        // big endian == NBO == do nothing.
        template <typename T>
        inline typename std::enable_if<sizeof(T) == 8, T>::type
        toNetByteOrder(T arg)
        {
            return arg;
        }
        
        template <typename T>
        inline typename std::enable_if<sizeof(T) == 8, T>::type
        toHostByteOrder(T arg)
        {
            return arg;
        }
#else
#error "Can't determine endianness."
#endif
        
        template <>
        inline float32 toNetByteOrder<float32>(float32 arg)
        {
            return convertRaw<uint32, float32>(toNetByteOrder(convertRaw<float32, uint32>(arg)));
        }
        
        template <>
        inline float32 toHostByteOrder<float32>(float32 arg)
        {
            return convertRaw<uint32, float32>(toHostByteOrder(convertRaw<float32, uint32>(arg)));
        }
    }
}


#endif
