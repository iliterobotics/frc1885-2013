
#ifndef NetworkingPoC_test_common_h
#define NetworkingPoC_test_common_h

#include <stdexcept>

#include "../neterror.h"

#define RUN_TEST(__method) \
    do { \
        std::cout << "Running test method: " #__method << std::endl; \
        __method; \
    } while(0)

#define TEST_ASSERT_STR(__cond, __str) \
    do { \
        try { \
            ::net::test::assert(__cond, __str, __LINE__, __FILE__); \
        } catch(...) { \
            ::net::test::assert(false, \
                                "UNEXPECTED EXCEPTION FROM " #__str, \
                                __LINE__, \
                                __FILE__); \
        } \
    } while(0)

#define TEST_ASSERT(__cond) TEST_ASSERT_STR(__cond, #__cond)

#define ASSERT_STR1(__name, __arg) #__name "(" #__arg ")"
#define ASSERT_STR2(__name, __arg1, __arg2) #__name "(" #__arg1 ", " #__arg2 ")"

#define _SUCCESS_VAL ::net::error::SUCCESS

#define ASSERT_EQ(__a, __b) TEST_ASSERT_STR(__a == __b, ASSERT_STR2(ASSERT_EQ, __a, __b))
#define ASSERT_NEQ(__a, __b) TEST_ASSERT_STR(__a != __b, ASSERT_STR2(ASSERT_NEQ, __a, __b))
#define ASSERT_LT(__a, __b) TEST_ASSERT_STR(__a < __b, ASSERT_STR2(ASSERT_LT, __a, __b))
#define ASSERT_GT(__a, __b) TEST_ASSERT_STR(__a > __b, ASSERT_STR2(ASSERT_GT, __a, __b))
#define ASSERT_LEQ(__a, __b) TEST_ASSERT_STR(__a <= __b, ASSERT_STR2(ASSERT_LEQ, __a, __b))
#define ASSERT_GEQ(__a, __b) TEST_ASSERT_STR(__a >= __b, ASSERT_STR2(ASSERT_GEQ, __a, __b))

#define ASSERT_SUCCESS(__a) TEST_ASSERT_STR(__a == _SUCCESS_VAL, ASSERT_STR1(ASSERT_SUCCESS, __a))
#define ASSERT_FAILURE(__a) TEST_ASSERT_STR(__a != _SUCCESS_VAL, ASSERT_STR1(ASSERT_FAILURE, __a))

#define ASSERT_NULL(__a) TEST_ASSERT_STR(__a == NULL, ASSERT_STR1(ASSERT_NULL, __a))
#define ASSERT_NOT_NULL(__a) TEST_ASSERT_STR(__a != NULL, ASSERT_STR1(ASSERT_NOT_NULL, __a))

#define ASSERT_TRUE(__a) TEST_ASSERT_STR(__a, ASSERT_STR1(ASSERT_TRUE, __a))
#define ASSERT_FALSE(__a) TEST_ASSERT_STR(!(__a), ASSERT_STR1(ASSERT_FALSE, __a))

#define ASSERT_NOTHROW(__a) \
    do { \
        try { \
            __a; \
        } catch(...) { \
            TEST_ASSERT_STR(false, ASSERT_STR1(ASSERT_NOTHROW, __a)); \
        } \
    } while(0)

#define ASSERT_THROWS(__a, __e) \
    do { \
        try { \
            __a; \
        } catch(__e) { \
            break; \
        } catch(...) { \
            TEST_ASSERT_STR(false, ASSERT_STR2(ASSERT_THROWS, __a, __e)); \
        } \
        TEST_ASSERT_STR(false, ASSERT_STR2(ASSERT_THROWS, __a, __e)); \
    } while(0)

namespace net
{
    namespace test
    {
        void assert(bool cond,
                    const char* ltxt,
                    std::size_t line,
                    const char* file);
    }
}

#endif
