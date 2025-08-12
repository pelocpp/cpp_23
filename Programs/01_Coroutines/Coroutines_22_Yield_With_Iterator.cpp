// ===========================================================================
// Coroutines_22_Yield_With_Iterator.cpp
// ===========================================================================

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include <cassert>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <coroutine>
#include <print>

namespace Coroutines_Motivation_Generator_With_Iterator
{
     // coroutine interface to deal with a simple task
     // - providing resume() to resume the coroutine
    template<typename T>
    class [[nodiscard]] CoroutineGen {
    public:
        struct promise_type;      // forward declaration

        using CoroutineHandle = std::coroutine_handle<promise_type>;

    private:
        CoroutineHandle m_hdl;    // native coroutine handle

    public:

        // Nested Implementation - using class explicitly in outer class 
        struct promise_type
        {
            T m_yieldValue{};                      // latest value from co_yield

            auto get_return_object() {             // init and return the coroutine interface
                return CoroutineGen{ CoroutineHandle::from_promise(*this) };
            }

            auto initial_suspend() {               // initial suspend point
                return std::suspend_always{};      // - suspend immediately
            }

            void unhandled_exception() {           // deal with exceptions
                std::terminate();                  // - terminate the program
            }

            auto final_suspend() noexcept {        // final suspend point
                return std::suspend_always{};      // - suspend immediately
            }

            // reaction to co_yield
            auto yield_value(T val) {
                m_yieldValue = val;                // - store value locally
                return std::suspend_always{};      // - suspend coroutine
            }

            void return_void() {}                  // deal with end of coroutine
        };

        // c'tor / d'tor
        CoroutineGen(std::coroutine_handle<promise_type> hdl)
            : m_hdl{ hdl }                                       // store coroutine handle in interface
        {
        }

        ~CoroutineGen() {

            if (m_hdl) {
                m_hdl.destroy();                                 // destroy coroutine handle
            }
        }

        // no copy / no move
        CoroutineGen(const CoroutineGen&) = delete;
        CoroutineGen& operator=(const CoroutineGen&) = delete;

        CoroutineGen(CoroutineGen&&) noexcept = delete;
        CoroutineGen& operator=(CoroutineGen&&) noexcept = delete;

        // API
        // => Iterator interface with begin() and end()
        struct iterator
        {
            std::coroutine_handle<promise_type> m_hdl;  // nullptr on end

            iterator(auto hdl) : m_hdl{ hdl } {}

            void getNext() {
                if (m_hdl) {
                    m_hdl.resume();        // resume
                    if (m_hdl.done()) {
                        m_hdl = nullptr;
                    }
                }
            }

            T operator*() const {
                assert(m_hdl != nullptr);
                return m_hdl.promise().m_yieldValue;
            }

            iterator operator++() {
                getNext();             // resume for next value
                return *this;
            }

            bool operator== (const iterator& other) const { 
                return m_hdl == other.m_hdl;
            };
        };

        iterator begin() const {
            if (!m_hdl || m_hdl.done()) {
                return iterator{ nullptr };
            }

            iterator itor{ m_hdl };      // initialize iterator
            itor.getNext();              // resume for first value
            return itor;
        }

        iterator end() const {
            return iterator{ nullptr };
        }
    };

    static CoroutineGen<int> coroutine(int max)
    {
        std::println("      coroutine entered: Max = {}", max);

        for (int val = 1; val <= max; ++val) {

            std::println("      coroutine: {}/{}", val, max);

            std::println("      coroutine: vor co_yield");
            co_yield val;                                        // suspending with value
            std::println("      coroutine: nach co_yield");
        }

        std::println("      coroutine leaving: Max = {}", max);  // demonstrating 'co_return'
    }

    static void motivation_01()
    {
        // simple example demonstrating
        // iterating over values that a coroutine yields 

        auto coroutineGen = coroutine(3);                        // initializing coroutine

        std::println("coroutine started (range-based for-loop)");

        // loop to resume the coroutine for the next value
        for (const auto& val : coroutineGen) {
            std::println("coroutine: val = {}", val);
        }

        std::println("coroutine done");
    }
}

// ===============================================================

void coroutines_22()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace Coroutines_Motivation_Generator_With_Iterator;
    motivation_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
