
#include "binencodingtests.h"

#include <string>
#include <iostream>
#include <vector>

#include "testcommon.h"
#include "../binaryencoding.h"
#include "../common.h"

namespace net
{
    namespace binencodingtests
    {
        namespace
        {
            template <typename T>
            union ByteOrderTester
            {
                T val;
                char bytes[sizeof(T)];
                
                ByteOrderTester()
                {
                    val = T();
                }
                
                ByteOrderTester(T v)
                {
                    val = v;
                }
            };
            
#if NET_USE_LITTLE_ENDIAN
            template <typename T>
            ByteOrderTester<T> byteOrderXform(const ByteOrderTester<T>& a)
            {
                ByteOrderTester<T> n;
                
                for(unsigned int i = 0; i < sizeof(T); ++i)
                {
                    n.bytes[sizeof(T) - 1 - i] = a.bytes[i];
                }
                
                return n;
            }
#elif NET_USE_BIG_ENDIAN
            template <typename T>
            ByteOrderTester<T> byteOrderXform(const ByteOrderTester<T>& a)
            {
                return a;
            }
#else
#error "Can't determine byte order"
#endif
            
            template <typename T>
            T doubleEncode(T a)
            {
                return binencoding::toHostByteOrder(binencoding::toNetByteOrder((a)));
            }
            
            void testByteOrderConversions()
            {
                const bool8 tst8 = 0x12;
                const uint16 tst16 = 0x1234;
                const uint32 tst32 = 0x12345678;
                const uint64 tst64 = 0x1234567890ABCDEFLL;
                const float32 tstf = 1.9876543210987654321;
                const float64 tstd = 1.9876543210987654321;
                
                using binencoding::toHostByteOrder;
                using binencoding::toNetByteOrder;
                
                // Test toHostByteOrder to ensure all versions give expected results
                ASSERT_EQ(toHostByteOrder(tst8), byteOrderXform(ByteOrderTester<bool8>(tst8)).val);
                ASSERT_EQ(toHostByteOrder(tst16), byteOrderXform(ByteOrderTester<uint16>(tst16)).val);
                ASSERT_EQ(toHostByteOrder(tst32), byteOrderXform(ByteOrderTester<uint32>(tst32)).val);
                ASSERT_EQ(toHostByteOrder(tst64), byteOrderXform(ByteOrderTester<uint64>(tst64)).val);
                ASSERT_EQ(toHostByteOrder(tstf), byteOrderXform(ByteOrderTester<float32>(tstf)).val);
                ASSERT_EQ(toHostByteOrder(tstd), byteOrderXform(ByteOrderTester<float64>(tstd)).val);
                
                // Test toNetByteOrder to ensure all versions give expected results
                ASSERT_EQ(toNetByteOrder(tst8), byteOrderXform(ByteOrderTester<bool8>(tst8)).val);
                ASSERT_EQ(toNetByteOrder(tst16), byteOrderXform(ByteOrderTester<uint16>(tst16)).val);
                ASSERT_EQ(toNetByteOrder(tst32), byteOrderXform(ByteOrderTester<uint32>(tst32)).val);
                ASSERT_EQ(toNetByteOrder(tst64), byteOrderXform(ByteOrderTester<uint64>(tst64)).val);
                ASSERT_EQ(toNetByteOrder(tstf), byteOrderXform(ByteOrderTester<float32>(tstf)).val);
                ASSERT_EQ(toNetByteOrder(tstd), byteOrderXform(ByteOrderTester<float64>(tstd)).val);
                
                // Test toNetByteOrder + toHostByteOrder gives back the initial value
                ASSERT_EQ(doubleEncode(tst8), tst8);
                ASSERT_EQ(doubleEncode(tst16), tst16);
                ASSERT_EQ(doubleEncode(tst32), tst32);
                ASSERT_EQ(doubleEncode(tst64), tst64);
                ASSERT_EQ(doubleEncode(tstf), tstf);
                ASSERT_EQ(doubleEncode(tstd), tstd);
            }
            
            void testTypeConversions()
            {
                using binencoding::bytesToType;
                using binencoding::typeToBytes;
                
                std::vector<types::Type> vec;
                
                vec.push_back(types::Boolean);
                vec.push_back(types::Byte);
                vec.push_back(types::Char);
                vec.push_back(types::ChunkObject);
                vec.push_back(types::Double);
                vec.push_back(types::Integer);
                vec.push_back(types::String);
                
                bool size = false;
                binencoding::EncodedType type = 0;
                
                for(std::vector<types::Type>::const_iterator it = vec.begin();
                    it != vec.end();
                    ++it)
                {
                    type = typeToBytes(*it, true);
                    ASSERT_EQ(bytesToType(type), *it);
                    
                    ASSERT_EQ(bytesToType(type, &size), *it);
                    ASSERT_TRUE(size);
                    
                    type = typeToBytes(*it, false);
                    ASSERT_EQ(bytesToType(type), *it);
                    
                    ASSERT_EQ(bytesToType(type, &size), *it);
                    ASSERT_FALSE(size);
                }
            }
        }
        
        void runTests()
        {
            RUN_TEST(testByteOrderConversions());
            RUN_TEST(testTypeConversions());
        }
    }
}
