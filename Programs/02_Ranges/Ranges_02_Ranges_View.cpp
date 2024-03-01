// ===========================================================================
// Ranges_Ex_02_Ranges_View.cpp
// ===========================================================================

import std;

namespace Cpp20Views
{
    // motivation
    static void views1_17_motivation()
    {
        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto getMaxScore = [](const std::vector<Student>& students, int year)
        {
            // student list needs to be copied in order to be filtered on the year
            auto result = std::vector<Student>{};

            std::copy_if(
                std::begin(students),
                std::end(students),
                std::back_inserter(result),
                [=](const auto& student) { return student.m_year == year; }
            );

            auto it = std::max_element(
                std::begin(result),
                std::end(result),
                [](const Student& a, const Student& b) {
                    return a.m_score < b.m_score;
                }
            );

            return it != result.end() ? it->m_score : -1;
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

        auto students = std::vector<Student>
        {
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        auto score = getMaxScore(students, 2021);

        std::cout << score << std::endl;
    }

    static void views1_23_motivation()
    {
        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto getMaxScore = [](const std::vector<Student>& students, int year) {

            auto maxValue = [](auto&& range) {
                const auto it = std::ranges::max_element(range);
                return it != range.end() ? *it : -1;
            };

            const auto byYear = [=](auto&& s) { return s.m_year == year; };

            return maxValue(students
                | std::ranges::views::filter(byYear)
                | std::ranges::views::transform(&Student::m_score)
            );
        };

        auto students = std::vector<Student>
        {
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        auto score = getMaxScore(students, 2021);

        std::println("{}", score);
    }

    // ------------------------------------------------------------------

    // introduction (understanding views from the Ranges library)
    static void views2_introduction_01()
    {
        auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto square = [](auto v) { return v * v; };

        auto squared_view = std::ranges::views::transform(numbers, square);

        for (auto&& s : squared_view) {
            std::print("{}, ", s);
        }
        std::println("");
    }

    static void views2_introduction_02()
    {
        auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto square = [](auto v) { return v * v; };

        auto squared_view = numbers | std::ranges::views::transform(square);

        for (auto&& s : squared_view) {
            std::print("{}, ", s);
        }
        std::println("");
    }

    static void views2_introduction_03()
    {
        // create a filtered view where only a part of the range is visible
        auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto oddView = std::ranges::views::filter(
            numbers,
            [](auto i) { return (i % 2) == 1; }
        );

        for (auto&& s : oddView) {
            std::print("{}, ", s);
        }
        std::println("");
    }

    static void views2_introduction_04()
    {
        // create a filtered view where only a part of the range is visible
        auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto oddView = numbers
            | std::ranges::views::filter([](auto i) { return (i % 2) == 1; });

        for (auto&& s : oddView) {
            std::print("{}, ", s);
        }
        std::println("");
    }

    static void views2_introduction_05()
    {
        auto list_of_lists = std::vector<std::vector<int>>
        {
            { 1, 2 },
            { 3, 4, 5 },
            { 6 },
            { 7, 8, 9, 10 }
        };

        auto flattenedView = std::ranges::views::join(list_of_lists);

        for (auto&& s : flattenedView) {
            std::print("{}, ", s);
        }
        std::println("");

        auto maxValue = *std::ranges::max_element(flattenedView);
        std::println("Maximum value: {}", maxValue);
    }

    // ------------------------------------------------------------------

    // views are composable
    static void views3_composable()
    {
        struct Student {
            std::string m_name{};
            int m_year{};
            int m_score{};
        };

        auto getMaxScore = [](const std::vector<Student>& s, int year) {

            auto byYear = [=](const auto& s) { return s.m_year == year; };

            auto view1 = std::ranges::ref_view{ s };                     // wrap container in a view

            auto view2 = std::ranges::filter_view{ view1, byYear };      // apply 'filter' view

            auto view3 = std::ranges::transform_view{ view2, &Student::m_score };  // apply 'transform' view

            auto it = std::ranges::max_element(view3);                   // apply 'max_element' view

            return it != view3.end() ? *it : 0;
        };

        auto getMaxScoreAlternate = [](const std::vector<Student>& s, int year) {

            auto byYear = [=](const auto& s) { return s.m_year == year; };

            auto scores = std::ranges::transform_view {
                std::ranges::filter_view {
                    std::ranges::ref_view{s}, 
                    byYear
                },
                &Student::m_score
            };

            auto it = std::ranges::max_element(scores);

            return it != scores.end() ? *it : 0;
        };

        auto students = std::vector<Student>
        {
            {"Georg", 2021, 120 },
            {"Hans",  2021, 140 },
            {"Susan", 2020, 180 },
            {"Mike",  2020, 110 },
            {"Hello", 2021, 190 },
            {"Franz", 2021, 110 },
        };

        auto score = getMaxScore(students, 2021);
        auto scoreAlternate = getMaxScoreAlternate(students, 2021);

        std::println("Score: {}", score);
        std::println("scoreAlternate: {}", scoreAlternate);
    }

    // ------------------------------------------------------------------

    // projections
    class Person
    {
    public:
        explicit Person(std::string first, std::string last)
            : m_firstName{ first }, m_lastName{ last }
        {}

        const std::string& getFirstName() const { return m_firstName; }
        const std::string& getLastName() const { return m_lastName; }

    private:
        std::string m_firstName;
        std::string m_lastName;
    };

    static void views4_projections_01()
    {
        std::vector<Person> persons
        {
            Person{ "John", "Miller" },
            Person{ "Jack", "Wagner" }
        };

        // std::sort(std::begin(persons), std::end(persons)); 

        // Error: "call to object of class type 'std::less<void>' : no matching call operator found
    }

    static void views4_projections_02()
    {
        std::vector<Person> persons
        {
            Person{ "John", "Miller" },
            Person{ "Jack", "Wagner" }
        };

        std::ranges::sort(
            std::begin(persons),
            std::end(persons),
            std::ranges::less{},
            [](const Person& person) { return person.getLastName(); }
        );

        for (const auto& elem : persons) {
            std::println("{} {}", elem.getFirstName(), elem.getLastName());
        }
    }

    static void views4_projections_03()
    {
        std::vector<Person> persons
        {
            Person{ "John", "Miller" },
            Person{ "Jack", "Wagner" }
        };

        std::ranges::sort(
            std::begin(persons),
            std::end(persons),
            {},
            &Person::getFirstName
        );

        for (const auto& elem : persons) {
            std::println("{} {}", elem.getFirstName(), elem.getLastName());
        }
    }

    // ------------------------------------------------------------------

    // range views come with range adaptors
    static void views5_range_adaptors()
    {
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

        auto scores = students 
            | std::ranges::views::filter(byYear) 
            | std::ranges::views::transform(&Student::m_score);

        auto it = std::ranges::max_element(scores);
        auto score = it != scores.end() ? *it : 0;

        std::println("Score: {}", score);
    }

    // ------------------------------------------------------------------

    // views don't mutate the underlying container
    static void views6_immutable()
    {
        auto numbers = std::list{ 1, 2, 3, 4, 5, 6 };

        auto strings = numbers | std::ranges::views::transform([](auto i) {
            return std::string{"\""} + std::to_string(i) + std::string{ "\"" };
            }
        );

        for (const auto& s : strings) {
            std::print("{}, ", s);
        }
        std::println("");

        for (const auto& n : numbers) {
            std::print("{}, ", n);
        }
        std::println("");
    }

    // ------------------------------------------------------------------

    // views can be materialized into containers
    static void views7_materialize()
    {
        auto numbers = std::list{ 1, 2, 3, 4, 5, 6 };

        auto numberToString = [](auto i) {
            return std::string{ "\"" } + std::to_string(i) + std::string{ "\"" };
        }; 

        auto strings = numbers | std::ranges::views::transform(numberToString);

        std::vector<std::string> vec{};

        std::ranges::copy(strings, std::back_inserter(vec));

        for (const auto& n : vec) {
            std::print("{}, ", n);
        }
        std::println("");

        vec.clear();

        vec = std::ranges::to<std::vector<std::string>>(strings);

        for (const auto& n : vec) {
            std::print("{}, ", n);
        }
        std::println("");
    }

    // ------------------------------------------------------------------

    // views are lazy evaluated (sort doesn't work)
    static void views8_lazy_evaluation_01()
    {
        auto vec = std::vector{ 4, 2, 7, 1, 2, 6, 3, 5 };
        for (auto&& s : vec) {
            std::print("{} ", s);
        }
        std::println("");

        // filter range
        auto isOdd = [](auto i) { return i % 2 == 1; };

        auto odd_numbers = vec | std::ranges::views::filter(isOdd);

        // std::ranges::sort(odd_numbers); // doesn't compile !!!

        // materialize the view before sorting
        auto result = std::ranges::to<std::vector<int>>(odd_numbers);
        for (auto&& s : result) {
            std::print("{} ", s);
        }
        std::println("");

        // sort range
        std::ranges::sort(result);
        for (auto&& s : result) {
            std::print("{} ", s);
        }
        std::println("");
    }

    static void views8_lazy_evaluation_02()
    {
        auto vec = std::vector{ 8, 6, 10, 9, 2, 1, 3, 7, 4, 5 };
        for (auto&& s : vec) {
            std::print("{}, ", s);
        }
        std::println("");

        // filter range
        auto firstHalf = vec | std::ranges::views::take(vec.size());
        std::ranges::sort(firstHalf);
        for (auto&& s : firstHalf) {
            std::print("{}, ", s);
        }
        std::println("");

        // filter range again
        std::ranges::sort(firstHalf, std::greater<>{});
        for (auto&& s : firstHalf) {
            std::print("{}, ", s);
        }
        std::println("");
    }

    // ------------------------------------------------------------------

    // range factories
    static void printRange(std::string_view msg, auto&& range)
    {
        std::println("{}", msg);

        for (const auto& elem : range) {
            std::println("# {}", elem);
        }
        std::println("");
    }

    static void views9_rangeFactories_01()
    {
        auto range1{ std::ranges::iota_view{ 10 } };
        auto range2{ std::ranges::views::iota(10) };
    }

    static void views9_rangeFactories_02()
    {
        // create an infinite sequence of the numbers 10, 11, 12, ...
        auto values{ std::ranges::views::iota(10) };

        auto isEven = [](const auto& value) { return value % 2 == 0; };

        // filter out all odd values, leaving only the even values
        auto result1 {
            values | std::ranges::views::filter(isEven)
        };

        auto powerTwo = [](const auto& value) { return value * 2.0; };

        // transform all values to their double value
        auto result2 {
            result1 | std::ranges::views::transform(powerTwo)
        };

        // take only the first five elements
        auto result3 {
            result2 | std::ranges::views::take(5)
        };

        printRange("Result: ", result3);
    }

    static void views9_rangeFactories_03()
    {
        // create an infinite sequence of the numbers 1, 2, 3, 4, .....
        auto values{ std::ranges::views::iota(1) };

        // printRange("Result: ", values);  // infinite loop !!!
    }

    static void views9_rangeFactories_04()
    {
        // same as views9_rangeFactories_02

        auto range = std::ranges::views::iota(10);

        auto result { range
            | std::ranges::views::filter([](const auto& value) { return value % 2 == 0; })
            | std::ranges::views::transform([](const auto& value) { return value * 2.0; })
            | std::ranges::views::take(5)
        };

        printRange("Result: ", result);
    }

    static void views9_rangeFactories_05()
    {
        // repeating view
        auto view = std::ranges::views::repeat(123, 5);

        printRange("Result of std::ranges::views::repeat: ", view);
    }

    static void views9_rangeFactories_06()
    {
        // empty view
        auto view = std::ranges::views::empty<int>;

        printRange("Result of std::ranges::views::empty: ", view);

        std::println("size: {}", view.size());
    }

    static void views9_rangeFactories_07()
    {
        constexpr std::ranges::single_view sv1{ 123 };

        static_assert(not sv1.empty());

        std::println("1) *sv1.data():  {}", *sv1.data());
        std::println("2) *sv1.begin(): {}", *sv1.begin());
        std::println("3)  sv1.size():  {}", sv1.size());
        std::println("4)  distance():  {}", std::distance(sv1.begin(), sv1.end()));
    }

    static void views9_rangeFactories_08()
    {
        std::println("Type integers, an integer >= 5 stops the program.");

        auto range = std::ranges::istream_view<int>{ std::cin };

        auto view = range
            | std::ranges::views::take_while([](const auto& v) { return v < 5; })
            | std::ranges::views::transform([](const auto& v) { return v * 2; });

        for (auto value : view) {
            std::println("> {}", value);
        }

        std::println("Terminating...");
    }

    // ------------------------------------------------------------------

    // std::ranges::views::common
    static void views10_views_common_01()
    {
        std::vector<int> vec{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };

        auto range = vec
            | std::ranges::views::take_while([](int x) { return x > 5; });

        // doesn't compile !!!
        // auto result = std::accumulate(std::begin(range), std::end(range), 0);
    }

    static void views10_views_common_02()
    {
        std::vector<int> vec{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };

        auto range = vec
            | std::ranges::views::take_while([](int x) { return x > 5; })
            | std::ranges::views::common;

        auto result = std::accumulate(std::begin(range), std::end(range), 0);

        std::println("{}", result);
    }

    static void views10_views_common_03()
    {
        std::vector<int> vec{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };

        auto range = vec
            | std::ranges::views::filter([](int x) { return x > 5; });

        // compiles :)
        auto result = std::accumulate(std::begin(range), std::end(range), 0);

        std::println("{}", result);
    }

    // ------------------------------------------------------------------

    // borrowed iterators
    static std::vector<int> getData()
    {
        return std::vector<int> { 1, 2, 3, 4, 5};
    }

    static void views11_borrowedIterators_01()
    {
        auto pos = std::ranges::find(getData(), 123); // returns iterator to temporary vector

        // temporary vector returned by getData() is destroyed here

        // std::cout << *pos;    // OOPS: using a dangling iterator // DOES NOT COMPILE !!!
    }
}

// ------------------------------------------------------------------

void ranges_02_views()
{
    using namespace Cpp20Views;

    views1_17_motivation();
    views1_23_motivation();
    
    views2_introduction_01();
    views2_introduction_02();
    views2_introduction_03();
    views2_introduction_04();
    views2_introduction_05();
    
    views3_composable();

    views4_projections_01();
    views4_projections_02();
    views4_projections_03();

    views5_range_adaptors();

    views6_immutable();

    views7_materialize();

    views8_lazy_evaluation_01();
    views8_lazy_evaluation_02();

    views9_rangeFactories_01();
    views9_rangeFactories_02();
    views9_rangeFactories_03();
    views9_rangeFactories_04();
    views9_rangeFactories_05();
    views9_rangeFactories_06();
    views9_rangeFactories_07();
    views9_rangeFactories_08();

    views10_views_common_01();
    views10_views_common_02();
    views10_views_common_03();

    views11_borrowedIterators_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
