//
//  chunktransmission.h
//  NetworkingPoC
//
//  Created by George Burgess IV on 2/2/13.
//  Copyright (c) 2013 George Burgess IV. All rights reserved.
//

#ifndef __NetworkingPoC__chunktransmission__
#define __NetworkingPoC__chunktransmission__

#include "chunk.h"
#include <string>

namespace net
{
    namespace chunktransmission
    {
        int writeChunk(int sock, const Chunk* toWrite, bool blocking = true);

        int readChunk(int sock, Chunk** result, bool blocking = true, Chunk** persistChunk = NULL, std::size_t* numRead = NULL);
    }
}

#endif /* defined(__NetworkingPoC__chunktransmission__) */
