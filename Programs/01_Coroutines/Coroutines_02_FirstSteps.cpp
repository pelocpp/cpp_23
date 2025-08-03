// ===========================================================================
// Coroutines_02_FirstSteps.cpp
// ===========================================================================

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <coroutine>
#include <memory>
#include <print>
#include <string>
#include <vector>

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
    static Generator generator()
    {
        co_yield std::string{ "Hello " };
        co_yield std::string{ "World" };
        co_return;
    }

    static void coroutines_hello_world_01()
    {
        auto coroutine = generator();

        std::println("{}", coroutine.next());
        std::println("{}", coroutine.next());
        std::println();
    }

    static void coroutines_hello_world_02()
    {
        auto coroutine = generator();
        std::string s{};
        while ((s = coroutine.next()) != std::string{}) {
            std::println("{}", s);
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
    static Generator generatorForNumbers(int begin, int end)
    {
        for (int i = begin; i <= end; ++i) {
            co_yield i;
        }
    }

    static void coroutines_exercise()
    {
        Generator coroutine = generatorForNumbers(1, 10);

        while (true) {

            int value = coroutine.next();
            if (value == -1) {
                break;
            }

            std::println("{}", value);
        }
    }
}

namespace Coroutines_CustomImplementation
{
    struct RoutinePromise;

    struct Routine
    {
        // type has to contain a 'promise_type' definition
        using promise_type = RoutinePromise;
    };

    struct RoutinePromise
    {
        // This function is used to create the instance
        // of the return type for the caller
        Routine get_return_object() { return {}; }

        // What should happen before the coroutine body starts
        std::suspend_never initial_suspend() noexcept { return {}; }

        // What should happen after the coroutine body has finished
        std::suspend_never final_suspend() noexcept { return {}; }

        // What should happen when the coroutine executes co_return;
        void return_void() {}

        // What should happen when there is an unhandled exception
        void unhandled_exception() {}
    };

    static void coroutines_custom_implementation()
    {
        //auto coro = [] -> Routine {
        //    std::println("Running...");
        //    co_return;
        //};

        //auto x = coro(); // coroutine starts and runs to completion
        //// decltype(x) == Routine
        //static_assert(std::is_same_v<decltype(x), Routine>);

        //coro(); // Because the return type is empty, this is the same as above
    }
}

// ===============================================================

void coroutines_02()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_FirstSteps;
    coroutines_hello_world_01();
    coroutines_hello_world_02();

    using namespace Coroutines_Exercise;
    coroutines_exercise();

    using namespace Coroutines_CustomImplementation;
    coroutines_custom_implementation();
}

// ===========================================================================
// End-of-File
// ===========================================================================
