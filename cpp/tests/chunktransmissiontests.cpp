//
//  chunktransmissiontests.cpp
//  NetworkingPoC
//
//  Created by George Burgess IV on 2/2/13.
//  Copyright (c) 2013 George Burgess IV. All rights reserved.
//

#include "chunktransmissiontests.h"

#define _XOPEN_SOURCE_EXTENDED 1
#include <sys/socket.h>
#include <unistd.h>
#include <string>

#include "../chunktransmission.h"
#include "../binarychunkreader.h"
#include "../binarychunkwriter.h"
#include "../chunkrwfactory.h"
#include "testcommon.h"

namespace net
{
    namespace
    {
        void testBasic()
        {
            const ubyte MAGIC_CROSS_CONSTANT = 0x84;
            int sockz[2];
            
            // I realize these won't be closed under A LOT of circumstances.
            // But this is a unit test, and the OS should handle cleanup for us.
            // So it doesn't matter.
            ASSERT_GEQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sockz), 0);
            
            Chunk ch;
            
            ch.push_back(MAGIC_CROSS_CONSTANT);
            
            ASSERT_SUCCESS(chunktransmission::writeChunk(sockz[1], &ch));
            
            Chunk* chptr;
            
            ASSERT_SUCCESS(chunktransmission::readChunk(sockz[0], &chptr));
            
            ASSERT_NOT_NULL(chptr);
            
            ASSERT_EQ(chptr->front(), MAGIC_CROSS_CONSTANT);
            
            delete chptr;
            
            // OK, OCD over not closing them got the best of me.
            close(sockz[0]);
            close(sockz[1]);
        }
        
        void testBreaking()
        {
            const int32 WRITTEN_INT = 0x12345678;
            const float64 WRITTEN_FLOAT = 3.14159;
            const bool8 WRITTEN_BOOL = true;
            const std::string WRITTEN_STRING("Hello, World");
            
            int sockz[2];
            
            ASSERT_GEQ(socketpair(AF_UNIX, SOCK_STREAM, 0, sockz), 0);
            
            BinaryChunkWriter bcw;
            
            ASSERT_SUCCESS(bcw.writeInt(WRITTEN_INT));
            ASSERT_SUCCESS(bcw.writeDouble(WRITTEN_FLOAT));
            ASSERT_SUCCESS(bcw.writeBoolean(WRITTEN_BOOL));
            ASSERT_SUCCESS(bcw.writeString(&WRITTEN_STRING));
            
            ASSERT_SUCCESS(chunktransmission::writeChunk(sockz[1], bcw.chunk()));
            
            Chunk* chptr = NULL;
            
            ASSERT_SUCCESS(chunktransmission::readChunk(sockz[0], &chptr));
            
            ASSERT_NOT_NULL(chptr);
            
            BinaryChunkReader bcr(chptr);
            
            int32 i = 0;
            ASSERT_SUCCESS(bcr.nextInt(&i));
            ASSERT_EQ(i, WRITTEN_INT);
            
            float64 f = 0;
            ASSERT_SUCCESS(bcr.nextDouble(&f));
            ASSERT_EQ(f, WRITTEN_FLOAT);
            
            bool b = false;
            ASSERT_SUCCESS(bcr.nextBoolean(&b));
            ASSERT_EQ(b, WRITTEN_BOOL);
            
            std::string s;
            ASSERT_SUCCESS(bcr.nextString(&s));
            ASSERT_EQ(s, WRITTEN_STRING);
            
            close(sockz[0]);
            close(sockz[1]);
        }
    }
    
    namespace chunktransmissiontests
    {
        void runTests()
        {
            RUN_TEST(testBasic());
            RUN_TEST(testBreaking());
        }
    }
}
