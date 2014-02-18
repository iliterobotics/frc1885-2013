#ifndef NetworkingPoC_binaryencoding_h
#define NetworkingPoC_binaryencoding_h

#include "nettypes.h"

namespace net
{
    namespace binencoding
    {
        typedef uint16 EncodedType;
        typedef uint32 SizeType;
        
        const int SIZE_INCLUDED_SHL = 15;
        const EncodedType SIZE_INCLUDED_MASK = 1 << SIZE_INCLUDED_SHL;
        
        // -> Converts a type + whether size was included to an encoded type.
        inline EncodedType typeToBytes(types::Type t, bool8 sizeIncluded);
        
        // -> Converts binary type to types::Type. Ignores size.
        inline types::Type bytesToType(EncodedType binType);
        
        // -> Converts binary type to types::Type and whether or not size was included.
        inline types::Type bytesToType(EncodedType binType, bool* sizeIncluded);
        
        // Template functions that convert a given type to NBO
        template <typename T>
        inline T toNetByteOrder(T arg);
        
        // Template functions that convert a given type to host byte order
        template <typename T>
        inline T toHostByteOrder(T arg);
    }
}

#include "binaryencoding-inl.h"

#endif
