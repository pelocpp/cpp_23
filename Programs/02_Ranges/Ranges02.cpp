// ===========================================================================
// Ranges02.cpp
// ===========================================================================

// https://mariusbancila.ro/blog/2019/01/20/cpp-code-samples-before-and-after-ranges/

#include <iostream>
#include <string_view>
#include <ranges>
#include <vector>
#include <algorithm>
//
//std::string to_roman(int value)
//{
//    std::vector<std::pair<int, std::string_view>> roman
//    {
//       { 1000, "M" }, { 900, "CM" },
//       { 500,  "D" }, { 400, "CD" },
//       { 100,  "C" }, { 90,  "XC" },
//       { 50,   "L" }, { 40,  "XL" },
//       { 10,   "X" }, { 9,   "IX" },
//       { 5,    "V" }, { 4,   "IV" },
//       { 1,    "I" }
//    };
//
//    std::string result;
//
//    for (auto const& [d, r] : roman)
//    {
//        while (value >= d)
//        {
//            result += r;
//            value -= d;
//        }
//    }
//
//    return result;
//}
//
//std::vector<int> v{ 1, 1, 2, 3, 5, 8, 13, 21, 34 };
//
//auto print_elem = [](auto const e) { std::cout << e << ' '; };
//
//auto is_even = [](auto const i) {return i % 2 == 0; };
//
//// print the all the elements of a range
//void example_ranges_02_01_old() {
//
//    std::for_each(std::cbegin(v), std::cend(v), print_elem);
//    std::cout << std::endl;
//
//    // or
//
//    for (auto const i : v)
//    {
//        print_elem(i);
//    };
//    std::cout << std::endl;
//}
//
//void example_ranges_02_01_new() {
//    std::ranges::for_each(std::cbegin(v), std::cend(v), print_elem);
//    std::cout << std::endl;
//    // or
//
//    std::ranges::for_each(std::as_const(v), print_elem);
//    std::cout << std::endl;
//}
//
//// print all the elements of a range in reverse order
//void example_ranges_02_02_old() {
//
//    std::for_each(std::crbegin(v), std::crend(v), print_elem);
//    std::cout << std::endl;
//}
//
//void example_ranges_02_02_new() {
//
//    std::ranges::for_each(std::crbegin(v), std::crend(v), print_elem);
//    std::cout << std::endl;
//
//    // or
//
//    for (auto const i : v | std::views::reverse)
//    {
//        print_elem(i);
//    };
//    std::cout << std::endl;
//}
//
//// print only the even elements of the range but in reverse order
//void example_ranges_02_03_old() {
//
//    std::for_each(
//        std::crbegin(v), 
//        std::crend(v),
//        [&](auto const i) {
//            if (i % 2 == 0)
//                print_elem(i);
//        }
//    );
//    std::cout << std::endl;
//}
//
//void example_ranges_02_03_new() {
//
//    for (auto const i : v | std::views::reverse | std::views::filter(is_even)) {
//        print_elem(i);
//    };
//    std::cout << std::endl;
//}
//
//// skip the first two elements of the range
//// and print only the even numbers of the next three in the range
//void example_ranges_02_04_old() {
//    auto it = std::cbegin(v);
//    std::advance(it, 2);
//    int idx = 0;
//    while (it != std::cend(v) && idx < 3)
//    {
//        if (is_even(*it))
//            print_elem(*it);
//        it++;
//        idx++;
//    }
//    std::cout << std::endl;
//}
//
//void example_ranges_02_04_new() {
//    std::cout << std::endl;
//
//    for (auto const i : v
//        | std::views::drop(2)
//        | std::views::take(3)
//        | std::views::filter(is_even))
//    {
//        print_elem(i);
//    };
//    std::cout << std::endl;
//}
//
//void example_ranges_02()
//{
//    example_ranges_02_01_old();
//    example_ranges_02_01_new();
//    example_ranges_02_02_old();
//    example_ranges_02_02_new();
//    example_ranges_02_03_old();
//    example_ranges_02_03_new();
//    example_ranges_02_04_old();
//    example_ranges_02_04_new();
//}

// ===========================================================================
// End-of-File
// ===========================================================================
