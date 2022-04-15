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
//#include <cstddef>
#include <conio.h>  // _getch

// ===========================================================================

// https://stackoverflow.com/questions/67930087/how-to-do-chained-coroutines-in-c-20

// ===========================================================================


// Sticky Bits – Powered by Feabhas
// Martin Bond

namespace Coroutines_StickyBits_Feabhats
{
    //template <typename T>
    //class Future
    //{
    //    class Promise
    //    {
    //    public:
    //        using value_type = std::optional<T>;

    //        Promise() = default;
    //        std::suspend_always initial_suspend() { return {}; }
    //        std::suspend_always final_suspend() noexcept { return {}; }

    //        void unhandled_exception() {
    //            std::cout << "unhandled_exception:" << std::endl;
    //            std::rethrow_exception(std::move(std::current_exception()));
    //        }

    //        std::suspend_always yield_value(T value) {
    //            this->value = std::move(value);
    //            return {};
    //        }

    //        // void return_value(T value) {
    //        //     this->value = std::move(value);
    //        // }

    //        void return_void() {
    //            this->value = std::nullopt;
    //        }

    //        inline Future get_return_object();

    //        value_type get_value() {
    //            return std::move(value);
    //        }

    //        bool finished() {
    //            return !value.has_value();
    //        }

    //    private:
    //        value_type value{};
    //    };

    //public:
    //    using value_type = T;
    //    using promise_type = Promise;

    //    explicit Future(std::coroutine_handle<Promise> handle)
    //        : handle(handle)
    //    {}

    //    ~Future() {
    //        if (handle) { handle.destroy(); }
    //    }

    //    Promise::value_type next() {
    //        if (handle) {
    //            handle.resume();
    //            return handle.promise().get_value();
    //        }
    //        else {
    //            return {};
    //        }
    //    }

    //private:
    //    std::coroutine_handle<Promise> handle;
    //};


    //template <typename T>
    //inline Future<T> Future<T>::Promise::get_return_object()
    //{
    //    return Future{ std::coroutine_handle<Promise>::from_promise(*this) };
    //}

    // ===================================================================

    // Der stammt aus 05_Iterators .. würde also besser in dieses Repo passen :-)))

    template<std::movable T>
    class Generator {
    public:
        struct promise_type {
            Generator<T> get_return_object() {
                return Generator{ Handle::from_promise(*this) };
            }
            static std::suspend_always initial_suspend() noexcept {
                return {};
            }
            static std::suspend_always final_suspend() noexcept {
                return {};
            }
            std::suspend_always yield_value(T value) noexcept {
                current_value = std::move(value);
                return {};
            }
            // Disallow co_await in generator coroutines.
            void await_transform() = delete;
            void return_void() {}
            [[noreturn]]
            static void unhandled_exception() {
                throw;
            }

            std::optional<T> current_value;
        };

        using Handle = std::coroutine_handle<promise_type>;

        explicit Generator(const Handle coroutine) :
            m_handle{ coroutine }
        {}

        Generator() = default;
        ~Generator() {
            if (m_handle) {
                m_handle.destroy();
            }
        }

        Generator(const Generator&) = delete;
        Generator& operator=(const Generator&) = delete;

        Generator(Generator&& other) noexcept :
            m_handle{ other.m_handle }
        {
            other.m_handle = {};
        }
        Generator& operator=(Generator&& other) noexcept {
            if (this != &other) {
                if (m_handle) {
                    m_handle.destroy();
                }
                m_handle = other.m_handle;
                other.m_handle = {};
            }
            return *this;
        }

        std::optional<T> next() {
            if (m_handle) {
                m_handle.resume();
               // return m_handle.promise().current_value.value();
                return m_handle.promise().current_value;
            }
            else {
                return {};
            }
        }

        // Range-based for loop support.
        class Iter {
        public:
            void operator++() {
                m_handle.resume();
            }
            const T& operator*() const {
                return *m_handle.promise().current_value; 
            }
            bool operator==(std::default_sentinel_t) const {
                return !m_handle || m_handle.done();
            }

            explicit Iter(const Handle handle) : m_handle{ handle } {}

        private:
            Handle m_handle;
        };

        Iter begin() {
            if (m_handle) {
                m_handle.resume();
            }
            return Iter{ m_handle };
        }

        std::default_sentinel_t end() {
            return {};
        }

    private:
        Handle m_handle;
    };

    // ===================================================================

    // read float co-routine
    // Original - works
    Generator<float> read_stream(std::istream& in)
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

    //Future<float> read_stream(std::istream& in)
    //{
    //    uint32_t data;
    //    int count{};
    //    char byte;
    //    while (byte = _getch()) {
    //        std::cout << byte;
    //        data = data << 8 | static_cast<unsigned char>(byte);
    //        if (++count == 4) {
    //            float value = *reinterpret_cast<float*>(&data);
    //            // co_yield *reinterpret_cast<float*>(&data);
    //            co_yield value;
    //            data = 0;
    //            count = 0;
    //        }
    //    }
    //}

    struct DataPoint
    {
        float timestamp;
        float data;

    };

    // read struct coroutine
    Generator<DataPoint> read_data(std::istream& in)
    {
        std::optional<float> first{};
        auto raw_data = read_stream(in);
        while (auto next = raw_data.next()) {
            if (first) {
                // co_yield DataPoint{ *first, *next };
                co_yield DataPoint{ first.value(), next.value() };
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
             std::cout << next.value() << std::endl;
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

       // auto xxx = values.next();
        //DataPoint dp = values.next();

        //while (dp.data != 0) {

        //}

        //DataPoint dp;
        //while ((dp = values.next()).data != 0) {

        //}

        std::optional<DataPoint> dp;
        float f;

        while (   f = values.next().value().data != 0) {
            std::cout << std::setw(8) << dp->timestamp
                << std::setw(8) << dp->data
                << (dp->data > threshold ? " ***Threshold exceeded***" : "")
                << std::endl;
        }
    }
}

// ===========================================================================

void coroutines_08()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_StickyBits_Feabhats;
    test_scratch_sticky_bits_01_just_testing_first_coroutine();
    std::cout << "Done." << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
