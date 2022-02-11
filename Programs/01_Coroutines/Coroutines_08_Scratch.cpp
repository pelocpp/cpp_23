// ===========================================================================
// Coroutines04_Awaitable.cpp
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
//  Vishal Chovatiya
namespace Coroutines_Scratch_01_Vishal
{
    struct HelloWorldCoro {
        struct promise_type { // compiler looks for `promise_type`
            HelloWorldCoro get_return_object() { return this; }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept{ return {}; }
            void return_void() { }
            void unhandled_exception() { }
        };
        HelloWorldCoro(promise_type* p) : m_handle(std::coroutine_handle<promise_type>::from_promise(*p)) {}
        ~HelloWorldCoro() { m_handle.destroy(); }
        std::coroutine_handle<promise_type>      m_handle;
    };

    HelloWorldCoro print_hello_world() {
        std::cout << "Hello ";
        co_await std::suspend_always{};
        std::cout << "World!" << std::endl;
    }

    void test_scratch_01() {
        HelloWorldCoro mycoro = print_hello_world();
        mycoro.m_handle.resume();
        mycoro.m_handle(); // Equal to mycoro.m_handle.resume();
    }
}

// ===========================================================================

// minimalistic approach
//  Vishal Chovatiya
namespace Coroutines_Scratch_02_Gajendra
{
    struct Task {
        struct promise_type {
            promise_type() = default;
            Task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() {}
        };
    };

    Task foo() {
        std::cout << "before coroutine" << std::endl;
        co_await std::suspend_always{};
        std::cout << "after coroutine" << std::endl;;
    }

    void test_scratch_02() {
        foo();
    }
}

namespace Coroutines_Scratch_03_Gajendra_Instrumented
{
    //global counter
    int num{ 0 };

    struct Task {

        struct promise_type {

            promise_type() { std::cout << num++ << ". promise_type default ctor\n"; }

            Task get_return_object() {
                std::cout << num++ << ". promise_type::get_return_object\n";
                return {};
            }

            std::suspend_never initial_suspend() {
                std::cout << num++ << ". promise_type::initial_suspend\n";
                return {};
            }

            std::suspend_never final_suspend() noexcept {
                std::cout << num++ << ". promise_type::final_suspend\n";
                return {};
            }

            void return_void() {
                std::cout << num++ << ". promise_type::return_void\n";
            }

            void unhandled_exception() { std::cout << num++ << ".throw\n"; }
        };
    };

    Task foo() {

        std::cout << num++ << ".   hello from coroutine\n";
        co_await std::suspend_never{}; //never suspend the coroutine at this point
        //co_await std::suspend_always{}; //suspend the coroutine at this point
        std::cout << num++ << ".   hello from coroutine\n";
    }


    void test_scratch_03() {
        foo();
    }
}


// ===========================================================================

void coroutines_08()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //using namespace Coroutines_Scratch_01_Vishal;
    //test_scratch_01();
    //std::cout << "Done." << std::endl;

    //using namespace Coroutines_Scratch_02_Gajendra;
    //test_scratch_02();
    //std::cout << "Done." << std::endl;

    using namespace Coroutines_Scratch_03_Gajendra_Instrumented;
    test_scratch_03();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
