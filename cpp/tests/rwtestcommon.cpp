
#include "rwtestcommon.h"

#include <cmath>

#include "testcommon.h"

namespace net
{
    namespace rwtestcommon
    {
        const int CTL_INT1 = 1024;
        const int CTL_INT2 = 9826;
        const char* const CTL_STR1 = "hello, world";
        const byte CTL_BYTE1 = 0x38;
        const byte CTL_BYTE2 = -0x19;
        const char* const CTL_STR2 = "w0rld, hai";
        const bool CTL_BOOL1 = false;
        const bool CTL_BOOL2 = true;
        // Pi, then random numbers when I forgot what numbers came next.
        const double CTL_DOUBLE1 = 3.141592684731974;
        const double CTL_DOUBLE2 = 91284.38438297123;
        
        inline bool equalWithinTolerance(double d1, double d2)
        {
            // So long as our doubles are 99.99% accurate, I think we're good.
            return fabs(d1 - d2) <= fabs(0.0001 * d1);
        }
        
        void writePrimitives(ChunkWriter* to)
        {
            ASSERT_SUCCESS(to->writeInt(CTL_INT1));
            ASSERT_SUCCESS(to->writeByte(CTL_BYTE1));
            ASSERT_SUCCESS(to->writeDouble(CTL_DOUBLE1));
            ASSERT_SUCCESS(to->writeBoolean(CTL_BOOL1));
            ASSERT_SUCCESS(to->writeString(CTL_STR1));
            ASSERT_SUCCESS(to->writeString(CTL_STR2));
            ASSERT_SUCCESS(to->writeBoolean(CTL_BOOL2));
            ASSERT_SUCCESS(to->writeDouble(CTL_DOUBLE2));
            ASSERT_SUCCESS(to->writeByte(CTL_BYTE2));
            ASSERT_SUCCESS(to->writeInt(CTL_INT2));
        }
        
        void readPrimitives(ChunkReader* from)
        {
            int i;
            double d;
            bool b;
            byte ub;
            std::string s;
            
            ASSERT_SUCCESS(from->nextInt(&i));
            ASSERT_EQ(i, CTL_INT1);
            
            ASSERT_SUCCESS(from->nextByte(&ub));
            ASSERT_EQ(ub, CTL_BYTE1);
            
            ASSERT_SUCCESS(from->nextDouble(&d));
            ASSERT_TRUE(equalWithinTolerance(d, CTL_DOUBLE1));
            
            ASSERT_SUCCESS(from->nextBoolean(&b));
            ASSERT_EQ(b, CTL_BOOL1);
            
            ASSERT_SUCCESS(from->nextString(&s));
            ASSERT_EQ(s, CTL_STR1);
            
            ASSERT_SUCCESS(from->nextString(&s));
            ASSERT_EQ(s, CTL_STR2);
            
            ASSERT_SUCCESS(from->nextBoolean(&b));
            ASSERT_EQ(b, CTL_BOOL2);
            
            ASSERT_SUCCESS(from->nextDouble(&d));
            ASSERT_TRUE(equalWithinTolerance(d, CTL_DOUBLE2));
            
            ASSERT_SUCCESS(from->nextByte(&ub));
            ASSERT_EQ(ub, CTL_BYTE2);
            
            ASSERT_SUCCESS(from->nextInt(&i));
            ASSERT_EQ(i, CTL_INT2);
        }
    }
}
