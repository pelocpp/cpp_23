// ===========================================================================
// Coroutines_20_Await_Suspend_Resume.cpp
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

namespace Coroutines_Motivation_Task
{
    // coroutine interface to deal with a simple task
    // - providing resume() to resume the coroutine
    class [[nodiscard]] CoroutineTask {
    public:
        struct promise_type;                                     // forward declaration

        using CoroutineHandle = std::coroutine_handle<promise_type>;
    
    private:
        CoroutineHandle m_hdl;                                   // native coroutine handle

    public:
        // c'tor / d'tor
        CoroutineTask(std::coroutine_handle<promise_type> hdl) 
            : m_hdl{ hdl }                                       // store coroutine handle in interface
        {}

        ~CoroutineTask() {

            if (m_hdl) {
                m_hdl.destroy();                                 // destroy coroutine handle
            }
        }

        // no copy / no move
        CoroutineTask(const CoroutineTask&) = delete;
        CoroutineTask& operator=(const CoroutineTask&) = delete;

        CoroutineTask(CoroutineTask&&) noexcept = delete;
        CoroutineTask& operator=(CoroutineTask&&) noexcept = delete;

        // API:
        // => to resume the coroutine
        // => returns whether there is still something to process
        bool resume() const {

            if (!m_hdl || m_hdl.done()) {
                return false;                  // nothing (more) to process
            }

            m_hdl.resume();                    // resume (blocks until suspended again or the end)
            return !m_hdl.done();
        }
    };

    struct CoroutineTask::promise_type
    {
        auto get_return_object() {         // init and return the coroutine interface
            return CoroutineTask{ CoroutineHandle::from_promise(*this) };
        }

        auto initial_suspend() {           // initial suspend point
            return std::suspend_always{};  // - suspend immediately
        }

        void unhandled_exception() {       // deal with exceptions
            std::terminate();              // - terminate the program
        }

        void return_void() {               // deal with the end or co_return;
        }

        auto final_suspend() noexcept {    // final suspend point
            return std::suspend_always{};  // - suspend immediately
        }
    };

    static CoroutineTask coroutine(int max)
    {
        std::println("      coroutine entered: Max = {}", max);

        for (int val = 1; val <= max; ++val) {

            std::println("      coroutine: {}/{}", val, max);

            std::println("      coroutine: vor co_await");
            co_await std::suspend_always{};                 // suspending
            std::println("      coroutine: nach co_await");
        }

        std::println("      coroutine leaving: Max = {}", max);
    }

    static void motivation_01()
    {
        auto coroTask = coroutine(3);                       // initializing coroutine

        std::println("coroutine started");

        // loop to resume the coroutine until it is done
        while (coroTask.resume()) {                         // resuming coroutine
            std::println("coroutine resumed");
        }

        std::println("coroutine done");
    }

    static void motivation_02()
    {
        // start two coroutines
        auto coroTask1 = coroutine(3);              // initializing 1st coroutine
        auto coroTask2 = coroutine(5);              // initializing 2nd coroutine

        std::println("two coroutines started");

        coroTask2.resume();                         // resuming 2nd coroutine once

        // loop to resume the 1st coroutine until it is done:
        while (coroTask1.resume()) {                // resuming 1st coroutine
            std::println("first coroutine resumed");
        }

        std::println("first coroutine done");

        std::println("resuming second coroutine");
        coroTask2.resume();                         //resuming 2nd coroutine again once
        coroTask2.resume();                         //resuming 2nd coroutine again once
        coroTask2.resume();                         //resuming 2nd coroutine again once
        coroTask2.resume();                         //resuming 2nd coroutine again once
        coroTask2.resume();                         //resuming 2nd coroutine again once

        std::println("Done");
    }
}

// ===============================================================

void coroutines_20()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_Motivation_Task;
    motivation_01();
    motivation_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
