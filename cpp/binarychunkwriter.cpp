
#include "binarychunkwriter.h"

#include <stdexcept>
#include <sstream>

#include "rwcommon.h"
#include "binaryencoding.h"
#include "common.h"

namespace net
{
    BinaryChunkWriter::BinaryChunkWriter()
    {
        rawWriteChunk(rwcommon::BIN_CHUNK_ID, rwcommon::arrsize(rwcommon::BIN_CHUNK_ID));
    }
    
    BinaryChunkWriter::BinaryChunkWriter(Chunk* chnk):
        ChunkWriter(chnk)
    {
        if(chnk == NULL || chnk->empty())
        {
            rawWriteChunk(rwcommon::BIN_CHUNK_ID, rwcommon::arrsize(rwcommon::BIN_CHUNK_ID));
        }
        else if(!rwcommon::isBinChunk(chnk))
        {
            throw std::runtime_error("Trying to create binwriter from incompatible chunk type.");
            // constructor throw
        }
    }

    BinaryChunkWriter::~BinaryChunkWriter() {}

    int BinaryChunkWriter::writeInt(int32 toWrite)
    {
        return writePrimitive(types::Integer, toWrite);
    }

    int BinaryChunkWriter::writeByte(byte toWrite)
    {
        return writePrimitive(types::Byte, toWrite);
    }
    
    int BinaryChunkWriter::writeDouble(float64 toWrite)
    {
        std::ostringstream ostr;
        
        ostr << toWrite;
        
        return writeContainer(types::Double, ostr.str());
    }

    int BinaryChunkWriter::writeBoolean(bool8 toWrite)
    {
        return writePrimitive(types::Boolean, toWrite);
    }
    
    int BinaryChunkWriter::writeString(const std::string* toWrite)
    {
        if(!toWrite)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("bad args");
#else
            return error::BADARG;
#endif
        }
        return writeContainer(types::String, *toWrite);
    }
    
    int BinaryChunkWriter::writeChunk(const Chunk* toWrite)
    {
        if(!toWrite)
        {
#ifdef NET_USE_EXCEPTIONS
            throw std::runtime_error("bad args");
#else
            return error::BADARG;
#endif
        }
        return writeContainer(types::ChunkObject, *toWrite);
    }
    
    template <typename T>
    int BinaryChunkWriter::writePrimitive(types::Type ty, const T& val)
    {
        const binencoding::EncodedType t =
        binencoding::toNetByteOrder(binencoding::typeToBytes(ty, false));
        const T val2 = binencoding::toNetByteOrder(val);
        return rawWriteChunkWithPrefix(&t, 1, &val2, 1);
    }
    
    // hackish thing so we can get the ContType for writeContainer
    template <class Cont>
    inline int BinaryChunkWriter::writeContainer(types::Type ty, const Cont& val)
    {
        return writeContainer(ty, val, val[0]);
    }
    
    /**
     * Writes a container for us. For the moment, we have no support for writing containers with multibyte
     * internal values, because doing so would require a decent amount of effort. Additionally, there is
     * no need for it at the moment. If there should be a need for it in the future, we can hack it in.
     */
    template <class Cont, typename ContType>
    typename haxx::_enable_if<sizeof(ContType) == 1, int>::type
    BinaryChunkWriter::writeContainer(types::Type ty, const Cont& val, const ContType&)
    {
        const binencoding::EncodedType t = binencoding::toNetByteOrder(binencoding::typeToBytes(ty, true));
        
        // Be sure to turn
        const binencoding::SizeType preSize = static_cast<binencoding::SizeType>(val.size() * sizeof(ContType));
        const binencoding::SizeType nboSize = binencoding::toNetByteOrder(preSize);
        
        // Size is taken prior to writing information because if we're writing to ourself, then taking the size
        // later causes a nasty bug where the chunk info is written twice (once at the end).
        const std::size_t valSize = val.size();
        
        int err = error::SUCCESS;
        
        if((err = rawWriteChunkWithPrefix(&t, 1, &nboSize, 1)))
        {
            return err;
        }
        
        return rawWriteChunk(&val[0], valSize);
    }
}
