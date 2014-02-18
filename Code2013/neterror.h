
#ifndef NetworkingPoC_neterrono_h
#define NetworkingPoC_neterrono_h

#include <string>

namespace net
{
    namespace error
    {
        enum Error
        {
            SUCCESS = 0,        // Nothing went wrong
            BADARG,             // Bad arguments
            HITEOF,             // Hit end of the file/stream
            NOSIZE,             // No size given by a
            NOSUFFICIENTFIELD,  // TODO:
            NOTYPEINFO,         // No type info found/given
            INVALIDTYPEINFO,    // Invalid type info found
            WRONGTYPE,          // Requesting incorrect type
            UNEXPECTEDSIZE,     // Size was given but not expected
            BADSIZE,            // Size is < 0
            EXPECTEDSIZE,       // Size was expected, but not given
            BADSEND,            // Socket send failed
            NUMBERTOOBIG,       // An overflow happened. This is an internal error.
            BADRECV,            // Socket recv failed
            UNREADABLEDATA,     // Cannot read a requested string.
            SOCKETCLOSED,       // Socket has been closed.
            FCNTLFAILED,        // fcntl failed
            BADTOTALSIZE        // Bad chunk size received
        };

        std::string stringFrom(Error e);

        inline std::string stringFrom(int i)
        {
            return stringFrom(static_cast<Error>(i));
        }
    }
}

#endif
