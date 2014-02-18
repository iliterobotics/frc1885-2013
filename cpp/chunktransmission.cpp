//
//  chunktransmission.cpp
//  NetworkingPoC
//
//  Created by George Burgess IV on 2/2/13.
//  Copyright (c) 2013 George Burgess IV. All rights reserved.
//

#include "chunktransmission.h"

#include <sys/socket.h>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "neterror.h"
#include "binaryencoding.h"

namespace net
{
    namespace chunktransmission
    {
        const int SEND_FLAGS = 0x0;
        const int RECV_FLAGS = 0x0;

        namespace
        {
            int setNonBlock(int sock, int* wasBlock)
            {
                int fl = fcntl(sock, F_GETFL);

                if(fl == -1)
                {
                    return 1;
                }

                if(wasBlock)
                {
                    *wasBlock = !(fl & O_NONBLOCK);
                }

                // if already nonblocking, don't set it.
                if(fl & O_NONBLOCK)
                {
                    return 0;
                }

                return fcntl(sock, F_SETFL, fl | O_NONBLOCK) == -1;
            }

            int setBlock(int sock, int* wasNonBlock)
            {
                int fl = fcntl(sock, F_GETFL);

                if(fl == -1)
                {
                    return 1;
                }

                if(wasNonBlock)
                {
                    *wasNonBlock = fl & O_NONBLOCK;
                }

                // if not nonblock, just return.
                if(!(fl & O_NONBLOCK))
                {
                    return 0;
                }

                return fcntl(sock, F_SETFL, fl & ~O_NONBLOCK) == -1;
            }

            int sendAll(int sock, const ubyte* bytes, std::size_t size)
            {
                ssize_t retval = 0;
                ssize_t bsent = 0;

                while(bsent < size)
                {
                    retval = send(sock, reinterpret_cast<const char*>(bytes + bsent), size - bsent, SEND_FLAGS);

                    if(retval < 0)
                    {
                        return error::BADSEND;
                    }

                    bsent += retval;
                }

                return 0;
            }

            int recvAll(int sock, ubyte* byteArr, std::size_t arrSize, std::size_t numBytes)
            {
                if(byteArr == NULL)
                {
                    return error::BADARG;
                }

                const std::size_t bytesToRead = (numBytes > arrSize) ? arrSize : numBytes;
                std::size_t bytesRead = 0;

                while(bytesRead < bytesToRead)
                {
                    ssize_t res = read(sock, reinterpret_cast<char*>(byteArr + bytesRead), bytesToRead - bytesRead);

							if(res == -1 && errno == EAGAIN)
                     {
								return error::NODATA;
							}

						  if(res == -1)
                    {
                        return error::BADRECV;
                    }

                    bytesRead += res;
                }

                return error::SUCCESS;
            }

            inline int recvAll(int sock, ubyte* byteArr, std::size_t arrSize)
            {
                return recvAll(sock, byteArr, arrSize, arrSize);
            }
        }

        int writeChunk(int sock, const Chunk* toWrite, bool blocking)
        {
            int resetBlocking;
            int err = error::SUCCESS;

            if(toWrite == NULL)
            {
                return error::BADARG;
            }

            if(blocking)
            {
                if(setBlock(sock, &resetBlocking))
                {
                    return error::FCNTLFAILED;
                }
            }
            else
            {
                if(setNonBlock(sock, &resetBlocking))
                {
                    return error::FCNTLFAILED;
                }
            }

            const std::size_t cSize = toWrite->size() * sizeof(ChunkStorageUnit);

            const uint32 chunkSize = static_cast<uint32>(cSize);

            const uint32 flippedChunkSize = binencoding::toNetByteOrder(chunkSize);

            // if overflow, quit.
            if(chunkSize != cSize)
            {
                err = error::NUMBERTOOBIG;
                goto cleanup;
            }

            if(sendAll(sock, reinterpret_cast<const ubyte*>(&flippedChunkSize), sizeof(uint32)))
            {
                err = (errno == EPIPE) ? error::SOCKETCLOSED : error::BADSEND;
                goto cleanup;
            }

            if(sendAll(sock, reinterpret_cast<const ubyte*>(&toWrite->front()), chunkSize))
            {
                err = (errno == EPIPE) ? error::SOCKETCLOSED : error::BADSEND;
                goto cleanup;
            }

        cleanup:
            if(resetBlocking)
            {
                if(blocking)
                {
                    // Only overwrite error if error is otherwise fine...
                    if(setNonBlock(sock, NULL) && err == error::SUCCESS)
                    {
                        err = error::FCNTLFAILED;
                    }
                }
                else
                {
                    if(setBlock(sock, NULL) && err == error::SUCCESS)
                    {
                        err = error::FCNTLFAILED;
                    }
                }
            }

            return err;
        }

        int readChunk(int sock, Chunk** result, bool blocking)
        {
            int resetBlocking;
            int err = error::SUCCESS;

            if(result == NULL)
            {
                return error::BADARG;
            }

            if(blocking)
            {
                if(setBlock(sock, &resetBlocking))
                {
                    return error::FCNTLFAILED;
                }
            }
            else
            {
                if(setNonBlock(sock, &resetBlocking))
                {
                    return error::FCNTLFAILED;
                }
            }

            *result = NULL;

            uint32 size = 0;
            Chunk* chunkres = NULL;

            if((err = recvAll(sock, reinterpret_cast<ubyte*>(&size), sizeof(uint32))))
            {
                err = (errno == EPIPE) ? error::SOCKETCLOSED : err;
                goto cleanup;
            }

            size = binencoding::toHostByteOrder(size);
            std::cout << "Size: " << size << std::endl
               << "Err: " << err << std::endl;

            if(size > 20 * 1024)
            {
                return error::BADTOTALSIZE;
            }

            chunkres = new Chunk();
            chunkres->resize(size / sizeof(ChunkStorageUnit));

            // Note: Size does not need to be multiplied by sizeof(ChunkStorageUnit) because
            // it is received, and should have already had that transformation applied.
            if((err = recvAll(sock, reinterpret_cast<ubyte*>(chunkres->data()), size)))
            {
                err = (errno == EPIPE) ? error::SOCKETCLOSED : err;
                goto cleanup;
            }

            *result = chunkres;

        cleanup:
            if(resetBlocking)
            {
                if(blocking)
                {
                    if(setNonBlock(sock, NULL) && err == error::SUCCESS)
                    {
                        return error::FCNTLFAILED;
                    }
                }
                else
                {
                    if(setBlock(sock, NULL) && err == error::SUCCESS)
                    {
                        return error::FCNTLFAILED;
                    }
                }
            }

            if(chunkres != NULL
               && *result != chunkres
               && persistChunk != NULL
               && *persistChunk != chunkres)
            {
                delete chunkres;
            }

            return err;
        }
    }
}
