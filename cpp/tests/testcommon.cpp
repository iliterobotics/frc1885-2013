
#include "testcommon.h"

#include <iostream>
#include <sstream>
#include <exception>
#include <errno.h>
#include <string.h>

namespace net
{
    namespace test
    {
        void assert(bool cond,
                    const char* ltxt,
                    std::size_t line,
                    const char* file)
        {
            if(!cond)
            {
                std::ostringstream oss;
                oss << "Assertion failed: " << ltxt << " @ line " << line << " in " << file <<
                " (errno: " << strerror(errno) << " [0x" << std::hex << errno << "])";
                std::cerr << oss.str() << std::endl;
                std::cerr.flush();
                std::cout.flush();
                std::terminate();
            }
        }
    }
}
