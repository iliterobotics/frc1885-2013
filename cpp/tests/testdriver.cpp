
#include "testdriver.h"

#include <iostream>

#include "testcommon.h"
#include "factorytests.h"
#include "binrwtests.h"
#include "binencodingtests.h"
#include "textrwtests.h"
#include "chunktransmissiontests.h"

namespace net
{
    namespace testdriver
    {
        void runAllTests()
        {
            RUN_TEST(factorytests::runTests());
            RUN_TEST(binencodingtests::runTests());
            RUN_TEST(binrwtests::runTests());
            RUN_TEST(textrwtests::runTests());
            RUN_TEST(chunktransmissiontests::runTests());
            
            std::cout << "All testdriver tests passed." << std::endl;
        }
    }
}
