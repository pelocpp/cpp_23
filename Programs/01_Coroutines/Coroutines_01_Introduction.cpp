// ===========================================================================
// Coroutines01.cpp
// ===========================================================================

#include <iostream>
#include <vector>
#include <future>
#include <coroutine>
#include <optional>

namespace Coroutines_Motivation
{
    std::vector<int> getNumbers(int begin, int end)
    {
        std::vector<int> numbers;

        for (int i = begin; i <= end; ++i) {
            numbers.push_back(i);
        }

        return numbers;
    }

    void motivation_01()
    {
        std::vector<int> numbers = getNumbers(1, 10);

        for (auto n : numbers) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
}

namespace Coroutines_SimpleExamples
{
    // from https://en.cppreference.com/w/cpp/coroutine/coroutine_handle
    template<std::movable T>
    class Generator {
    public:
        struct promise_type {
            Generator<T> get_return_object() {
                return Generator{ Handle::from_promise(*this) };
            }
            static std::suspend_always initial_suspend() noexcept {
                return {};
            }
            static std::suspend_always final_suspend() noexcept {
                return {};
            }
            std::suspend_always yield_value(T value) noexcept {
                current_value = std::move(value);
                return {};
            }
            void return_void() {}

            // Disallow co_await in generator coroutines.
            void await_transform() = delete;
            [[noreturn]]
            static void unhandled_exception() {
                throw;
            }

            std::optional<T> current_value;
        };

        using Handle = std::coroutine_handle<promise_type>;

        explicit Generator(const Handle coroutine) :
            m_coroutine{ coroutine }
        {}

        Generator() = default;
        ~Generator() {
            if (m_coroutine) {
                m_coroutine.destroy();
            }
        }

        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& other) noexcept :
            m_coroutine{ other.m_coroutine }
        {
            other.m_coroutine = {};
        }
        Generator& operator=(Generator&& other) noexcept {
            if (this != &other) {
                if (m_coroutine) {
                    m_coroutine.destroy();
                }
                m_coroutine = other.m_coroutine;
                other.m_coroutine = {};
            }
            return *this;
        }

        // Range-based for loop support.
        class Iter {
        public:
            void operator++() {
                m_coroutine.resume();
            }
            const T& operator*() const {
                return *m_coroutine.promise().current_value;
            }
            bool operator==(std::default_sentinel_t) const {
                return !m_coroutine || m_coroutine.done();
            }

            explicit Iter(const Handle coroutine) :
                m_coroutine{ coroutine }
            {}

        private:
            Handle m_coroutine;
        };

        Iter begin() {
            if (m_coroutine) {
                m_coroutine.resume();
            }
            return Iter{ m_coroutine };
        }
        std::default_sentinel_t end() {
            return {};
        }

    private:
        Handle m_coroutine;
    };

    // ===========================================================================

    template<std::integral T>
    Generator<T> range(T first, const T last) {
        while (first < last) {
            co_yield first++;
        }
    }

    // ===========================================================================

    Generator<char> hello() {
        co_yield 'h';
        co_yield 'e';
        co_yield 'l';
        co_yield 'l';
        co_yield 'o';
    }

    Generator<long long> fibonacci() {
        long long a = 0;
        long long b = 1;
        while (true) {
            co_yield b;
            auto tmp = a;
            a = b;
            b += tmp;
        }
    }

    void simple_coroutine_01() {

        for (auto ch : hello()) {
            std::cout << ch << std::endl;
        }

        std::cout << std::endl;
    }

    void simple_coroutine_02() {

        for (auto i : fibonacci()) {
            if (i > 1'000'000)
                break;

            std::cout << i << std::endl;
        }

        std::cout << std::endl;
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
