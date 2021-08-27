// ===========================================================================
// Ranges_Ex_05_MiscExamples.cpp
// ===========================================================================

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <vector>
#include <ranges>

namespace Cpp20RangesMiscellaneousExamples
{
    auto printElem = [](auto n) { std::cout << n << ' '; };
    
    auto printElemNL = [](auto n) { std::cout << n << std::endl; };

    // print the all the elements of a range 
    void example_01()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        std::ranges::for_each(vec, printElem);
    }

    // print the all the elements of a range in reverse order
    void example_02()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto range = vec | std::views::reverse;

        std::ranges::for_each(range, printElem);

        // or 
        //std::cout << std::endl;

        //for (auto n : vec | std::views::reverse) {
        //    printElem(n);
        //}
    }

    // print only the even elements of a range 
    void example_03()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto range = vec | std::views::filter([](auto i) { return i % 2 == 0; });

        std::ranges::for_each(range, printElem);
    }

    // print only the even elements of a range in reverse order
    void example_04()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto range = vec
            | std::views::filter([](auto i) { return i % 2 == 0; })
            | std::views::reverse;

        //or

        //auto range = vec
        //    | std::views::reverse 
        //    | std::views::filter([](auto i) { return i % 2 == 0; });

        std::ranges::for_each(range, printElem);
    }

    // skip the first two elements of the range and print only the even numbers of the next three in the range
    void example_05()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto range = vec
            | std::views::drop(2)
            | std::views::take(4)
            | std::views::filter([](auto i) { return i % 2 == 0; });

        std::ranges::for_each(range, printElem);
    }

    // print all numbers from 1 to 20
    void example_06()
    {
        auto range = std::views::iota(1, 21);

        std::ranges::for_each(range, printElem);
    }

    // print all Roman numerals from 1 to 50
    std::string toRoman(int value)
    {
        std::vector<std::pair<int, std::string_view>> romans
        {
            { 1000, "M" },
            { 900, "CM" },
            { 500, "D" },
            { 400, "CD" },
            { 100, "C" },
            { 90, "XC" },
            { 50, "L" },
            { 40, "XL" },
            { 10, "X" },
            { 9, "IX" },
            { 5, "V" },
            { 4, "IV" },
            { 1, "I" }
        };

        std::string result{};

        for (auto const& [decimal, roman] : romans)
        {
            while (value >= decimal)
            {
                result += roman;
                value -= decimal;
            }
        }

        return result;
    }

    void example_07()
    {
        auto range = std::views::iota(1, 51)
            | std::views::transform([](auto i) { return toRoman(i); });

        std::ranges::for_each(range, printElemNL);
    }

    // print the Roman numerals of the last three numbers divisible by 7 in the range [1, 50], in reverse order
    void example_08()
    {
        auto range = std::views::iota(1, 22)
            | std::views::filter([](auto i) { return i % 7 == 0; })
            | std::views::transform([](auto i) { return toRoman(i); })
            | std::views::reverse
            | std::views::take(3);

        std::ranges::for_each(range, printElemNL);
    }


}

void ranges_ex_05_examples()
{
    using namespace Cpp20RangesMiscellaneousExamples;

    example_01();
    std::cout << std::endl;
    example_02();
    std::cout << std::endl;
    example_03();
    std::cout << std::endl;
    example_04();
    std::cout << std::endl;
    example_05();
    std::cout << std::endl;
    example_06();
    std::cout << std::endl;
    example_07();
    std::cout << std::endl;
    example_08();
    std::cout << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
