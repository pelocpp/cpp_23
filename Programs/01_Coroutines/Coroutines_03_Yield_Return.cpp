// ===========================================================================
// Coroutines03_MinimalisticApproach.cpp
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

#include <iostream>
#include <string>
#include <coroutine>

// ===========================================================================

// minimalistic approach
// Simon Toth
namespace Coroutines_MinimalisticApproach_01_Simplest_Variant
{
    struct Generator {

        struct promise_type {
            Generator get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() { }
        };
    };

    static Generator myCoroutine() {
        co_return; // make it a coroutine
    }

    static void test_01()
    {
        Generator t = myCoroutine();
    }
}

// ===========================================================================

// a) instrumented
// b) demonstrating 'std::suspend_never' vs 'std::suspend_always'

namespace Coroutines_MinimalisticApproach_02_Simplest_Variant_Instrumented
{
    struct Generator {

        struct promise_type {

            promise_type() {
                std::cout << "  c'tor promise" << std::endl;
            }

            ~promise_type() {
                std::cout << "  ~promise" << std::endl;
            }

            Generator get_return_object() {
                std::cout << "  get_return_object" << std::endl;
                return {};
            }

            std::suspend_never initial_suspend() {  // <== change here: std::suspend_always
                std::cout << "  initial_suspend" << std::endl;
                return {};
            }

            std::suspend_never final_suspend() noexcept {
                std::cout << "  final_suspend" << std::endl;
                return {};
            }

            void return_void() {
                std::cout << "  return_void" << std::endl;
            }

            void unhandled_exception() {
                std::cout << "  unhandled_exception" << std::endl;
            }
        };

        Generator() {
            std::cout << "c'tor Generator" << std::endl;
        }

        ~Generator() {
            std::cout << "~Generator" << std::endl;
        }
    };

    static Generator myCoroutine() {
        std::cout << "inside coroutine" << std::endl;
        co_return; // make it a coroutine
    }

    static void test_02()
    {
        std::cout << "before coroutine call" << std::endl;
        Generator t = myCoroutine();
        std::cout << "after coroutine call" << std::endl;
    }
}

// ===========================================================================

// a) instrumented
// b) demonstrating 'std::suspend_never' vs 'std::suspend_always'

namespace Coroutines_MinimalisticApproach_03_CoroutineHandle
{
    struct Generator {

        struct promise_type {

            using Handle = std::coroutine_handle<promise_type>;

            Generator get_return_object() {
                std::cout << "  get_return_object" << std::endl;
                return Generator{ Handle::from_promise(*this) };
            }

            promise_type() {
                std::cout << "  c'tor promise" << std::endl;
            }

            ~promise_type() {
                std::cout << "  ~promise" << std::endl;
            }

            std::suspend_always initial_suspend() {  // <== change here: std::suspend_always
                std::cout << "  initial_suspend" << std::endl;
                return {};
            }

            std::suspend_never final_suspend() noexcept {
                std::cout << "  final_suspend" << std::endl;
                return {};
            }

            void return_void() {
                std::cout << "  return_void" << std::endl;
            }

            void unhandled_exception() {
                std::cout << "  unhandled_exception" << std::endl;
            }
        };

        explicit Generator(promise_type::Handle coro) : m_coro{ coro } {}

        void destroy() { m_coro.destroy(); }
        void resume() { m_coro.resume(); }

    private:
        promise_type::Handle m_coro;

    public:
        Generator() {
            std::cout << "c'tor Generator" << std::endl;
        }

        ~Generator() {
            std::cout << "~Generator" << std::endl;
        }

        // making Generator move-only:
        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& t) noexcept : m_coro(t.m_coro) { t.m_coro = {}; }
        Generator& operator=(Generator&& t) noexcept {
            if (this == &t) {
                return *this;
            }
            if (m_coro) {
                m_coro.destroy();
            }
            m_coro = t.m_coro;
            t.m_coro = {};
            return *this;
        }
    };

    static Generator myCoroutine() {
        std::cout << "inside coroutine" << std::endl;
        co_return; // make it a coroutine
    }

    static void test_03()
    {
        std::cout << "before coroutine call" << std::endl;
        Generator t = myCoroutine();
        std::cout << "after coroutine call" << std::endl;
        t.resume();
        // t.destroy();
    }
}

namespace Coroutines_MinimalisticApproach_04_Generator
{
    struct Generator {

        struct promise_type {

            using Handle = std::coroutine_handle<promise_type>;

            Generator get_return_object() {
                return Generator{ Handle::from_promise(*this) };
            }

            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            std::suspend_always yield_value(int value) {
                current_value = value;
                return {};
            }

            void return_void() {}

            void unhandled_exception() { }

            int current_value;
        };

        explicit Generator(promise_type::Handle coro) : m_coro{ coro } {}

        ~Generator() {
            if (m_coro) m_coro.destroy();
        }

        // make move-only
        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& t) noexcept : m_coro(t.m_coro) { t.m_coro = {}; }
        Generator& operator=(Generator&& t) noexcept {
            if (this == &t) return *this;
            if (m_coro) m_coro.destroy();
            m_coro = t.m_coro;
            t.m_coro = {};
            return *this;
        }

        int get_next() {
            m_coro.resume();
            return m_coro.promise().current_value;
        }

    private:
        promise_type::Handle m_coro;
    };

    static Generator myCoroutine() {
        int x = 0;
        while (true) {
            co_yield x++;
        }
    }

    static void test_04()
    {
        Generator c = myCoroutine();
        int x = 0;
        while ((x = c.get_next()) < 10) {
            std::cout << x << std::endl;
        }
    }
}

namespace Coroutines_MinimalisticApproach_05_Generator_Instrumented
{
    struct Generator {

        struct promise_type {

            promise_type() : m_currentValue{} {
                std::cout << "  c'tor promise" << std::endl;
            }

            ~promise_type() {
                std::cout << "  ~promise" << std::endl;
            }

            using Handle = std::coroutine_handle<promise_type>;

            Generator get_return_object() {
                std::cout << "  get_return_object" << std::endl;
                return Generator{ Handle::from_promise(*this) };
            }

            std::suspend_always initial_suspend() {
                std::cout << "  suspend_always" << std::endl;
                return {};
            }

            std::suspend_always final_suspend() noexcept {
                std::cout << "  final_suspend" << std::endl;
                return {};
            }

            std::suspend_always yield_value(int value) {
                std::cout << "  yield_value" << std::endl;
                m_currentValue = value;
                return {};
            }

            void return_void() {
                std::cout << "  return_void" << std::endl;
            }

            void unhandled_exception() {
                std::cout << "  unhandled_exception" << std::endl;
            }

            int m_currentValue;
        };

        explicit Generator(promise_type::Handle coro) : m_coro{ coro } {
            std::cout << "c'tor Generator" << std::endl;
        }

        ~Generator() {
            std::cout << "~Generator" << std::endl;
            if (m_coro) m_coro.destroy();
        }

        // make move-only
        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& t) noexcept : m_coro(t.m_coro) {
            t.m_coro = {};
        }

        Generator& operator=(Generator&& t) noexcept {
            if (this == &t) return *this;
            if (m_coro) m_coro.destroy();
            m_coro = t.m_coro;
            t.m_coro = {};
            return *this;
        }

        int get_next() {
            std::cout << "get_next" << std::endl;
            m_coro.resume();

            const auto& promise = m_coro.promise();
            auto value = promise.m_currentValue;
            return value;
        }

    private:
        promise_type::Handle m_coro;
    };

    static Generator myCoroutine() {
        int x = 0;
        while (true) {
            std::cout << "  before co_yield ..." << std::endl;
            co_yield x++;
        }
    }

    static void test_05() {
        Generator c = myCoroutine();
        int x = 0;
        while ((x = c.get_next()) < 3) {
            std::cout << x << std::endl;
        }
    }
}

// ===========================================================================

void coroutines_03()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_MinimalisticApproach_01_Simplest_Variant;
    test_01();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_MinimalisticApproach_02_Simplest_Variant_Instrumented;
    test_02();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_MinimalisticApproach_03_CoroutineHandle;
    test_03();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_MinimalisticApproach_04_Generator;
    test_04();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_MinimalisticApproach_05_Generator_Instrumented;
    test_05();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
