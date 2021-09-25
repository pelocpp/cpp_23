// ===========================================================================
// Ranges_Ex_05_MiscExamples.cpp
// ===========================================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <list>
#include <vector>
#include <ranges>

#define Cpp_20 202002L
#define Cpp_17 201703L

namespace Cpp20RangesMiscellaneousExamples
{
#if __cplusplus >= Cpp_20

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

    // =======================================================================

    // count the number of words (delimited by space) in a text
    auto sizeOfRange(auto&& r)
    {
        size_t result{};

        if constexpr (std::ranges::sized_range<decltype(r)>) {
            result = std::ranges::size(r);
        }
        else {
            result = std::distance(r.begin(), r.end());
        }

        return result;
    }

    void example_strings_01()
    {
        std::string text { "The quick brown fox jumps over the lazy dog" };

        auto words{ text | std::views::split(' ') };
        size_t num{ sizeOfRange(words) };
        std::cout << num << " words.";
    }

    auto toVector(auto&& r)
    {
        std::vector<std::ranges::range_value_t<decltype(r)>> vec;

        if constexpr (std::ranges::sized_range<decltype(r)>) {
            vec.reserve(std::ranges::size(r));
        }
        else {
            vec.reserve(std::distance(r.begin(), r.end()));
        }

        std::ranges::copy(r, std::back_inserter(vec));
        return vec;
    }

    void example_strings_02()
    {
        std::string text{ "The-quick-brown-fox-jumps-over-the-lazy-dog" };

        auto range = text | std::views::split('-') | std::views::transform([](auto&& s) {
            auto subrange{ s | std::views::common };
            std::string word{ subrange.begin(), subrange.end() };
            return word;
        });

        auto words{ toVector(range) };

        for (auto&& word : words) {
            std::cout << word << "--";
        }
    }

    void example_strings_02a()
    {
        std::string text{ "The-quick-brown-fox-jumps-over-the-lazy-dog" };

        auto range = text | std::views::split('-') | std::views::transform([](auto&& s) {
            auto subrange{ s | std::views::common };
            std::string word{ subrange.begin(), subrange.end() };
            return word;
        });

        for (auto&& word : range) {
            std::cout << word << "!!";
        }
    }

    auto toString(auto&& r)
    {
        std::string result{};
        if constexpr (std::ranges::sized_range<decltype(r)>) {
            result.reserve(std::ranges::size(r));
        }
        else {
            result.reserve(std::distance(r.begin(), r.end()));
        }

        std::ranges::copy(r, std::back_inserter(result));
        return result;
    }

    void example_strings_02b()
    {
        std::string text { "The-quick-brown-fox-jumps-over-the-lazy-dog" };

        auto range = text | std::views::split('-') | std::views::transform([](auto&& s) {
            auto subrange{ s | std::views::common };
            std::string word{ toString (subrange)};
            return word;
        });

        for (auto&& word : range) {
            std::cout << word << "??";
        }
    }

    void example_strings_03()
    {
        std::vector<std::string> words{
            "Lorem", ".", "ipsum", ".",
            "dolor", ".", "sit", ".",
            "amet"
        };

        // okay, solution without ranges :(
        std::string text{ std::accumulate(std::begin(words), std::end(words), std::string{}) };
        std::cout << text;
    }

    void example_strings_03a()
    {
        std::vector<std::string> words{
            "Lorem", "-", "ipsum", "-",
            "dolor", "-", "sit", "-",
            "amet"
        };

        // this solution is with ranges :)
        auto range{ words | std::views::join };
        std::string text{ toString(range) };
        std::cout << text;
    }

    void example_strings_04()
    {
        std::string words{ "Modern-_-C++-_-is-_-really-_-awesome-_-!" };
        std::string delim{ "-_-" };

        auto range = words | std::views::split(delim) | std::views::transform([](auto&& s) {
            auto subrange{ s | std::views::common };
            std::string word{ subrange.begin(), subrange.end() };
            return word;
        });

        for (std::string word : range) {
            std::cout << std::quoted(word) << std::endl;
        }
    }
#endif
}

void ranges_ex_05_examples()
{
#if __cplusplus >= Cpp_20

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

    example_strings_01();
    std::cout << std::endl;
    example_strings_02();
    std::cout << std::endl;
    example_strings_02a();
    std::cout << std::endl;
    example_strings_02b();
    std::cout << std::endl;
    example_strings_03();
    std::cout << std::endl;
    example_strings_03a();
    std::cout << std::endl;
    example_strings_04();
    std::cout << std::endl;
#endif
}

// ===========================================================================
// End-of-File
// ===========================================================================
