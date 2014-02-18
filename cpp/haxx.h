
#ifndef NetworkingPoC_haxx_h
#define NetworkingPoC_haxx_h

namespace net
{
    namespace haxx
    {
        template <bool b, typename Ty>
        struct _enable_if
        {};
        
        template <typename Ty>
        struct _enable_if<true, Ty>
        {
            typedef Ty type;
        };
    }
}

#endif
