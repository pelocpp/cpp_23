// ===========================================================================
// Coroutines_21_Yield_Return.cpp
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
#include <print>

namespace Coroutines_Motivation_Generator
{
    // coroutine interface to deal with a simple task
    // - providing resume() to resume the coroutine
    class [[nodiscard]] CoroutineGen {
    public:
        struct promise_type;    // forward declaration

        using CoroutineHandle = std::coroutine_handle<promise_type>;

    private:
        CoroutineHandle m_hdl;                                   // native coroutine handle

    public:
        struct promise_type
        {
            int m_coroutineValue = 0;              // last value from co_yield

            auto get_return_object() {             // init and return the coroutine interface
                return CoroutineGen{ CoroutineHandle::from_promise(*this) };
            }

            auto initial_suspend() {               // initial suspend point
                return std::suspend_always{};      // - suspend immediately
            }

            void unhandled_exception() {           // deal with exceptions
                std::terminate();                  // - terminate the program
            }

            // reaction to co_yield
            auto yield_value(int val) {
                m_coroutineValue = val;           // - store value locally
                return std::suspend_always{};     // - suspend coroutine
            }

            void return_void() {                   // deal with the end or co_return;
            }

            auto final_suspend() noexcept {    // final suspend point
                return std::suspend_always{};  // - suspend immediately
            }
        };





        // c'tor / d'tor
        CoroutineGen(std::coroutine_handle<promise_type> hdl)
            : m_hdl{ hdl }                                       // store coroutine handle in interface
        {
        }

        ~CoroutineGen() {

            if (m_hdl) {
                m_hdl.destroy();                                // destroy coroutine handle
            }
        }

        // no copy / no move
        CoroutineGen(const CoroutineGen&) = delete;
        CoroutineGen& operator=(const CoroutineGen&) = delete;

        CoroutineGen(CoroutineGen&&) noexcept = delete;
        CoroutineGen& operator=(CoroutineGen&&) noexcept = delete;

        // API
        // - resume the coroutine
        bool resume() const {

            if (!m_hdl || m_hdl.done()) {
                return false;                       // nothing (more) to process
            }

            m_hdl.resume();                         // resume (blocks until suspended again or the end)
            return !m_hdl.done();
        }

        // - yield value from co_yield
        int getValue() const {

            return m_hdl.promise().m_coroutineValue;
        }
    };

    //struct CoroutineGen::promise_type
    //{
    //    int m_coroutineValue = 0;              // last value from co_yield

    //    auto get_return_object() {             // init and return the coroutine interface
    //        return CoroutineGen{ CoroutineHandle::from_promise(*this) };
    //    }

    //    auto initial_suspend() {               // initial suspend point
    //        return std::suspend_always{};      // - suspend immediately
    //    }

    //    void unhandled_exception() {           // deal with exceptions
    //        std::terminate();                  // - terminate the program
    //    }

    //    // reaction to co_yield
    //    auto yield_value(int val) {
    //        m_coroutineValue = val;           // - store value locally
    //        return std::suspend_always{};     // - suspend coroutine
    //    }

    //    void return_void() {                   // deal with the end or co_return;
    //    }

    //    auto final_suspend() noexcept {    // final suspend point
    //        return std::suspend_always{};  // - suspend immediately
    //    }
    //};

    static CoroutineGen coroutine(int max)
    {
        std::println("      coroutine entered: Max = {}", max);

        for (int val = 1; val <= max; ++val) {

            std::println("      coroutine: {}/{}", val, max);

            std::println("      coroutine: vor co_await");
            //co_await std::suspend_always{};                 // suspending
            co_yield val; // suspending with value
            std::println("      coroutine: nach co_await");
        }

        std::println("      coroutine leaving: Max = {}", max);
    }

    static void motivation_01()
    {
        auto coroutineGen = coroutine(3);                            // initializing coroutine

        std::println("coroutine started");

        // loop to resume the coroutine until it is done
        while (coroutineGen.resume()) {                         // resuming coroutine once
          //  std::println("coroutine resumed");

            auto val = coroutineGen.getValue();
        //    std::cout << "coro() suspended with " << val << '\n';
            std::println("coroutine resumed with {}", val);
        }

        std::println("coroutine done");
    }

}

// ===============================================================

void coroutines_21()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_Motivation_Generator;
    motivation_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
