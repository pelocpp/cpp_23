// ===========================================================================
// Ranges_Ex_01_Algorithms.cpp
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

namespace Cpp20Ranges
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

    // iterating
    void r1_17()
    {
#if __cplusplus <= Cpp_17
        auto vec = std::vector{ 9, 2, 5, 3, 4 };
        print(vec);
        print2(vec);
#endif
    }

    void r1_20()
    {
#if __cplusplus >= Cpp_20
        auto vec = std::vector{ 9, 2, 5, 3, 4 };
        print(vec);
        print2(vec);
#endif
    }

    // transforming
    void r2_17()
    {
#if __cplusplus <= Cpp_17
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto res = std::vector<int>(vec.size());
        auto lambda = [](auto&& i) { return i * i; };

        std::transform(std::begin(vec), std::end(vec), std::begin(res), lambda);
        print(res);
#endif
    }

    void r2_20()
    {
#if __cplusplus >= Cpp_20
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto res = std::vector<int>(vec.size());
        auto lambda = [](auto&& i) { return i * i; };

        std::ranges::transform(vec, std::begin(res), lambda);
        print(res);
#endif
    }

    // generating elements
    void r3_17()
    {
#if __cplusplus <= Cpp_17
        auto vec = std::vector<int>(5);

        std::fill(
            std::begin(vec),
            std::end(vec),
            1
        );
        print(vec);

        std::generate(
            std::begin(vec),
            std::end(vec),
            [count = 1]() mutable { return count++; }
        );
        print(vec);

        std::generate(
            std::begin(vec),
            std::end(vec),
            std::rand
        );
        print(vec);

        std::iota(
            std::begin(vec),
            std::end(vec),
            10
        );
        print(vec);
#endif
    }

    void r3_20()
    {
#if __cplusplus >= Cpp_20
        auto vec = std::vector<int>(5);

        std::ranges::fill(vec, 1);
        print(vec);

        std::ranges::generate(vec, [count = 1]() mutable { return count++; });
        print(vec);

        std::ranges::generate(vec, std::rand);
        print(vec);

        // to be done: std::iota
#endif
    }

    // sorting
    void r4_17()
    {
#if __cplusplus <= Cpp_17
        auto values = std::vector{ 9, 2, 5, 3, 4 };

        // sort using the classical algorithm
        std::sort(std::begin(values), std::end(values));
        print(values);
#endif
    }

    void r4_20()
    {
#if __cplusplus >= Cpp_20
        auto values = std::vector{ 9, 2, 5, 3, 4 };

        // sort using the constrained algorithm under std::ranges
        std::ranges::sort(values);
        print(values);

        // works too
        std::ranges::sort(std::begin(values), std::end(values));
        print(values);
#endif
    }

    // finding elements
    void r5_17()
    {
#if __cplusplus <= Cpp_17
        auto values = std::list{ 4, 3, 2, 3, 1 };

        // if the element we are looking for couldn't be found,
        // find() returns end() iterator of the collection
        auto it = std::find(
            std::begin(values),
            std::end(values),
            2
        );
        if (it != std::end(values)) {
            std::cout << *it << std::endl;
        }
#endif
    }

    void r5_20()
    {
#if __cplusplus >= Cpp_20
        auto values = std::list{ 4, 3, 2, 3, 1 };

        auto it = std::ranges::find(values, 2);
        if (it != std::end(values)) {
            std::cout << *it << std::endl;
        }
#endif
    }

    // finding elements using binary search
    void r6_17()
    {
#if __cplusplus <= Cpp_17
        auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };   // sorted!

        assert(std::is_sorted(
            std::begin(vec),
            std::end(vec))
        );

        bool found = std::binary_search(
            std::begin(vec),
            std::end(vec),
            3
        );
        std::cout << std::boolalpha << found << std::endl;
#endif
    }

    void r6_20()
    {
#if __cplusplus >= Cpp_20
        auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };     // sorted!
        assert(std::ranges::is_sorted(vec));
        bool found = std::ranges::binary_search(vec, 3);
        std::cout << std::boolalpha << found << std::endl;
#endif
    }

    // testing for certain conditions (any_of, all_of and none_of) 
    void r7_17()
    {
#if __cplusplus <= Cpp_17
        auto vec = std::vector{ 3, 2, 2, 1, 0, 2, 1 };
        auto is_negative = [](auto i) { return i < 0; };

        if (std::none_of(std::begin(vec), std::end(vec), is_negative)) {
            std::cout << "Contains only positive numbers" << std::endl;
        }

        if (std::all_of(std::begin(vec), std::end(vec), is_negative)) {
            std::cout << "Contains only negative numbers" << std::endl;
        }

        if (std::any_of(std::begin(vec), std::end(vec), is_negative)) {
            std::cout << "Contains at least one negative number" << std::endl;
        }
#endif
    }

    void r7_20()
    {
#if __cplusplus >= Cpp_20
        auto vec = std::vector{ 3, 2, 2, 1, 0, 2, 1 };
        auto is_negative = [](auto i) { return i < 0; };

        if (std::ranges::none_of(vec, is_negative)) {
            std::cout << "Contains only positive numbers" << std::endl;
        }

        if (std::ranges::all_of(vec, is_negative)) {
            std::cout << "Contains only negative numbers" << std::endl;
        }

        if (std::ranges::any_of(vec, is_negative)) {
            std::cout << "Contains at least one negative number" << std::endl;
        }
#endif
    }

    // counting elements
    void r8_17()
    {
#if __cplusplus <= Cpp_17
        auto numbers = std::vector{ 3, 3, 2, 1, 3, 1, 3 };

        // counting in linear time
        auto n = std::count(std::begin(numbers), std::end(numbers), 3);
        std::cout << n << std::endl;

        // counting in O(log n) time
        std::sort(std::begin(numbers), std::end(numbers));
        std::pair<std::vector<int>::iterator, std::vector<int>::iterator> result =
            std::equal_range(std::begin(numbers), std::end(numbers), 3);
        n = std::distance(result.first, result.second);
        std::cout << n << std::endl;
#endif
    }

    void r8_20()
    {
#if __cplusplus >= Cpp_20
        auto numbers = std::vector{ 3, 3, 2, 1, 3, 1, 3 };

        // counting in linear time
        auto n = std::ranges::count(numbers, 3);
        std::cout << n << std::endl;

        // counting in O(log n) time
        std::ranges::sort(numbers);
        std::ranges::subrange result = std::ranges::equal_range(numbers, 3);
        n = std::ranges::size(result);
        std::cout << n << std::endl;
#endif
    }

    // minimum, maximum, and clamping
    const auto min = 1;
    const auto max = 100;

    int some_func() { return 50; }

    void r9_17()
    {
#if __cplusplus <= Cpp_17

        auto y{ 0 };
        y = std::min(some_func(), max);
        y = std::max(std::min(some_func(), max), min);
        y = std::clamp(some_func(), min, max);

        auto vec = std::vector{ 4, 2, 1, 7, 3, 1, 5 };
        auto min_iter = std::min_element(std::begin(vec), std::end(vec));
        auto max_iter = std::max_element(std::begin(vec), std::end(vec));
        std::cout << "Min: " << *min_iter << ", Max: " << *max_iter << std::endl;
#endif
    }

    void r9_20()
    {
#if __cplusplus >= Cpp_20
        auto y{ 0 };
        y = std::min(some_func(), max);
        y = std::max(std::min(some_func(), max), min);
        y = std::clamp(some_func(), min, max);

        const auto vec = std::vector{ 4, 2, 1, 7, 3, 1, 5 };
        auto min_iter = std::ranges::min_element(vec);
        auto max_iter = std::ranges::max_element(vec);
        std::cout << "Min: " << *min_iter << ", Max: " << *max_iter << std::endl;

        const auto [min, max] = std::ranges::minmax(vec);
        std::cout << "Min: " << min << ", Max: " << max << std::endl;
#endif
    }

    // custom comparator functions
    void r10_17()
    {
#if __cplusplus <= Cpp_17
        auto names = std::vector<std::string>{
            "Alexander", "Dennis", "Bjarne", "Ken", "Stephen", "Dave"
        };

        std::sort(
            std::begin(names),
            std::end(names),
            [](const std::string& a, const std::string& b) {
                return a.size() < b.size();
            }
        );
        print(names);

        // find names with length 3
        auto result = std::find_if(
            std::begin(names),
            std::end(names),
            [](const auto& v) { return v.size() == 3; }
        );
        if (result != std::end(names)) {
            std::cout << *result << std::endl;
        }

#endif
    }

    void r10_17a()
    {
#if __cplusplus <= Cpp_17
        struct Task {
            std::string m_desc{};
            unsigned int m_priority{ 0 };
        };

        std::vector<Task> tasks{
            { "Clean up my apartment", 10 },
            { "Finish homework", 5 },
            { "Go to the supermarket", 12 }
        };

        auto print = [](auto&& t) {
            std::cout << t.m_desc << ": Priority: " << t.m_priority << std::endl;
        };

        std::cout << "List of tasks:" << std::endl;
        std::for_each(std::begin(tasks), std::end(tasks), print);
        std::sort(
            std::begin(tasks),
            std::end(tasks),
            [](const Task& a, const Task& b) {
                return a.m_priority > b.m_priority;
            }
        ); // <<  “extract” a data member 
        std::cout << "Next priorities:" << std::endl;
        std::for_each(std::begin(tasks), std::end(tasks), print);
#endif
    }

    // constrained algorithms use projections
    void r10_20()
    {
#if __cplusplus >= Cpp_20

        auto names = std::vector<std::string>{
            "Alexander", "Dennis", "Bjarne", "Ken", "Stephen", "Dave"
        };

        // i) names is passed as the first argument
        // ii) {} means the default template argument - in this case, it’s std::less<>{} as comparator
        // iii) the projection is a callable which takes a single argument

        std::ranges::sort(names, {}, &std::string::size);
        std::ranges::sort(names, std::less<>{}, & std::string::size);
        print(names);

        // find names with length 3
        auto result = std::ranges::find(names, 3, &std::string::size);
        if (result != std::end(names)) {
            std::cout << *result << std::endl;
        }
#endif
    }

    // https://www.cppstories.com/2020/10/understanding-invoke.html/
    void r10_20a()
    {
#if __cplusplus >= Cpp_20

        struct Task {
            std::string m_desc{};
            unsigned int m_priority{ 0 };
        };

        std::vector<Task> tasks{
            { "Clean up my apartment", 10 },
            { "Finish homework", 5 },
            { "Go to the supermarket", 12 }
        };

        auto print = [](auto&& t) {
            std::cout << t.m_desc << ": Priority: " << t.m_priority << std::endl;
        };

        std::cout << "List of tasks:" << std::endl;
        std::ranges::for_each(tasks, print);
        std::ranges::sort(tasks, std::ranges::greater{}, &Task::m_priority); // <<  “extract” a data member 
        std::cout << "Next priorities:" << std::endl;
        std::ranges::for_each(tasks, print);
#endif
    }
}

void ranges_ex_01_algorithms()
{
    using namespace Cpp20Ranges;

    r1_17();
    r1_20();
    r2_17();
    r2_20();
    r3_17();
    r3_20();
    r4_17();
    r4_20();
    r5_17();
    r5_20();
    r6_17();
    r6_20();
    r7_17();
    r7_20();
    r8_17();
    r8_20();
    r9_17();
    r9_20();
    r10_17();
    r10_17a();
    r10_20();
    r10_20a();
}

// ===========================================================================
// End-of-File
// ===========================================================================
