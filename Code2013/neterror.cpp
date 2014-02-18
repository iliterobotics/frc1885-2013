
#include "neterror.h"
namespace net
{
    namespace error
    {
        // i so lazii
        #define AUTOGEN_CASE(__x) case __x: return std::string(#__x)
        std::string stringFrom(Error e)
        {
            switch(e)
            {
                AUTOGEN_CASE(SUCCESS);
                AUTOGEN_CASE(BADARG);
                AUTOGEN_CASE(HITEOF);
                AUTOGEN_CASE(NOSIZE);
                AUTOGEN_CASE(NOSUFFICIENTFIELD);
                AUTOGEN_CASE(NOTYPEINFO);
                AUTOGEN_CASE(INVALIDTYPEINFO);
                AUTOGEN_CASE(WRONGTYPE);
                AUTOGEN_CASE(UNEXPECTEDSIZE);
                AUTOGEN_CASE(BADSIZE);
                AUTOGEN_CASE(EXPECTEDSIZE);
                AUTOGEN_CASE(BADSEND);
                AUTOGEN_CASE(NUMBERTOOBIG);
                AUTOGEN_CASE(BADRECV);
                AUTOGEN_CASE(UNREADABLEDATA);
                AUTOGEN_CASE(SOCKETCLOSED);
                AUTOGEN_CASE(FCNTLFAILED);
                AUTOGEN_CASE(BADTOTALSIZE);
                default: return std::string("UNKNOWN ERROR");
            }
        }
        #undef AUTOGEN_CASE
    }
}
