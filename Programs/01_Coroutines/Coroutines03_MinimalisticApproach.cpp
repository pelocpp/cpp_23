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

namespace Coroutines_MinimalisticApproach_01_Simplest_Variant
{
    struct Task {

        struct promise_type {
            Task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() { }
            void unhandled_exception() { }
        };
    };

    Task myCoroutine() {
        co_return; // make it a coroutine
    }

    void test_01()
    {
        Task x = myCoroutine();
    }
}

// ===========================================================================

namespace Coroutines_MinimalisticApproach_02_Simplest_Variant_Instrumented
{
    struct Task {

        struct promise_type {

            promise_type() {
                std::cout << "  c'tor promise" << std::endl;
            }

            ~promise_type() {
                std::cout << "  ~promise" << std::endl;
            }

            Task get_return_object() {
                std::cout << "  get_return_object" << std::endl;
                return {};
            }

            std::suspend_never initial_suspend() {
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

        Task() {
            std::cout << "c'tor Task" << std::endl;
        }

        ~Task() {
            std::cout << "~Task" << std::endl;
        }
    };

    Task myCoroutine() {
        std::cout << "inside coroutine" << std::endl;
        co_return; // make it a coroutine
    }

    void test_02()
    {
        std::cout << "before coroutine call" << std::endl;
        Task x = myCoroutine();
        std::cout << "after coroutine call" << std::endl;
    }
}

// ===========================================================================

void coroutines_03()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //using namespace Coroutines_MinimalisticApproach_01_Simplest_Variant;
    //test_01();
    //std::cout << "Done." << std::endl;

    using namespace Coroutines_MinimalisticApproach_02_Simplest_Variant_Instrumented;
    test_02();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
