// ===========================================================================
// Coroutines_05_Iterators.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <coroutine>
#include <chrono>
#include <thread>

// ===========================================================================

// inspired from
// https://itnext.io/c-20-ranges-complete-guide-4d26e3511db0

namespace Coroutines_Iterators_FizzBuzz
{
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
            // Disallow co_await in generator coroutines.
            void await_transform() = delete;
            void return_void() {}
            [[noreturn]]
            static void unhandled_exception() {
                throw;
            }

            std::optional<T> current_value;
        };

        using Handle = std::coroutine_handle<promise_type>;

        explicit Generator(const Handle coroutine) :
            m_handle{ coroutine }
        {}

        Generator() = default;
        ~Generator() {
            if (m_handle) {
                m_handle.destroy();
            }
        }

        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& other) noexcept :
            m_handle{ other.m_handle }
        {
            other.m_handle = {};
        }
        Generator& operator=(Generator&& other) noexcept {
            if (this != &other) {
                if (m_handle) {
                    m_handle.destroy();
                }
                m_handle = other.m_handle;
                other.m_handle = {};
            }
            return *this;
        }

        T next() {
            if (m_handle) {
                m_handle.resume();
                return m_handle.promise().current_value.value();
            }
            else {
                return {};
            }
        }

        // Range-based for loop support.
        class Iter {
        public:
            void operator++() {
                m_handle.resume();
            }
            const T& operator*() const {
                return *m_handle.promise().current_value;
            }
            bool operator==(std::default_sentinel_t) const {
                return !m_handle || m_handle.done();
            }

            explicit Iter(const Handle handle) : m_handle{ handle } {}

        private:
            Handle m_handle;
        };

        Iter begin() {
            if (m_handle) {
                m_handle.resume();
            }
            return Iter{ m_handle };
        }

        std::default_sentinel_t end() {
            return {};
        }

    private:
        Handle m_handle;
    };

    // =========================================

    // coroutine
    Generator<std::string> generator()
    {
        co_yield std::string{ "The" };
        co_yield std::string{ "quick" };
        co_yield std::string{ "brown " };
        co_yield std::string{ "fox" };
        co_yield std::string{ "jumps " };
        co_yield std::string{ "over" };
        co_yield std::string{ "the" };
        co_yield std::string{ "lazy" };
        co_yield std::string{ "dog" };
        co_return;
    }

    void test_01()
    {
        auto coroutine = generator();

        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << std::endl;
    }

    void test_02()
    {
        auto coroutine = generator();
        std::string s{};
        while ((s = coroutine.next()) != std::string{}) {
            std::cout << s << std::endl;
        }
        std::cout << std::endl;
    }

    void test_03()
    {
        for (const auto& next : generator()) {
            std::cout << next << std::endl;
        }
    }

    // =========================================

    Generator<std::string> fizzbuzz()
    {
        for (int i = 1; ; i++) {
            std::string result{};
            if (i % 3 == 0) 
                result += "Fizz";
            if (i % 5 == 0)
                result += "Buzz";
            if (result.empty())
                co_yield std::to_string(i);
            else
                co_yield result;
        }
    }

    void test_04()
    {
        auto coroutine = fizzbuzz();

        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << std::endl;
    }

    void test_05()
    {
        auto coroutine = fizzbuzz();

        // endless loop
        for (int n{}; const auto & next : coroutine) {
            std::cout << next << std::endl;
            if (++n >= 20) break;
        }
        std::cout << std::endl;
    }
}

// ===========================================================================

void coroutines_05()
{
    using namespace Coroutines_Iterators_FizzBuzz;
    test_01();
    test_02();
    test_03();
    test_04();
    test_05();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
