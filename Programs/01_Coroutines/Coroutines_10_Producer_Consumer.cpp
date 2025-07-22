// ===========================================================================
// Coroutines_10_Producer_Consumer.cpp
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

#include <algorithm>
#include <atomic>
#include <chrono>
#include <coroutine>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <thread>
#include <utility>
#include <vector>
#include <source_location>


#define FUNC() std::cout << __func__ << '\n'

//void FUNC(const std::source_location location = std::source_location::current())
//{
//    std::cout << location.function_name() << '\n';
//}

namespace details
{
    template <typename InputIterator>
    void printIterable(InputIterator first, InputIterator last)
    {
        using value_type = std::decay_t<decltype(*first)>;
        std::cout << '[';
        if constexpr (std::is_same_v<std::uint8_t, value_type>) {
            std::copy(first, std::prev(last), std::ostream_iterator<std::uint16_t>(std::cout, ", "));
            std::cout << static_cast<std::uint16_t>(*std::prev(last)) << "]\n";
        }
        else
        {
            std::copy(first, std::prev(last), std::ostream_iterator<value_type>(std::cout, ", "));
            std::cout << *std::prev(last) << "]\n";
        }
    }

    template <typename Container>
    void printContainer(const Container& container)
    {
        printIterable(std::cbegin(container), std::cend(container));
    }
}

using DataType = std::vector<int>;

// Generator
class AudioDataResult final
{
public:
    class promise_type;

    using Handle = std::coroutine_handle<promise_type>;

    // Predefined interface 'promise_type' that has to be specified in order to implement
    // coroutine's state-machine transitions
    class promise_type
    {
    public:
        using value_type = DataType;

    private:
        value_type        m_data;
        std::atomic<bool> m_data_ready;

    public:
        AudioDataResult get_return_object()
        {
            return AudioDataResult{ Handle::from_promise(*this) };
        }

        std::suspend_never initial_suspend() noexcept { 
            return {};
        }

        std::suspend_always final_suspend() noexcept {
            return {};
        }

        void return_void() {}

        void unhandled_exception()
        {
            std::rethrow_exception(std::current_exception());
        }

        // generates the value and suspends the "producer"
        // note: co_yield is called in two flavours:
        // with named and unnamed objects - therefore for this method 'yield_value'
        // code is generated twice: with value being an lvalue or an rvalue
        template <typename Data>
            requires std::convertible_to<std::decay_t<Data>, value_type>
        std::suspend_always yield_value(Data&& value)
        {
            m_data = std::forward<Data>(value);
            m_data_ready.store(true, std::memory_order::relaxed);
            return {};
        }

        // Awaiter interface:
        // for consumer waiting on data being ready
        class AudioDataAwaiter
        {
        private:
            promise_type& m_promise;

        public:
            explicit AudioDataAwaiter(promise_type& promise) noexcept : m_promise(promise) {}

            bool await_ready() const {
                std::cout << "> await_ready" << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
                return m_promise.m_data_ready.load(std::memory_order::relaxed);

                // return false;
            }

            void await_suspend(Handle handle) const
            {
                std::cout << "> await_suspend" << std::endl;

                while (not m_promise.m_data_ready.exchange(false)) {
                    std::cout << "yielding .............." << std::endl;
                    std::this_thread::yield();
                }

                //while (true) {
                //    std::cout << "yielding .............." << std::endl;
                //    std::this_thread::yield();
                //}
            }

            // move assignment at client invocation side:
            // const auto data = co_await audioDataResult;
            // This requires that coroutine's result type provides the co_await unary operator
            value_type&& await_resume() const
            {
                std::cout << "> await_resume" << std::endl;

                return std::move(m_promise.m_data);
            }

        }; //Awaiter interface

    }; //promise_type interface

    auto operator co_await() noexcept
    {
        return promise_type::AudioDataAwaiter{ m_handle.promise() };
    }

    // Make the result type move-only, due to ownership over the handle
    AudioDataResult(const AudioDataResult&) = delete;
    AudioDataResult& operator=(const AudioDataResult&) = delete;

    AudioDataResult(AudioDataResult&& other) noexcept : m_handle(std::exchange(other.m_handle, nullptr)) {}
    AudioDataResult& operator=(AudioDataResult&& other) noexcept
    {
        using namespace std;
        AudioDataResult tmp = std::move(other);
        swap(*this, tmp);
        std::cout << "HIER:::::::::::::::::: Wozu ????????????????????????????????????";
        return *this;
    }

    // d-tor: RAII
    ~AudioDataResult() {
        if (m_handle) {
            FUNC(); 
            m_handle.destroy();
        } 
    }

    // For resuming the producer - at the point when the data are consumed
    void resume() {
        if (not m_handle.done()) {
            FUNC(); 
            m_handle.resume();
        }
    }

private:

    AudioDataResult() {
        FUNC();
    }

    AudioDataResult(Handle handle) noexcept 
        : AudioDataResult{}
    {
        m_handle = handle;
    }

private:
    Handle m_handle;
};

static AudioDataResult producer(DataType& data)
{
    for (int i = 1; i != 5; ++i) {

        FUNC();
        data.push_back(i);

        std::cout << "producer: vor co_yield:" << std::endl;
        co_yield data;
        std::cout << "producer: nach co_yield:" << std::endl;

       // details::printContainer(data);
    }

    co_yield DataType{}; // exit criteria

    co_return;
}

static AudioDataResult consumer(AudioDataResult& audioDataResult)
{
    for (;;)
    {
        FUNC();

        std::cout << "consumer: vor co_await:" << std::endl;;
        const auto& data = co_await audioDataResult;

        if (data.empty()) { 
            std::cout << "consumer: no data - exit!\n";
            break;
        }
        
        std::cout << "consumer: data received:";
        details::printContainer(data);

        audioDataResult.resume(); // resume producer
    }

    co_return;
}

void coroutines_10()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        DataType data{ };

        auto audioDataProducer = producer(data);
        
        std::thread t([&] {auto audioRecorded = consumer(audioDataProducer); });
        t.join();
    }

    std::cout << "Done" << std::endl;
}

// ===========================================================================
// End-of-File
// ===========================================================================
