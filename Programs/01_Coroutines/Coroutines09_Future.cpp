// ===========================================================================
// Coroutines02_Future.cpp
// ===========================================================================

#include <iostream>
#include <coroutine>
#include <memory>

namespace Coroutines_SimpleFuture
{
    template <typename T>
    class MyFuture
    {
    private:
        std::shared_ptr<T> m_ptr;

    public:
        MyFuture(std::shared_ptr<T> ptr) : m_ptr{ ptr } {
            std::cout << "MyFuture::MyFuture - " << *ptr << std::endl;
        }

        ~MyFuture() {
            std::cout << "MyFuture::~MyFuture" << std::endl;
        }

        T getValue() {
            std::cout << "MyFuture::get" << std::endl;
            T tmp = *m_ptr;
            return tmp;
        }

        class promise_type
        {
        private:
            std::shared_ptr<T> m_ptr;

        public:
            promise_type() {
                std::cout << "\tpromise_type::promise_type" << std::endl;
                m_ptr = std::make_shared<T>(123);
            }

            ~promise_type() {
                std::cout << "\tpromise_type::~promise_type" << std::endl;
            }

            MyFuture<T> get_return_object() {
                std::cout << "\tpromise_type::get_return_object" << std::endl;
                return MyFuture<T>(m_ptr);
            }

            void return_value(T v) {
                std::cout << "\tpromise_type::return_value" << std::endl;
                *m_ptr = v;
            }

            std::suspend_never initial_suspend() {
                std::cout << "\tpromise_type::initial_suspend" << std::endl;
                return {};
            }

            std::suspend_never final_suspend() noexcept {
                std::cout << "\tpromise_type::final_suspend" << std::endl;
                return {};
            }

            void unhandled_exception() {
                std::cout << "\tpromise_type::unhandled_exception" << std::endl;
                std::exit(1);
            }
        };
    };

    MyFuture<int> createFuture() {
        std::cout << "createFuture: vor co_return" << std::endl;
        co_return 12345;
    }

    void coroutines_01()
    {
        MyFuture<int> future = createFuture();
        std::cout << "main: vor get" << std::endl;
        int result = future.getValue();
        std::cout << "main: nach get - result: " << result << std::endl;
    }
}

// ===============================================================

void coroutines_09()
{
    using namespace Coroutines_SimpleFuture;
    coroutines_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
