// ===========================================================================
// Ranges_01_Algorithms.cpp
// ===========================================================================

import std;

namespace Cpp17
{
    template <typename T>
    using ValueType = typename std::remove_reference<T>::type::value_type;

    template <typename T>
    static void print(T&& range) {
        std::for_each(
            std::begin(std::forward<T>(range)),
            std::end(std::forward<T>(range)),
            [](ValueType<T> i) { std::cout << i << ' '; }
        );
        std::cout << std::endl;
    }
}

namespace Cpp23
{
    static void print(auto&& range)
    {
        // std::print("{:n}", range);  // NOT YET SUPPORTED 

        for (const auto& elem : range) {
            std::print("{} ", elem);
        }
        std::println("");
    }
}

namespace Cpp20Ranges
{
    // iterating
    static void range1_17_iterating()
    {
        auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
        Cpp17::print(vec);
    }

    static void range1_23_iterating()
    {
        auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
        Cpp23::print(vec);
    }

    // transforming
    static void range2_17_transforming()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto res = std::vector<int>(vec.size());
        auto lambda = [](auto&& i) { return i * i; };

        std::transform(std::begin(vec), std::end(vec), std::begin(res), lambda);
        Cpp17::print(res);
    }

    static void range2_23_transforming()
    {
        auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto res = std::vector<int>(vec.size());
        auto lambda = [](auto&& i) { return i * i; };

        std::ranges::transform(vec, std::begin(res), lambda);
        Cpp23::print(res);
    }

    // generating elements
    static void range3_17_generating()
    {
        auto vec = std::vector<int>(5);

        std::fill(std::begin(vec), std::end(vec), 1);
        Cpp17::print(vec);

        std::generate(
            std::begin(vec),
            std::end(vec),
            [count = 1]() mutable { return count++; }
        );
        Cpp17::print(vec);

        std::iota(
            std::begin(vec),
            std::end(vec),
            10
        );
        Cpp17::print(vec);
    }

    static void range3_23_generating()
    {
        auto vec = std::vector<int>(5);
        std::ranges::fill(vec, 1);
        Cpp23::print(vec);

        std::ranges::generate(vec, [count = 1]() mutable { return count++; });
        Cpp23::print(vec);

        auto values{ std::ranges::views::iota(10) | std::ranges::views::take(5) };
        Cpp23::print(values);
    }

    // sorting
    static void range4_17_sorting()
    {
        auto values = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };

        // sort using the classical algorithm
        std::sort(std::begin(values), std::end(values));
        Cpp17::print(values);
    }

    static void range4_23_sorting()
    {
        auto values = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };

        // sort using the constrained algorithm under std::ranges
        std::ranges::sort(values);
        Cpp23::print(values);
    }

    // finding elements
    static void range5_17_finding()
    {
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
    }

    static void range5_23_finding()
    {
        auto values = std::list{ 4, 3, 2, 3, 1 };

        auto it = std::ranges::find(values, 2);
        if (it != std::end(values)) {
            std::println("{}", *it);
        }
    }

    // finding elements using binary search
    static void range6_17_finding_binary()
    {
        auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };     // sorted!

        bool sorted = std::is_sorted(
            std::begin(vec),
            std::end(vec)
        );
        std::cout << "Sorted: " << std::boolalpha << sorted << std::endl;

        bool found = std::binary_search(
            std::begin(vec),
            std::end(vec),
            3
        );
        std::cout << "Found:  " << std::boolalpha << found << std::endl;
    }

    static void range6_23_finding_binary()
    {
        auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };     // sorted!

        bool sorted = std::ranges::is_sorted(vec);
        std::println("Sorted: {}", sorted);

        bool found = std::ranges::binary_search(vec, 3);
        std::println("Found:  {}", found);
    }

    // testing for certain conditions (any_of, all_of and none_of) 
    static void range7_17_conditions()
    {
        auto vec = std::vector{ 5, 4, 3, 2, 1, 0, -1, 0, 1, 2 };
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
    }

    static void range7_23_conditions()
    {
        auto vec = std::vector{ 5, 4, 3, 2, 1, 0, -1, 0, 1, 2 };
        auto is_negative = [](auto i) { return i < 0; };

        if (std::ranges::none_of(vec, is_negative)) {
            std::println("Contains only positive numbers");
        }

        if (std::ranges::all_of(vec, is_negative)) {
            std::println("Contains only negative numbers");
        }

        if (std::ranges::any_of(vec, is_negative)) {
            std::println("Contains at least one negative number");
        }
    }

    // counting elements
    static void range8_17_counting()
    {
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
    }

    static void range8_23_counting()
    {
        auto numbers = std::vector{ 3, 3, 2, 1, 3, 1, 3 };

        // counting in linear time
        auto n = std::ranges::count(numbers, 3);
        std::println("{}", n);

        // counting in O(log n) time
        std::ranges::sort(numbers);
        std::ranges::subrange result = std::ranges::equal_range(numbers, 3);
        n = std::ranges::size(result);
        std::println("{}", n);
    }

    // minimum, maximum, and clamping
    const auto min = 1;
    const auto max = 100;

    int some_func() { return 50; }

    static void range9_17_minmaxclamp()
    {
        auto y{ 0 };
        y = std::min(some_func(), max);
        y = std::max(std::min(some_func(), max), min);
        y = std::clamp(some_func(), min, max);

        auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9, 10 };
        auto min_iter = std::min_element(std::begin(vec), std::end(vec));
        auto max_iter = std::max_element(std::begin(vec), std::end(vec));
        std::cout << "Min: " << *min_iter << ", Max: " << *max_iter << std::endl;
    }

    static void range9_23_minmaxclamp()
    {
        auto y{ 0 };
        y = std::min(some_func(), max);
        y = std::max(std::min(some_func(), max), min);
        y = std::clamp(some_func(), min, max);

        const auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9, 10 };
        auto min_iter = std::ranges::min_element(vec);
        auto max_iter = std::ranges::max_element(vec);
        std::println("Min: {}, Max: {}", *min_iter, *max_iter);

        const auto [min, max] = std::ranges::minmax(vec);
        std::println("Min: {}, Max: {}", min, max);
    }

    // custom comparator functions
    static void range10_17_custom_comparator()
    {
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
        Cpp17::print(names);

        // find names with length 3
        auto result = std::find_if(
            std::begin(names),
            std::end(names),
            [](const auto& v) { return v.size() == 3; }
        );
        if (result != std::end(names)) {
            std::cout << *result << std::endl;
        }
        std::cout << std::endl;
    }

    static void range10_17a_custom_comparator()
    {
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
        );
        std::cout << "Next priorities:" << std::endl;
        std::for_each(std::begin(tasks), std::end(tasks), print);
        std::cout << std::endl;
    }

    // constrained algorithms use projections
    static void range10_23_custom_comparator()
    {
        auto names = std::vector<std::string>{
            "Alexander", "Dennis", "Bjarne", "Ken", "Stephen", "Dave"
        };

        // i) names is passed as the first argument
        // ii) {} means the default template argument - in this case, it's std::less<>{} as comparator
        // iii) the projection is a callable which takes a single argument

        std::ranges::sort(names, {}, &std::string::size);
        std::ranges::sort(names, std::less<>{}, & std::string::size);
        Cpp23::print(names);

        // find names with length 3
        auto result = std::ranges::find(names, 3, &std::string::size);
        if (result != std::end(names)) {
            std::println("{}", *result);
        }
        std::println("");
    }

    static void range10_20a_custom_comparator()
    {
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
            std::println("{}: Priority: {}", t.m_desc, t.m_priority);
        };

        std::println("List of tasks:");
        std::ranges::for_each(tasks, print);
        std::ranges::sort(tasks, std::ranges::greater{}, &Task::m_priority); // <<  "extract" a data member 
        std::println("Next priorities:");
        std::ranges::for_each(tasks, print);
    }
}

void ranges_01_algorithms()
{
    using namespace Cpp20Ranges;

    range1_17_iterating();
    range1_23_iterating();
    range2_17_transforming();
    range2_23_transforming();
    range3_17_generating();
    range3_23_generating();

    range4_17_sorting();
    range4_23_sorting();
     
    range5_17_finding();
    range5_23_finding();
    range6_17_finding_binary();
    range6_23_finding_binary();
    
    range7_17_conditions();
    range7_23_conditions();
    
    range8_17_counting();
    range8_23_counting();
    range9_17_minmaxclamp();
    range9_23_minmaxclamp();
    
    range10_17_custom_comparator();
    range10_17a_custom_comparator();
    range10_23_custom_comparator();
    range10_20a_custom_comparator();
}

// ===========================================================================
// End-of-File
// ===========================================================================
