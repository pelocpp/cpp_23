// ===========================================================================
// Ranges_00_Motivation.cpp
// ===========================================================================

import std;

namespace Cpp20Ranges_Motivation
{
    static void motivation_01_errors()
    {
        std::vector<int> numbers = { 1, 4, 2, 3 };
        // std::list<int> numbers = { 1, 4, 2, 3 };

        std::sort(
            std::begin(numbers),
            std::end(numbers)
        );

        std::ranges::copy(numbers, std::ostream_iterator<int>(std::cout, ", "));
    }

    static void motivation_02_errors()
    {
        std::vector<int> numbers = { 1, 4, 2, 3 };
        // std::list<int> numbers = { 1, 4, 2, 3 };

        std::ranges::sort(numbers);

        std::ranges::copy(numbers, std::ostream_iterator<int>(std::cout, ", "));
    }

    static void motivation_02_readability()
    {
        std::vector<int> vec1{ 1, 2, 3, 4, 5 };
        std::vector<int> vec2;

        // a)
        std::copy(
            std::begin(vec1),
            std::end(vec1), 
            std::back_inserter(vec2)
        );

        for (int n : vec2) {
            std::print("{} ", n);
        }
        std::println("");

        // b)
        vec2 = vec1;

        for (int n : vec2) {
            std::print("{} ", n);
        }
        std::println("");

        // c)
        std::ranges::copy(vec1, std::back_inserter(vec2));

        for (int n : vec2) {
            std::print("{} ", n);
        }
        std::println("");
    }

    static bool is_even(int n) {
        return n % 2 == 0;
    }

    static void motivation_03_classic()
    {
        std::vector<int> numbers{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        std::vector<int> evenNumbers{};

        std::copy_if(
            std::begin(numbers),
            std::end(numbers),
            std::back_inserter(evenNumbers),
            [](int n) -> bool { return n % 2 == 0; }
        );

        std::vector<int> squares{};

        std::transform(
            std::begin(evenNumbers),
            std::end(evenNumbers),
            std::back_inserter(squares),
            [](int n) -> int { return n * n; }
        );

        for (int n : squares) { 
            std::print("{} ", n);
        }
        std::println("");
    }

    static void motivation_03_ranges()
    {
        std::vector<int> numbers{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        auto results = numbers
            | std::ranges::views::filter([](int n) -> bool { return n % 2 == 0; })
            | std::ranges::views::transform([](int n) { return n * n; });

        for (int n : results) {
            std::print("{} ", n);
        }
        std::println("");
    }

    struct Book {
        std::string m_title;
        std::string m_author;
        int m_year;
        double m_price;
    };

    static void motivation_04_classic()
    {
        std::vector<Book> books 
        {
            { "C++", "Bjarne Stroustrup", 1985, 20.55 },
            { "C", "Dennis Ritchie", 1972, 11.99 } ,
            { "Java", "James Gosling", 1995, 19.99 },
            { "C#", "Anders Hejlsberg", 2000, 29.99 }
        };

        std::sort(
            std::begin(books),
            std::end(books),
            [](const Book& book1, const Book& book2) {
                return book1.m_title < book2.m_author;
            }
        );

        for (int index{ 1 }; const auto& [title, author, year, price] : books) {
            std::println("{}: [{}] ", price, title);
            index++;
        }
        std::println("");
    }

    static void motivation_04_ranges()
    {
        std::vector<Book> books
        {
            { "C++", "Bjarne Stroustrup", 1985, 20.55 },
            { "C", "Dennis Ritchie", 1972, 11.99 } ,
            { "Java", "James Gosling", 1995, 19.99 },
            { "C#", "Anders Hejlsberg", 2000, 29.99 }
        };

        // sort books
        std::ranges::sort(books, std::less{}, &Book::m_price);

        for (int index{ 1 }; const auto & [title, author, year, price] : books) {
            std::println("{}: [{}] ", price, title);
            index++;
        }
        std::println("");

        // list all titles
        auto titlesView{ books | std::ranges::views::transform(&Book::m_title) };

        for (const auto& title : titlesView) {
            std::println("{}", title);
        }
        std::println("");
    }
}

void ranges_00_motivation()
{
    using namespace Cpp20Ranges_Motivation;

    motivation_01_errors();
    motivation_02_errors();
    motivation_02_readability();
    motivation_03_classic();
    motivation_03_ranges();
    motivation_04_classic();
    motivation_04_ranges();
}

// ===========================================================================
// End-of-File
// ===========================================================================
