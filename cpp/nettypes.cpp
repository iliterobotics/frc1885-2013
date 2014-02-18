
#include "nettypes.h"

namespace net
{
    namespace typenames
    {
        const char* Char = "char";
        const char* Byte = "byte";
        const char* Integer = "int";
        const char* Boolean = "bool";
        const char* Double = "double";
        const char* String = "str";
        const char* ChunkObject = "chunk";
    }
    
    namespace types
    {
        bool stringFor(Type t, std::string* outstr)
        {
            outstr->clear();
            
            switch(t)
            {
                case Char: outstr->assign(typenames::Char); break;
                case Byte: outstr->assign(typenames::Byte); break;
                case Integer: outstr->assign(typenames::Integer); break;
                case Boolean: outstr->assign(typenames::Boolean); break;
                case Double: outstr->assign(typenames::Double); break;
                case String: outstr->assign(typenames::String); break;
                case ChunkObject: outstr->assign(typenames::ChunkObject); break;
                case Unreadable: outstr->assign("UNREADABLE"); break;
                case TypeMask: outstr->assign("TypeMask"); break;
            }
            
            return !outstr->empty();
        }
    }
}
