
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <algorithm>

#include <arpa/inet.h>

#include "../textchunkwriter.h"
#include "../textchunkreader.h"
#include "../binarychunkreader.h"
#include "../binarychunkwriter.h"
#include "../chunk.h"
#include "../chunkrwfactory.h"
#include "../binaryencoding.h"
#include "../tests/testdriver.h"

int main(int, const char**)
{
    net::testdriver::runAllTests();

    return 0;
}

