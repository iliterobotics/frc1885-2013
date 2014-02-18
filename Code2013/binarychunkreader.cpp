
#include "binarychunkreader.h"

#include <sstream>
#include <stdexcept>
#include <assert.h>

#include "binaryencoding.h"
#include "neterror.h"
#include "rwcommon.h"

namespace net
{
    BinaryChunkReader::BinaryChunkReader(const Chunk* chnk):
        ChunkReader(chnk)
    {
        if(!rwcommon::isBinChunk(chnk))
        {
            throw std::runtime_error("Trying to create textwriter from incompatible chunk type.");
            // constructor throw.
        }
        
        forceCursor(cursor() + sizeof(char) * rwcommon::arrsize(rwcommon::BIN_CHUNK_ID));
    }
    
    BinaryChunkReader::BinaryChunkReader(const Chunk* chnk, const ubyte* curs):
        ChunkReader(chnk, curs)
    {
        if(!rwcommon::isBinChunk(chnk))
        {
            throw std::runtime_error("Trying to create textwriter from incompatible chunk type.");
            // constructor throw.
        }
        
        const ubyte* targ = cursor() + sizeof(char) * rwcommon::arrsize(rwcommon::BIN_CHUNK_ID);
        
        if(cursor() < targ)
        {
            forceCursor(targ);
        }
    }

    BinaryChunkReader::~BinaryChunkReader() {}
    
    int BinaryChunkReader::nextInt(int* into)
    {
        return consumePrimitive(types::Integer, into);
    }
    
    int BinaryChunkReader::nextByte(byte* into)
    {
        return consumePrimitive(types::Byte, into);
    }

    int BinaryChunkReader::nextBoolean(bool* into)
    {
        return consumePrimitive(types::Boolean, into);
    }
    
    int BinaryChunkReader::nextDouble(double* into)
    {
        const ubyte* dataStart = NULL;
        const ubyte* dataEnd = NULL;
        
        int e = 0;
        
        if((e = consumeVariableSize(types::Double, &dataStart, &dataEnd)))
        {
            return e;
        }
        
        assert(dataStart != NULL && dataEnd != NULL);
        
        std::istringstream strm(std::string(dataStart, dataEnd));
        
        double res = 0.0;
        
        if(!(strm >> res))
        {
            return error::UNREADABLEDATA;
        }
        
        *into = res;
        
        return error::SUCCESS;
    }
    
    int BinaryChunkReader::nextString(std::string* into)
    {
        // duplicated in nextChunk(). Refactor if we need to copy again.
        const ubyte* dataStart = NULL;
        const ubyte* dataEnd = NULL;
        
        int e = 0;
        
        if((e = consumeVariableSize(types::String, &dataStart, &dataEnd)))
        {
            return e;
        }
        
        assert(dataStart != NULL && dataEnd != NULL);
        
        into->assign(dataStart, dataEnd);
        
        return error::SUCCESS;
    }
    
    int BinaryChunkReader::nextChunk(Chunk* into)
    {
        // copied from nextString(). Refactor if we need to copy again.
        const ubyte* dataStart = NULL;
        const ubyte* dataEnd = NULL;
        
        int e = 0;
        
        if((e = consumeVariableSize(types::ChunkObject, &dataStart, &dataEnd)))
        {
            return e;
        }
        
        assert(dataStart != NULL && dataEnd != NULL);
        
        into->assign(dataStart, dataEnd);
        
        return error::SUCCESS;
    }
    
    int BinaryChunkReader::nextType(types::Type* t) const
    {
        return nextType(NULL, NULL, t);
    }

    int BinaryChunkReader::nextType(const ubyte** cursorAfter,
                                    bool* hasSizeInfo,
                                    types::Type* into) const
    {
        // to make typeInfo line shorter.
        using binencoding::EncodedType;
        
        if(!into)
        {
            return error::BADARG;
        }
        
        *into = types::Unreadable;
        
        // Init params in case of error
        if(cursorAfter)
        {
            *cursorAfter = NULL;
        }
        
        if(hasSizeInfo)
        {
            *hasSizeInfo = false;
        }
        
        // Structure is [aa][d+]
        // where a is one byte of type info, and d+ is multiple bytes of whatever.
        if(bytesRemaining() < sizeof(EncodedType))
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Not enough bytes remaining to read type.");
#else
            return error::NOTYPEINFO;
#endif
        }
        
        const EncodedType* const typeInfo = reinterpret_cast<const EncodedType*>(cursor());
        const EncodedType theType = binencoding::toHostByteOrder(*typeInfo);
        types::Type ret;
        
        if(hasSizeInfo)
        {
            ret = binencoding::bytesToType(theType, hasSizeInfo);
        }
        else
        {
            ret = binencoding::bytesToType(theType);
        }
        
        if(ret > types::String)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Out of bounds type");
#else
            return error::INVALIDTYPEINFO;
#endif
        }
        
        // set cursorAfter and sizeAfter if necessary.
        if(cursorAfter)
        {
            *cursorAfter = reinterpret_cast<const ubyte*>(typeInfo + 1);
        }
        
        *into = ret;
        
        return error::SUCCESS;
    }
    
    template <typename T>
    int BinaryChunkReader::consumePrimitive(types::Type asType, T* into)
    {
        const ubyte* nxtCursor = NULL;
        bool hazSize = false;
        
        if(!into)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("badarg");
#else
            return error::BADARG;
#endif
        }
        
        *into = T();
        
        types::Type t;
        int e = nextType(&nxtCursor, &hazSize, &t);
        
        if(e)
        {
            return e;
        }
        
        // Quick type check
        if(t != asType)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Requested type != actual type");
#else
            return error::WRONGTYPE;
#endif
        }
        
        if(hazSize)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Size is not supported in primitive types (yet?)");
#else
            return error::UNEXPECTEDSIZE;
#endif
        }
        
        // Primitives don't have size attached atm, so we can just assume
        // the next n bytes are the primitive.
        
        // if we don't have enough data left, complain
        if(cursorMax() < nxtCursor + sizeof(T))
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Not enough bytes left :(");
#else
            return error::NOSUFFICIENTFIELD;
#endif
        }
        
        // this is here in hopes that nxtCursor + sizeof(T) from above will
        // already be in a register. and it won't mess with our function's
        // work anyway.
        forceCursor(nxtCursor + sizeof(T));
        
        // Copy the value.
        *into = binencoding::toHostByteOrder(*reinterpret_cast<const T*>(nxtCursor));
        
        return error::SUCCESS;
    }
    
    template <typename T>
    int BinaryChunkReader::consumeVariableSize(types::Type asType, T** dataStart, T** dataEnd)
    {
        if(!dataStart || !dataEnd)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("bad arg");
#else
            return error::BADARG;
#endif
        }
        
        *dataEnd = NULL;
        *dataStart = NULL;
        
        const ubyte* nxtCursor = NULL;
        bool hazSize = false;
        
        types::Type t;
        int e = nextType(&nxtCursor, &hazSize, &t);
        
        if(e)
        {
            return e;
        }
        
        // Quick type check
        if(t != asType)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Requested type != actual type");
#else
            return error::WRONGTYPE;
#endif
        }
        
        if(!hazSize)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Need size for variable-size things.");
#else
            return error::EXPECTEDSIZE;
#endif
        }
        
        // if we don't have enough data left, complain
        if(cursorMax() < (nxtCursor + sizeof(binencoding::SizeType)))
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Not enough bytes left to read size :(");
#else
            return error::NOSIZE;
#endif
        }
        
        // Read dat size
        const binencoding::SizeType sz =
        binencoding::toHostByteOrder(*reinterpret_cast<const binencoding::SizeType*>(nxtCursor));
        
        nxtCursor += sizeof(binencoding::SizeType);
        
        const ubyte* const endAt = nxtCursor + sizeof(T) * sz;
        
        // if we don't have enough data left, complain
        if(cursorMax() < endAt)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("Not enough bytes left to read object :(");
#else
            return error::BADSIZE;
#endif
        }
        
        // this is here in hopes that nxtCursor + sizeof(T) from above will
        // already be in a register. and it won't mess with our function's
        // work anyway.
        forceCursor(endAt);
        
        *dataStart = nxtCursor;
        *dataEnd = endAt;
        
        return error::SUCCESS;
    }
}
