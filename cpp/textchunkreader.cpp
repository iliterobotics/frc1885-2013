
#include "textchunkreader.h"

#include <map>
#include <cstring>
#include <sstream>
#include <cctype>
#include <stdexcept>

#include "neterror.h"
#include "rwcommon.h"

namespace net
{
    namespace
    {
        // Micro-optimization:
        // Singleton string->type map so we don't have to reconstruct it every time
        // we need it.
        const std::map<std::string, types::Type>& strTypeMap()
        {
            static std::map<std::string, types::Type> result;
            
            if(result.empty())
            {
                result[typenames::Boolean] = types::Boolean;
                result[typenames::Byte] = types::Byte;
                result[typenames::Char] = types::Char;
                result[typenames::ChunkObject] = types::ChunkObject;
                result[typenames::Double] = types::Double;
                result[typenames::Integer] = types::Integer;
                result[typenames::String] = types::String;
            }
            
            return result;
        }
        
        // type-string map
        const std::map<types::Type, std::string>& typeStrMap()
        {
            static std::map<types::Type, std::string> result;
            
            if(result.empty())
            {
                result[types::Boolean] = typenames::Boolean;
                result[types::Byte] = typenames::Byte;
                result[types::Char] = typenames::Char;
                result[types::ChunkObject] = typenames::ChunkObject;
                result[types::Double] = typenames::Double;
                result[types::Integer] = typenames::Integer;
                result[types::String] = typenames::String;
            }
            
            return result;
        }
        
        int sizeOfPart(const ubyte* cursor, const ubyte* max)
        {
            // no max null check because if max <= cursor and cursor != null, max != null.
            if(max <= cursor || cursor == NULL)
            {
                return -1;
            }
            
            std::istringstream ss(std::string(cursor, max));
            
            int amt = 0;
        
            if(!(ss >> amt))
            {
                return -1;
            }
            
            return amt;
        }
        
        inline const ubyte* nextField(const ubyte* cursor, const ubyte* end)
        {
            const ubyte* ub = reinterpret_cast<const ubyte*>(memchr(cursor, ':', static_cast<std::size_t>(end - cursor)));
            
            return ub;
        }
    }
    
    TextChunkReader::TextChunkReader(const Chunk* chnk):
        ChunkReader(chnk)
    {
        if(!rwcommon::isTextChunk(chnk))
        {
            throw std::runtime_error("Trying to create textwriter from incompatible chunk type.");
            // no return because constructor.
        }
        
        const ubyte* targ = cursor() + sizeof(char) * rwcommon::arrsize(rwcommon::TXT_CHUNK_ID);
        
        forceCursor(targ);
    }
    
    TextChunkReader::TextChunkReader(const Chunk* chnk, const ubyte* curs):
        ChunkReader(chnk, curs)
    {
        if(!rwcommon::isTextChunk(chnk))
        {
            throw std::runtime_error("Trying to create textwriter from incompatible chunk type.");
            // no return because constructor.
        }
        
        const ubyte* targ = cursor() + sizeof(char) * rwcommon::arrsize(rwcommon::TXT_CHUNK_ID);
        
        if(cursor() < targ)
        {
            forceCursor(targ);
        }
    }

    TextChunkReader::~TextChunkReader() {}

    int TextChunkReader::nextType(const ubyte** cursorAfter,
                                  std::size_t* sizeAfter,
                                  types::Type* into) const
    {
        if(!into)
        {
            return error::BADARG;
        }
        
        // Init cursorAfter and sizeAfter if necessary.
        if(cursorAfter)
        {
            *cursorAfter = NULL;
        }
        
        if(sizeAfter)
        {
            *sizeAfter = 0;
        }
        
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }
        
        // At the moment, we have "13:double:30.1:some-other-number:...:"
        const int partSize = sizeOfPart(cursor(), cursorMax());
        
        // + 1 to advance passed the :.
        const ubyte* const cursorFirst = nextField(cursor(), cursorMax()) + 1;
        
        if(partSize < 0)
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ostr;
            
            ostr << "TextChunkReader: Size not found in chunk " << dumpAsString()
                 << " at char " << static_cast<std::size_t>(cursor() - cursorStart()); 
            
            throw std::runtime_error(ostr.str().c_str());
#else
            return error::NOSIZE;
#endif
        }
        
        // Highest available cursor value
        const ubyte* const maxCurs = partSize + cursorFirst;
        
        const ubyte* at = cursorFirst;
        
        if(at == NULL)
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ostr;
            
            ostr << "TextChunkReader: No field after size in chunk " << dumpAsString()
                 << " at char " << static_cast<std::size_t>(cursor() - cursorStart());
            
            throw std::runtime_error(ostr.str().c_str());
#else
            return error::NOSUFFICIENTFIELD;
#endif
        }
        
        // Now, we're at the type!
        std::string typeStr;
        
        // Iterate through, adding to our string until we hit the delim.
        for(; at < maxCurs && *at != ':'; ++at)
        {
            typeStr += tolower(static_cast<char>(*at));
        }
        
        // Did we go to the end?
        if(at + 1 >= maxCurs)
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ostr;
            
            ostr << "TextChunkReader: No type information delim found for " << dumpAsString()
                 << " at char " << static_cast<std::size_t>(cursor() - cursorStart());
            
            throw std::runtime_error(ostr.str().c_str());
#else
            return error::NOTYPEINFO;
#endif
        }
        
        // ++at advances it passed the ':'
        ++at;
        
        // Now we (supposedly) have the type string. Time to store off cursorAfter/sizeAfter if needed.
        if(cursorAfter)
        {
            *cursorAfter = at;
        }
        
        if(sizeAfter)
        {
            // Size of the entire part - how far it starts from the cursor.
            *sizeAfter = partSize - (at - (cursorFirst - 1));
        }
        
        // Cool! Look for it...
        const std::map<std::string, types::Type>& ourMap = strTypeMap();
        std::map<std::string, types::Type>::const_iterator it = ourMap.find(typeStr);
        
        // Not found :(
        if(it == ourMap.end())
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ostr;
            
            ostr << "TextChunkReader: Invalid type information found for " << dumpAsString()
                 << " at char " << static_cast<std::size_t>(cursor() - cursorStart());
            
            throw std::runtime_error(ostr.str().c_str());
#else
            return error::INVALIDTYPEINFO;
#endif
        }
        
        *into = it->second;
        
        return error::SUCCESS;
    }
    
    int TextChunkReader::nextType(types::Type* into) const
    {
        return nextType(NULL, NULL, into);
    }

    int TextChunkReader::nextInt(int* into)
    {
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }
        
        return nextSomething<int>(types::Integer, into);
    }

    int TextChunkReader::nextByte(byte* into)
    {
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }

        *into = 0;

        int iptr;
        int result = nextSomething(types::Byte, &iptr);
        
        if(!result)
        {
            *into = iptr;
        }

        return result;
    }
    
    int TextChunkReader::nextBoolean(bool* into)
    {
        if(!into)
        {
            return error::BADARG;
        }
        
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }
        
        *into = false;
        
        // we use int here for template because we want to guarantee the bool
        // is treated as a number, and "true/false" are not parsed accidentally.
        int r = 0;
        int e = 0;
        
        if((e = nextSomething<int>(types::Boolean, &r)))
        {
            return e;
        }
        
        *into = !!r;
        
        return error::SUCCESS;
    }
    
    int TextChunkReader::nextDouble(double* into)
    {
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }
        
        return nextSomething<double>(types::Double, into);
    }
    
    int TextChunkReader::nextString(std::string* into)
    {
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }
        
        return nextComplexSomething(types::String, into);
    }
    
    int TextChunkReader::nextChunk(Chunk* into)
    {
        if(atEnd())
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Trying to read after end of chunk");
#else
            return error::HITEOF;
#endif
        }
        
        return nextComplexSomething(types::ChunkObject, into);
    }
    
    template <typename T>
    int TextChunkReader::nextSomething(types::Type expected, T* into)
    {
        if(!into)
        {
            return error::BADARG;
        }
        
        *into = T();
        
        const ubyte* curs = NULL;
        std::size_t size = 0;
        
        types::Type datatype;
        
        int e = nextType(&curs, &size, &datatype);
        
        if(e)
        {
            return e;
        }
        
        if(datatype != expected)
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ss;
            
            ss << "Tried to extract " << types::stringFor(expected)
               << " from " << dumpAsString() << "."
               << " Actual type is " << types::stringFor(datatype);
            
            throw std::runtime_error(ss.str());
#else
            return error::WRONGTYPE;
#endif
        }
        
        std::istringstream sstr(std::string(reinterpret_cast<const char*>(curs), size));
        
        T result;
        
        // if sstr >> result fails, it will sometimes put junk into result.
        // so that's why we have a temporary.
        if(!(sstr >> result))
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ss;
            
            ss << "Tried to extract " << types::stringFor(expected)
               << " from " << dumpAsString() << "."
               << " ...But it wasn't an " << types::stringFor(expected)
               << ". So someone lied. "
               << "What a horrible person.";
            
            throw std::runtime_error(ss.str());
#else
            return error::INVALIDTYPEINFO;
#endif
        }
        
        advanceCursor(curs + size);
        
        *into = result;
        
        return error::SUCCESS;
    }
    
    template <typename T>
    int TextChunkReader::nextComplexSomething(types::Type expected, T* outvar)
    {
        if(outvar == NULL)
        {
            // Shouldn't be triggered because this is a private method, but you never know.
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Y U PASS TEXTCHUNKREADER::NEXTCOMPLEXSOMETHING NULL OUTVAR");
#else
            return error::BADARG;
#endif
        }
        
        const ubyte* curs = NULL;
        std::size_t size = 0;
        
        types::Type datatype;
        
        int e = nextType(&curs, &size, &datatype);
        
        if(e)
        {
            return e;
        }
        
        if(datatype != expected)
        {
#ifdef NET_USE_EXCEPTIONS
            std::ostringstream ss;
            
            ss << "Tried to extract " << types::stringFor(expected)
            << " from " << dumpAsString() << "."
            << " Actual type is " << types::stringFor(datatype);
            
            throw std::runtime_error(ss.str());
#else
            return error::WRONGTYPE;
#endif
        }
        
        advanceCursor(curs + size);
        
        // assuming outvar supports assign and end. size - 1 because size will bring us to the final
        // ':' of the statement. We don't want to include that in our output.
        outvar->assign(curs, curs + size - 1);
        
        return error::SUCCESS;
    }
    
    // This is here because there are times where the serializer will place multiple consecutive
    // :'s. So, we will run through all of them until we find a char that is not a ':', or the end
    // of the stream.
    void TextChunkReader::advanceCursor(const ubyte* min)
    {
        for(; *min == ':' && min < cursorMax(); ++min);
        
        forceCursor(min);
    }
}
