// ===========================================================================
// Coroutines_01_Introduction.cpp
// ===========================================================================

#include <coroutine>
#include <future>
#include <generator>
#include <optional>
#include <vector>
#include <print>

namespace Coroutines_Motivation
{
    static std::vector<int> getNumbers(int begin, int end)
    {
        std::vector<int> numbers;

        for (int i = begin; i <= end; ++i) {
            numbers.push_back(i);
        }

        return numbers;
    }

    static void motivation_01()
    {
        std::vector<int> numbers = getNumbers(1, 10);

        for (auto n : numbers) {
            std::print("{} ", n);
        }
        std::println();
    }
}

namespace Coroutines_SimpleExamples
{
    static std::generator<char> hello() {
        co_yield 'h';
        co_yield 'e';
        co_yield 'l';
        co_yield 'l';
        co_yield 'o';
        co_return;
    }

    static std::generator<long long> fibonacci() {
        long long a = 0;
        long long b = 1;
        while (true) {
            co_yield b;
            auto tmp = a;
            a = b;
            b += tmp;
        }
    }

    static void simple_coroutine_01() {

        for (auto ch : hello()) {
            std::println("{}", ch);
        }

        std::println();
    }

    static void simple_coroutine_02() {

        for (auto i : fibonacci()) {
            if (i > 1'000'000)
                break;

            std::println("{}", i);
        }

        std::println();
    }
}

// ===============================================================

void coroutines_01()
{
    using namespace Coroutines_Motivation;
    motivation_01();

    using namespace Coroutines_SimpleExamples;
    simple_coroutine_01();
    simple_coroutine_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
