// ===========================================================================
// Ranges_Ex_02_Ranges_View.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>
#include <numeric>
#include <vector>
#include <list>
#include <version>
#include <cassert>
#include <ranges>

// https://docs.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-160

/*****

To set this compiler option in Visual Studio,
open the project's Property Pages dialog box.

Select: Configuration Properties ==> C / C++ ==> Command Line property page.

Add /Zc:__cplusplus to the Additional options pane

Zc:__cplusplus	/std:c++17	201703L
Zc:__cplusplus	/std:c++20	202002L

****/

#define Cpp_20 202002L
#define Cpp_17 201703L

namespace Cpp20Views
{
    // iterating
#if __cplusplus <= Cpp_17
    template <typename T>
    using ValueType = typename std::remove_reference<T>::type::value_type;

    template <typename T>
    void print(T&& r) {
        std::for_each(
            std::begin(std::forward<T>(r)),
            std::end(std::forward<T>(r)),
            [](ValueType<T> i) { std::cout << i << ' '; }
        );
        std::cout << std::endl;
    }

    template <typename T>
    void print2(T&& r) {
        std::for_each(
            std::begin(std::forward<T>(r)),
            std::end(std::forward<T>(r)),
            [](auto&& i) { std::cout << i << ' '; }
        );
        std::cout << std::endl;
    }
#endif

#if __cplusplus >= Cpp_20
    void print(auto&& r) {
        std::ranges::for_each(r, [](auto&& i) { std::cout << i << ' '; });
        std::cout << std::endl;
    }

    template <typename T>
    void print2(T&& r) {
        std::ranges::for_each(std::forward<T>(r), [](auto&& i) { std::cout << i << ' '; });
        std::cout << std::endl;
    }
#endif


    // motivation
    void views1_17_motivation()
    {
#if __cplusplus <= Cpp_17

        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto getMaxScore = [](const std::vector<Student>& students, int year)
        {
            auto byYear = [=](const auto& s) { return s.m_year == year; };

            // student list needs to be copied in order to be filtered on the year
            auto result = std::vector<Student>{};

            std::copy_if(
                std::begin(students),
                std::end(students),
                std::back_inserter(result),
                byYear
            );

            auto it = std::max_element(
                std::begin(result),
                std::end(result),
                [](const Student& a, const Student& b) {
                    return a.m_score < b.m_score;
                }
            );

            return it != result.end() ? it->m_score : 0;
        };

        // Although it's easily achievable in this small example to find a fitting solution,
        // the goal is to to be able to implement this algorithm by composing small algorithmic building blocks,
        // rather than implementing it for every use case from scratch using a single for-loop
        auto getMaxScoreAlternate = [](const std::vector<Student>& students, int year) {
            auto max_score = 0;
            for (const auto& student : students) {
                if (student.m_year == year) {
                    max_score = std::max(max_score, student.m_score);
                }
            }
            return max_score;
        };

        auto students = std::vector<Student>{
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        auto score = getMaxScore(students, 2021);

        std::cout << score << std::endl;
#endif
    }

    void views1_20_motivation()
    {
#if __cplusplus >= Cpp_20

        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto maxValue = [](auto&& range) {
            const auto it = std::ranges::max_element(range);
            return it != range.end() ? *it : 0;
        };

        auto getMaxScore = [&](const std::vector<Student>& students, int year) {
            const auto byYear = [=](auto&& s) { return s.m_year == year; };
            return maxValue(students
                | std::views::filter(byYear)
                | std::views::transform(&Student::m_score)
            );
        };

        auto students = std::vector<Student>{
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        auto score = getMaxScore(students, 2021);

        std::cout << score << std::endl;

#endif
    }

    // introduction (understanding views from the Ranges library)
    void views2_20_introduction()
    {
#if __cplusplus >= Cpp_20
        // simple example
        auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto square = [](auto v) { return v * v; };
        auto squared_view = std::views::transform(numbers, square);
        for (auto s : squared_view) { // square lambda is invoked here
            std::cout << s << ", ";
        }
        std::cout << std::endl;
#endif
    }

    void views2_20a_introduction()
    {
#if __cplusplus >= Cpp_20
        // create a filtered view where only a part of the range is visible
        auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto oddView = std::views::filter(numbers, [](auto i) { return (i % 2) == 1; });
        for (auto odd : oddView) {
            std::cout << odd << ", ";
        }
        std::cout << std::endl;
#endif
    }

    void views2_20b_introduction()
    {
#if __cplusplus >= Cpp_20

        auto list_of_lists = std::vector<std::vector<int>>{
            { 1, 2 },
            { 3, 4, 5 },
            { 6 },
            { 7, 8, 9, 10 }
        };

        auto flattenedView = std::views::join(list_of_lists);

        for (auto v : flattenedView)
            std::cout << v << ", ";
        std::cout << std::endl;

        auto maxValue = *std::ranges::max_element(flattenedView);
        std::cout << "Maximum value: " << maxValue << std::endl;
#endif
    }

    // views are composable
    void views3_20_composable()
    {
#if __cplusplus >= Cpp_20

        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto getMaxScore = [](const std::vector<Student>& s, int year) {

            auto byYear = [=](const auto& s) { return s.m_year == year; };

            auto v1 = std::ranges::ref_view{ s };                  // wrap container in a view
            auto v2 = std::ranges::filter_view{ v1, byYear };      // apply 'filter' view
            auto v3 = std::ranges::transform_view{ v2, &Student::m_score };  // apply 'transform' view
            auto it = std::ranges::max_element(v3);                // apply 'max_element' view
            return it != v3.end() ? *it : 0;
        };

        auto getMaxScoreAlternate = [](const std::vector<Student>& s, int year) {

            auto byYear = [=](const auto& s) { return s.m_year == year; };

            auto scores = std::ranges::transform_view{
                std::ranges::filter_view {
                    std::ranges::ref_view{s}, 
                    byYear
                },
                &Student::m_score
            };

            auto it = std::ranges::max_element(scores);
            return it != scores.end() ? *it : 0;
        };

        auto students = std::vector<Student>{
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        auto score = getMaxScore(students, 2021);
        auto scoreAlternate = getMaxScoreAlternate(students, 2021);

        std::cout << score << std::endl;
        std::cout << scoreAlternate << std::endl;
#endif
    }

    // range views come with range adaptors
    void views4_20_range_adaptors()
    {
#if __cplusplus >= Cpp_20

        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto students = std::vector<Student>{
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        int year = 2021;
        auto byYear = [=](const auto& s) { return s.m_year == year; };

        auto scores = students | std::views::filter(byYear) | std::views::transform(&Student::m_score);

        auto it = std::ranges::max_element(scores);
        auto score = it != scores.end() ? *it : 0;

        std::cout << score << std::endl;
#endif
    }

    // views don't mutate the underlying container
    void views5_20_immutable()
    {
#if __cplusplus >= Cpp_20

        auto ints = std::list{ 1, 2, 3, 4, 5, 6 };

        auto strings = ints | std::views::transform([](auto i) {
            return std::string{"\""} + std::to_string(i) + std::string{ "\"" };
            }
        );

        for (const auto& s : strings) {
            std::cout << s << ", ";
        }
        std::cout << std::endl;

        for (const auto& i : ints) {
            std::cout << i << ", ";
        }
#endif
    }

    // views can be materialized into containers
#if __cplusplus >= Cpp_20
    auto to_vector(auto&& r)
    {
        std::vector<std::ranges::range_value_t<decltype(r)>> v;

        if constexpr (std::ranges::sized_range<decltype(r)>) {
            v.reserve(std::ranges::size(r));
        }

        std::ranges::copy(r, std::back_inserter(v));

        return v;
    }
#endif

    void views6_20_materialize()
    {
#if __cplusplus >= Cpp_20

        auto ints = std::list{ 1, 2, 3, 4, 5, 6 };

        auto strings = ints | std::views::transform([](auto i) {
            return std::string{ "\"" } + std::to_string(i) + std::string{ "\"" };
            }
        );

        auto vec = std::vector<std::string>{};

        std::ranges::copy(strings, std::back_inserter(vec));

        for (const auto& s : vec) {
            std::cout << s << ", ";
        }
        std::cout << std::endl;

        vec.clear();

        vec = to_vector(strings);

        for (const auto& s : vec) {
            std::cout << s << ", ";
        }
        std::cout << std::endl;
#endif
    }

    // views are lazy evaluated (sort doesn't work)
    void views7_20_lazy_evaluation()
    {
    #if __cplusplus >= Cpp_20

        auto vec = std::vector{ 4, 2, 7, 1, 2, 6, 1, 5 };
        print(vec);

        // filter range
        auto isOdd = [](auto i) { return i % 2 == 1; };
        auto odd_numbers = vec | std::views::filter(isOdd);

        // std::ranges::sort(odd_numbers); // doesn't compile !!!

        // materialize the view before sorting
        auto result = to_vector(odd_numbers);
        print(result);

        // sort range
        std::ranges::sort(result);
        print(result);
    #endif
    }

    void views7_20a_lazy_evaluation()
    {
    #if __cplusplus >= Cpp_20

        auto vec = std::vector{ 8, 6, 10, 9, 2, 1, 3, 7, 4, 5 };
        print(vec);

        // filter range
        auto firstHalf = vec | std::views::take(vec.size());
        std::ranges::sort(firstHalf);
        print(firstHalf);

        std::ranges::sort(firstHalf, std::greater<>{});
        print(firstHalf);
    #endif
    }
}

void ranges_ex_02_views()
{
    using namespace Cpp20Views;

    views1_17_motivation();
    views1_20_motivation();
    views2_20_introduction();
    views2_20a_introduction();
    views2_20b_introduction();
    views3_20_composable();
    views4_20_range_adaptors();
    views5_20_immutable();
    views6_20_materialize();
    views7_20_lazy_evaluation();
    views7_20a_lazy_evaluation();
}

// ===========================================================================
// End-of-File
// ===========================================================================
