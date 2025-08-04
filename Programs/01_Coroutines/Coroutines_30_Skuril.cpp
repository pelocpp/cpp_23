// ===========================================================================
// Coroutines_23_Awaiter.cpp
// ===========================================================================

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <coroutine>
#include <print>
#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <thread>
#include <type_traits>

namespace Coroutines_Motivation_Coroutines_Threadpool
{
    class [[nodiscard]] task
    {
    public:
        using promise_type = task_promise;

        explicit task(std::coroutine_handle<task_promise> handle)
            : m_handle(handle)
        {
        }

    private:
        std::coroutine_handle<task_promise> m_handle;
    };

    struct task_promise
    {
        task get_return_object() noexcept {
            return task{ std::coroutine_handle<task_promise>::from_promise(*this) };
        };

        std::suspend_never initial_suspend() const noexcept { return {}; }
        std::suspend_never final_suspend() const noexcept { return {}; }

        void return_void() noexcept {}

        void unhandled_exception() noexcept {
            std::cerr << "Unhandled exception caught...\n";
            exit(1);
        }
    };

    task run_async_print(threadpool& pool)
    {
        co_await pool.schedule();
        std::cout << "This is a hello from thread: "
            << std::this_thread::get_id() << "\n";
    }

    auto schedule()
    {
        struct awaiter
        {
            threadpool* m_threadpool;

            constexpr bool await_ready() const noexcept { return false; }
            constexpr void await_resume() const noexcept {}
            void await_suspend(std::coroutine_handle<> coro) const noexcept {
                m_threadpool->enqueue_task(coro);
            }
        };
        return awaiter{ this };
    }


    class threadpool
    {
    public:
        // ...

        auto schedule()
        {
            // ... here we've got to return the awaiter I mentioned earlier
        }
    private:
        std::queue<std::coroutine_handle<>> m_coros;
        // ...

        void thread_loop()
        {
            while (!m_stop_thread)
            {
                std::unique_lock<std::mutex> lock(m_mutex);

                // ... waiting for a task, checking for stop requests

                auto coro = m_coros.front();
                m_coros.pop();
                lock.unlock();
                coro.resume();
            }
        }

        void enqueue_task(std::couroutine_handle<> coro) noexcept {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_coros.emplace(coro);
            m_cond.notify_one();
        }
    };

}

// ===============================================================

void coroutines_30()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_Motivation_Coroutines_Threadpool;

    int main() {

    }
}

// ===========================================================================
// End-of-File
// ===========================================================================
