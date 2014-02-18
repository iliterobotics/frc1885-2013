
#include "textchunkwriter.h"

#include <sstream>

#include "rwcommon.h"

namespace net {
    TextChunkWriter::TextChunkWriter() {
        rawWriteChunk(rwcommon::TXT_CHUNK_ID, rwcommon::arrsize(rwcommon::TXT_CHUNK_ID));
    }
    
    TextChunkWriter::TextChunkWriter(Chunk* chnk):
        ChunkWriter(chnk)
    {
        if(chnk == NULL || chnk->empty())
        {
            rawWriteChunk(rwcommon::TXT_CHUNK_ID, rwcommon::arrsize(rwcommon::TXT_CHUNK_ID));
        }
        else if(!rwcommon::isTextChunk(chnk))
        {
            throw std::runtime_error("Trying to create textwriter from incompatible chunk type.");
            // constructor throw.
        }
    }
    
    TextChunkWriter::~TextChunkWriter() {}
    
    int TextChunkWriter::writeInt(int32 toWrite)
    {
        return genericWrite(typenames::Integer, toWrite);
    }

    int TextChunkWriter::writeByte(byte toWrite)
    {
        // cast to int because we want it to display as a number,
        // and int8_t does not always show up as such
        return genericWrite(typenames::Byte, static_cast<int>(toWrite));
    }
    
    int TextChunkWriter::writeDouble(float64 toWrite)
    {
        return genericWrite(typenames::Double, toWrite);
    }
    
    int TextChunkWriter::writeBoolean(bool8 toWrite)
    {
        return genericWrite(typenames::Boolean, (bool)toWrite);
    }
    
    int TextChunkWriter::writeString(const std::string* toWrite)
    {
        if(!toWrite)
        {
            return error::BADARG;
        }
        
        return genericWrite(typenames::String, *toWrite);
    }
    
    int TextChunkWriter::writeChunk(const Chunk* toWrite)
    {
        if(!toWrite)
        {
            return error::BADARG;
        }
        
        std::string fstr(typenames::ChunkObject);
        fstr += ':';
        fstr.append(toWrite->begin(), toWrite->end());
        fstr += ':';
        
        return writeSizedRawString(fstr);
    }
    
    // I'm lazy.
    template <typename T>
    int TextChunkWriter::genericWrite(const std::string& pfx, const T& thing)
    {
        std::ostringstream ss;
        
        ss << thing << ':';
        
        std::string toWrite(pfx + ":" + ss.str());
        
        return writeSizedRawString(toWrite);
    }
    
    int TextChunkWriter::writeSizedRawString(const std::string& str)
    {
        std::ostringstream ss;
        
        ss << str.size() + 1 << ':';
        
        std::string ostr = ss.str();
        
        return rawWriteChunkWithPrefix(&ostr[0], ostr.size(), &str[0], str.size());
    }
}
