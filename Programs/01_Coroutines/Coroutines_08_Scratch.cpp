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

    struct dummy { // Awaitable
        std::suspend_always operator co_await() { return {}; }  // <== DAS IST NEU !!!!!!!!!!!!!!!!!!!
    };

    HelloWorldCoro print_hello_world_02() {
        std::cout << "Hello ";
        co_await dummy{};
        std::cout << "World!" << std::endl;
    }

    void test_scratch_02() {
        HelloWorldCoro mycoro = print_hello_world_02();
        mycoro.m_handle.resume();
        mycoro.m_handle(); // Equal to mycoro.m_handle.resume();
    }

    struct my_awaiter {
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<>) {}
        void await_resume() {}
    };

    HelloWorldCoro print_hello_world_03() {
        std::cout << "Hello ";
        co_await my_awaiter{};
        std::cout << "World!" << std::endl;
    }
}


// ===========================================================================

// ===========================================================================

void coroutines_08()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //using namespace Coroutines_Scratch_01_Vishal;
    //test_scratch_01();
    //std::cout << "Done." << std::endl;


}

// ===========================================================================
// End-of-File
// ===========================================================================
