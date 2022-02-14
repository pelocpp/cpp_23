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
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <cstddef>    // TODO ???????????????????????

// ===========================================================================

// Sticky Bits – Powered by Feabhas
// Martin Bond

namespace Coroutines_StickyBits_Feabhats
{
    template <typename T>
    class Future
    {
        class Promise
        {
        public:
            using value_type = std::optional<T>;

            Promise() = default;
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }

            void unhandled_exception() {
                std::cout << "unhandled_exception:" << std::endl;
                std::rethrow_exception(std::move(std::current_exception()));
            }

            std::suspend_always yield_value(T value) {
                this->value = std::move(value);
                return {};
            }

            // void return_value(T value) {
            //     this->value = std::move(value);
            // }

            void return_void() {
                this->value = std::nullopt;
            }

            inline Future get_return_object();

            value_type get_value() {
                return std::move(value);
            }

            bool finished() {
                return !value.has_value();
            }

        private:
            value_type value{};
        };

    public:
        using value_type = T;
        using promise_type = Promise;

        explicit Future(std::coroutine_handle<Promise> handle)
            : handle(handle)
        {}

        ~Future() {
            if (handle) { handle.destroy(); }
        }

        Promise::value_type next() {
            if (handle) {
                handle.resume();
                return handle.promise().get_value();
            }
            else {
                return {};
            }
        }

    private:
        std::coroutine_handle<Promise> handle;
    };


    template <typename T>
    inline Future<T> Future<T>::Promise::get_return_object()
    {
        return Future{ std::coroutine_handle<Promise>::from_promise(*this) };
    }

    // read float co-routine
    Future<float> read_stream(std::istream& in)
    {
        uint32_t data;
        int count{};
        char byte;
        while (in.get(byte)) {
            data = data << 8 | static_cast<unsigned char>(byte);
            if (++count == 4) {
                float value = *reinterpret_cast<float*>(&data);
                // co_yield *reinterpret_cast<float*>(&data);
                co_yield value;
                data = 0;
                count = 0;
            }
        }
    }

    struct DataPoint
    {
        float timestamp;
        float data;

    };

    // read struct coroutine
    Future<DataPoint> read_data(std::istream& in)
    {
        std::optional<float> first{};
        auto raw_data = read_stream(in);
        while (auto next = raw_data.next()) {
            if (first) {
                co_yield DataPoint{ *first, *next };
                first = std::nullopt;
            }
            else {
                first = next;
            }
        }
    }

    static constexpr float threshold{ 21.0 };

    void test_scratch_sticky_bits_01_just_testing_first_coroutine()
    {
         auto raw_data = read_stream(std::cin);
         while (auto next = raw_data.next()) {
             std::cout << *next << std::endl;
         }
    }

    void test_scratch_sticky_bits_02()
    {
        // auto raw_data = read_stream(std::cin);
        // while (auto next = raw_data.next()) {
        //     std::cout << *next << std::endl;
        // }

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Time (ms)   Data" << std::endl;
        auto values = read_data(std::cin);
        while (auto n = values.next()) {
            std::cout << std::setw(8) << n->timestamp
                << std::setw(8) << n->data
                << (n->data > threshold ? " ***Threshold exceeded***" : "")
                << std::endl;
        }
    }
}

// ===========================================================================

// minimalistic approach
// Vishal Chovatiya
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
        std::suspend_always operator co_await() { return {}; } 
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

void coroutines_08()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //using namespace Coroutines_Scratch_01_Vishal;
    //test_scratch_01();
    //std::cout << "Done." << std::endl;

    using namespace Coroutines_StickyBits_Feabhats;
    test_scratch_sticky_bits_01_just_testing_first_coroutine();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
