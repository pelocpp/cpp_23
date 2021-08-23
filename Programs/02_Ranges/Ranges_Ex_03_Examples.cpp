// ===========================================================================
// Ranges_Ex_03_Examples.cpp
// ===========================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <ranges>

namespace Cpp20RangesExamples
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
}

void ranges_ex_03_examples()
{
    using namespace Cpp20RangesExamples;

    example_01_filterMapReduce();
}

// ===========================================================================
// End-of-File
// ===========================================================================
