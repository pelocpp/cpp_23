// ===========================================================================
// Coroutines09_Scratch.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <vector>
#include <coroutine>
#include <memory>

namespace Coroutines_FirstSteps
{
    class Generator;

    struct Promise
    {
        using value_type = std::string;

        std::string m_value{};

        Promise() = default;

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() { std::rethrow_exception(std::current_exception()); }

        std::suspend_always yield_value(std::string value) {
            m_value = std::move(value);
            return {};
        }

        void return_void() {
            m_value.clear();
        }

        Generator get_return_object();
    };

    class Generator
    {
    public:
        using promise_type = Promise;

        explicit Generator(std::coroutine_handle<Promise> handle)
            : m_handle(handle)
        {}

        ~Generator() {
            if (m_handle) { m_handle.destroy(); }
        }

        Promise::value_type next() {
            if (m_handle) {
                m_handle.resume();
                return m_handle.promise().m_value;
            }
            else {
                return {};
            }
        }

    private:
        std::coroutine_handle<Promise> m_handle{};
    };

    Generator Promise::get_return_object()
    {
        return Generator{ std::coroutine_handle<Promise>::from_promise(*this) };
    }

    // coroutine
    Generator generator()
    {
        co_yield std::string{ "Hello " };
        co_yield std::string{ "World" };
        co_return;
    }

    void coroutines_hello_world_01()
    {
        auto coroutine = generator();

        std::cout << coroutine.next() << std::endl;
        std::cout << coroutine.next() << std::endl;
        std::cout << std::endl;
    }

    void coroutines_hello_world_02()
    {
        auto coroutine = generator();
        std::string s{};
        while ((s = coroutine.next()) != std::string{}) {
            std::cout << s << std::endl;
        }
    }
}

namespace Coroutines_Exercise
{
    class Generator;

    struct Promise
    {
        using value_type = int;

        int m_value{};

        Promise() = default;

        std::suspend_always initial_suspend() { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void unhandled_exception() { std::rethrow_exception(std::current_exception()); }

        std::suspend_always yield_value(int value) {
            m_value = value;
            return {};
        }

        void return_void() {
            m_value = -1;
        }

        Generator get_return_object();
    };

    class Generator
    {
    public:
        using promise_type = Promise;

        explicit Generator(std::coroutine_handle<Promise> handle)
            : m_handle(handle)
        {}

        ~Generator() {
            if (m_handle) { m_handle.destroy(); }
        }

        Promise::value_type next() {
            if (m_handle) {
                m_handle.resume();
                return m_handle.promise().m_value;
            }
            else {
                return {};
            }
        }

    private:
        std::coroutine_handle<Promise> m_handle{};
    };

    Generator Promise::get_return_object()
    {
        return Generator{ std::coroutine_handle<Promise>::from_promise(*this) };
    }

    // coroutine
    Generator generatorForNumbers(int begin, int end)
    {
        for (int i = begin; i <= end; ++i) {
            co_yield i;
        }
    }

    void coroutines_exercise()
    {
        Generator coroutine = generatorForNumbers(1, 10);

        while (true) {

            int value = coroutine.next();
            if (value == -1) {
                break;
            }
        }
    }
}

// ===============================================================

void coroutines_02()
{
    using namespace Coroutines_FirstSteps;
    //coroutines_hello_world_01();
    coroutines_hello_world_02();

    using namespace Coroutines_Exercise;
    //coroutines_exercise();
}

// ===========================================================================
// End-of-File
// ===========================================================================
