// ===========================================================================
// Coroutines04_Awaiter_Awaitable.cpp
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
#include <chrono>
#include <thread>

// ===========================================================================

// minimalistic approach
// Gajendra Gulgulia
namespace Coroutines_Awaiter_Awaitable_01_Simplest_Variant
{
    struct Generator {
        struct promise_type {
            promise_type() = default;
            Generator get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() {}
        };
    };

    Generator myCoroutine() {
        std::cout << "before coroutine" << std::endl;
        co_await std::suspend_never{};
        std::cout << "after coroutine" << std::endl;;
    }

    static void test_01() {
        myCoroutine();
    }
}

// Gajendra Gulgulia
namespace Coroutines_Awaiter_Awaitable_02_Simplest_Variant_Instrumented
{
    struct Generator {

        struct promise_type {

            promise_type() { 
                std::cout << "  c'tor promise_type" << std::endl;
            }

            ~promise_type() {
                std::cout << "    ~promise_type" << std::endl;
            }

            Generator get_return_object() {
                std::cout << "    get_return_object" << std::endl;
                return {};
            }

            std::suspend_never initial_suspend() {
                std::cout << "    initial_suspend" << std::endl;
                return {};
            }

            std::suspend_never final_suspend() noexcept {
                std::cout << "    final_suspend" << std::endl;
                return {};
            }

            void return_void() {
                std::cout << "    return_void" << std::endl;
            }

            void unhandled_exception() { }
        };
    };

    Generator myCoroutine() {

        std::cout << "First Hello from coroutine\n";
        co_await std::suspend_never{};     // never suspend the coroutine at this point
        // co_await std::suspend_always{}; //suspend the coroutine at this point
        std::cout << "Second Hello from coroutine\n";
    }

    static void test_02() {
        myCoroutine();
    }
}

// ===========================================================================

// minimalistic approach
// Simon Toth
namespace Coroutines_Awaiter_Awaitable_03_Awaitable
{
    struct Sleeper {

        bool await_ready() const noexcept { return false; }

        void await_suspend(std::coroutine_handle<> handle) const {
            auto t = std::jthread([handle, duration = m_duration] {
                std::this_thread::sleep_for(duration);
                handle.resume();
                }
            );
        }

        void await_resume() const noexcept {}

        const std::chrono::duration<int, std::milli> m_duration;
    };

    struct Generator {
        struct promise_type {
            Generator get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() { }
        };
    };

    Generator myCoroutine() {
        using namespace std::chrono_literals;
        auto before = std::chrono::steady_clock::now();
        std::cout << "Going to sleep on thread " <<
            std::this_thread::get_id() << std::endl;

        co_await Sleeper{ 5000ms };

        auto after = std::chrono::steady_clock::now();
        std::cout << "Slept for " << (after - before) / 1ms << " ms" << std::endl;
        std::cout << "Now on thread " << std::this_thread::get_id() << std::endl;
    }

    static void test_03()
    {
        myCoroutine();
    }
}

// Simon Toth
namespace Coroutines_Awaiter_Awaitable_04_Awaitable_Instrumented
{
    struct Sleeper {

        bool await_ready() const noexcept {
            std::cout << "  Sleeper::await_ready" << std::endl;
            return false;
        }

        void await_suspend(std::coroutine_handle<> h) const {
            std::cout << "  Sleeper::await_suspend" << std::endl;
            auto t = std::jthread([h, l = length] {
                std::cout << "  Sleeper::jthread (1)" << std::endl;
                std::this_thread::sleep_for(l);
                std::cout << "  Sleeper::jthread (2)" << std::endl;
                h.resume();
                std::cout << "  Sleeper::jthread (3)" << std::endl;
                }
            );
        }

        void await_resume() const noexcept {
            std::cout << "  Sleeper::await_resume" << std::endl;
        }

        const std::chrono::duration<int, std::milli> length;
    };

    struct Generator {

        struct promise_type {
            Generator get_return_object() {
                std::cout << "  promise_type::get_return_object" << std::endl;
                return {};
            }
            std::suspend_never initial_suspend() {
                std::cout << "  promise_type::initial_suspend" << std::endl;
                return {};
            }
            std::suspend_never final_suspend() noexcept {
                std::cout << "  promise_type::final_suspend" << std::endl;
                return {};
            }
            void return_void() {
                std::cout << "  promise_type::return_void" << std::endl;
            }
            void unhandled_exception() {
                std::cout << "  promise_type::unhandled_exception" << std::endl;
            }
        };

        Generator() {
            std::cout << "c'tor Generator" << std::endl;
        }

        ~Generator() {
            std::cout << "~Generator" << std::endl;
        }
    };

    Generator myCoroutine() {
        std::cout << "myCoroutine starts" << std::endl;
        using namespace std::chrono_literals;
        auto before = std::chrono::steady_clock::now();
        co_await Sleeper{ 5000ms };
        auto after = std::chrono::steady_clock::now();
        std::cout << "myCoroutine done: slept for " << (after - before) / 1ms << " ms\n";
    }

    static void test_04() {
        myCoroutine();
    }
}

// Vishal Chovatiya
namespace Coroutines_Awaiter_Awaitable_05_Terminology
{
    struct HelloWorldCoro {
        struct promise_type {
            HelloWorldCoro get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() { }
        };
    };

    // Awaitable

    struct dummy { // Awaitable
        std::suspend_never operator co_await() { return {}; }
    };

    HelloWorldCoro printHelloWorld() {
        std::cout << "Hello ";
        co_await dummy{};
        std::cout << "World!" << std::endl;
    }

    static void test_05() {
        printHelloWorld();
    }

    // Awaiter

    struct my_awaiter {
        bool await_ready() { return true; }  // or false
        void await_suspend(std::coroutine_handle<>) {}
        void await_resume() {}
    };

    HelloWorldCoro printHelloWorldEx() {
        std::cout << "Hello (2) ";
        co_await my_awaiter{};
        std::cout << "World (2) !" << std::endl;
    }

    static void test_06() {
        printHelloWorldEx();
    }

    // co_await

    struct HelloWorldCoroExEx {
        struct promise_type {
            HelloWorldCoroExEx get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() { }
            auto await_transform(const dummy&) { return std::suspend_never{}; }
        };
    };

    HelloWorldCoroExEx printHelloWorldExEx() {
        std::cout << "Hello (3) ";
        co_await dummy{};
        std::cout << "World (3) !" << std::endl;
    }

    static void test_07() {
        printHelloWorldExEx();
    }
}

// ===========================================================================

void coroutines_04()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_Awaiter_Awaitable_01_Simplest_Variant;
    test_01();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_Awaiter_Awaitable_02_Simplest_Variant_Instrumented;
    test_02();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_Awaiter_Awaitable_03_Awaitable;
    test_03();
    std::cout << "Done." << std::endl;
    std::cout << std::endl;

    using namespace Coroutines_Awaiter_Awaitable_04_Awaitable_Instrumented;
    test_04();
    std::cout << "Done." << std::endl;

    using namespace Coroutines_Awaiter_Awaitable_05_Terminology;
    test_05();
    test_06();
    test_07();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
