
#ifndef NetworkingPoC_nettypes_h
#define NetworkingPoC_nettypes_h

#include <string>
#include <cfloat>

namespace net
{
    // Definitions of fixed-width types, because you can never have enough
    // of those! Also, in case we have to deal with a non-POSIX compliant
    // platform (Windows), these may come in handy.
    typedef uint64_t uint64;
    typedef int64_t int64;
    typedef uint32_t uint32;
    typedef int32_t int32;
    typedef uint16_t uint16;
    typedef int16_t int16;
    typedef float float32;
    typedef double float64;
    typedef uint8_t bool8;
    typedef uint8_t ubyte;
    typedef int8_t byte;
    
    // Contains string names for each type.
    // Generally used in the TextChunkReader/Writer
    namespace typenames
    {
        extern const char* Char;
        extern const char* Byte;
        extern const char* Integer;
        extern const char* Boolean;
        extern const char* Double;
        extern const char* String;
        extern const char* ChunkObject;
    }
    
    namespace types
    {
        // Number values for each type that can be transmitted over the wire.
        enum Type
        {
            Byte = 0,          // 8 bit unsigned byte
            Boolean = 1,       // 8 bit unsigned byte (autoconverted to 1 or 0)
            Char = 2,          // 8 bit unsigned byte (syntactic sugar?)
            ChunkObject = 3,   // Nested chunk
            Double = 4,        // 64-bit IEEE-754 compliant floating point number
            Integer = 5,       // 32-bit two's compliment integer
            String = 6,        // variable-length array of chars
            Unreadable = 0xFE, // what we return when something goes wrong
            TypeMask = 0xFF    // mask for lolz
        };
        
        // -> Returns a string to represent the given type into the
        // given parameter. Will return false/give empty string on no match.
        bool stringFor(Type t, std::string* outstr);
        
        // Less efficient than the above, but here for convenience.
        inline std::string stringFor(Type t)
        {
            std::string s;
            stringFor(t, &s);
            return s;
        }
    }
}

#endif
