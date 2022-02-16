// ===========================================================================
// Ranges04.cpp
// ===========================================================================

// Motivation

#include <iostream>
#include <algorithm>
#include <ranges>
#include <string>
#include  <numeric>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

namespace Cpp20Ranges_CommonRange
{
    void test_01()
    {
        std::vector<int> vec {9, 8, 7, 6, 5, 4, 3, 2, 1 };

        auto range = vec 
            | std::views::take_while([](int x) { return x > 5; })
            | std::views::common;

        auto result = std::accumulate(std::begin(range), std::end(range), 0);

        std::cout << result << std::endl;
    }
}

void ranges_09_common_range()
{
    using namespace Cpp20Ranges_CommonRange;

    test_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
