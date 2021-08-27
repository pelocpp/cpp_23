// ===========================================================================
// Ranges_Ex_06_RealworldExamples.cpp
// ===========================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <vector>
#include <ranges>

namespace Cpp20RangesRealworldExamples
{
    void example_01_filterMapReduce()
    {
        // testing 'Filter-Map-Reduce' Pattern
        struct Book {
            std::string m_title;
            std::string m_author;
            int m_year;
            double m_price;
        };

        std::list<Book> books {
                {"C", "Dennis Ritchie", 1972, 11.99 } ,
                {"Java", "James Gosling", 1995, 19.99 },
                {"C++", "Bjarne Stroustrup", 1985, 20.00 },
                {"C#", "Anders Hejlsberg", 2000, 29.99 }
        };

        // i) filter books which appeared past 1990
        // ii) extract book title
        auto titles = books
            | std::views::filter([](auto&& b) { return b.m_year >= 1990; })
            | std::views::transform([](auto&& b) { return b.m_title; });

        // iii) reduce to result string, e.g. comma seperated list
        auto result = std::accumulate(
            std::begin(titles),
            std::end(titles),
            std::string{},
            [](const std::string& a, auto&& b) {
                std::ostringstream oss;
                if (a.empty()) {
                    oss << b;
                }
                else {
                    oss << a << ", " << b;
                }
                return oss.str();
            }
        );

        std::cout << result << std::endl;
    }

    void example_02_binaryToDecimalConversion()
    {
        auto digits = std::vector<uint8_t>{ 1, 1, 1, 0 };

        // [0, 1, 2, 3, 4, 5, 6, 7, ...]
        auto rangeInts = std::views::iota(0, std::ranges::distance(digits));

        // [1, 2, 4, 8, 16, 32, 64, 128, ...]
        auto r_pow = rangeInts | std::views::transform([] (int x) {
            return 1 << x; }
        );

        // reverse range
        auto rangeReverse = digits | std::views::reverse;

        // only in classical STL-notation possible -  // e.g. 0*1 + 1*2 + 1*4 + 1*8 = 14
        auto value = std::inner_product(
            std::begin(rangeReverse), 
            std::end(rangeReverse),
            std::begin(r_pow), 
            0
        );

        std::cout << value << std::endl;
    }
}

void ranges_ex_06_examples()
{
    using namespace Cpp20RangesRealworldExamples;

    example_01_filterMapReduce();
    example_02_binaryToDecimalConversion();
}

// ===========================================================================
// End-of-File
// ===========================================================================
