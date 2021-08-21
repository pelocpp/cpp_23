// ===========================================================================
// Ranges01.cpp
// ===========================================================================

// https://hannes.hauswedell.net/post/2019/11/30/range_intro/

#include <iostream>
#include <ranges>
#include <vector>
#include <algorithm>
//
//void example_ranges_01_a()
//{
//    std::vector<int> vec{ 6, 2, 3, 5, 8, 9, 4, 7, 0, 1  };
//
//    std::sort(vec.begin(), vec.end());
//    for (int n : vec) 
//        std::cout << n << ' ';
//    std::cout << std::endl;
//}
//
//void example_ranges_01_b()
//{
//    std::vector<int> vec{ 6, 2, 3, 5, 8, 9, 4, 7, 0, 1 };
//
//    std::sort(vec.begin() + 5, vec.end());
//    std::sort(vec.rbegin(), vec.rend());
//    std::sort(vec.rbegin() + 5, vec.rend());
//
//    for (int n : vec)
//        std::cout << n << ' ';
//    std::cout << std::endl;
//}
//
//void example_ranges_01_c()
//{
//    std::vector<int> vec{ 6, 2, 3, 5, 8, 9, 4, 7, 0, 1 };
//
//    std::ranges::sort(std::views::drop(vec, 5));
//    std::ranges::sort(std::views::reverse(vec));
//    std::ranges::sort(std::views::drop(std::views::reverse(vec), 5));
//
//    for (int n : vec)
//        std::cout << n << ' ';
//    std::cout << std::endl;
//}
//
//void example_ranges_01_d()
//{
//    std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };
//    auto view = std::views::reverse(vec);
//    std::cout << *view.begin() << std::endl;
//}
//
//void example_ranges_01_e()
//{
//    std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };
//    std::ranges::reverse_view<std::ranges::ref_view<std::vector<int>>> view = 
//        std::views::reverse(vec);
//    std::cout << *view.begin() << std::endl;
//}
//
//void example_ranges_01_f()
//{
//    std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };
//    auto view = vec | std::views::reverse | std::views::drop(2);
//    std::cout << *view.begin() << std::endl;
//}
//
//void example_ranges_01_g()
//{
//    std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };
//    auto view = std::views::drop(std::views::reverse(vec), 2);
//    std::cout << *view.begin() << std::endl;
//}
//
//void example_ranges_01_h()
//{
//    std::vector<int> vec{ 1, 5, 6, 8, 5 };
//
//    auto square = [](int const i) {
//        std::cout << "calculate square of " << i << std::endl;
//        return i * i; 
//    };
//
//    auto view = vec | std::views::transform(square) | std::views::drop(2);
//    std::cout << *std::begin(view) << std::endl;
//}
//
//void example_ranges_01()
//{
//    example_ranges_01_a();
//    example_ranges_01_b();
//    example_ranges_01_c();
//    example_ranges_01_d();
//    example_ranges_01_e();
//    example_ranges_01_f();
//    example_ranges_01_g();
//    example_ranges_01_h();
//}

// ===========================================================================
// End-of-File
// ===========================================================================
