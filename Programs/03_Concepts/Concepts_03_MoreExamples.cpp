// ===========================================================================
// Concepts_03_MoreExamples.cpp
// ===========================================================================

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <future>

namespace MoreExamples {

    namespace Concept_01_SimplestConceptOnEarth {

        template<typename T>
        concept Any = true;

        void test()
        {
            static_assert(Any<char> == true);
            static_assert(Any<std::thread> == true);
            static_assert(Any<std::future<int>> == true);
        }
    }

    namespace Concept_02_CombiningConcepts {

        template<typename T>
        concept Number = std::integral<T> || std::floating_point<T>;

        template<typename T>
        concept Integer = std::integral<T>;

        template<typename T>
        concept Float = std::floating_point<T>;

        template<typename T>
        concept NumberEx = Integer<T> || Float<T>;

        void test()
        {
            static_assert(Number<short> == true);
            static_assert(Integer<int> == true);
            static_assert(Float<double> == true);
            static_assert(NumberEx<double> == true);
        }
    }

    namespace Concept_03_BiggerExample {

        template<typename... Args>
        auto add0(Args&&... args)
        {
            return (... + args);
        }

        void test()
        {
            auto result1 = add0(1);
            auto result2 = add0(1, 2, 3, 4, 5);
        }

        template<typename T, typename...>
        struct first_type
        {
            using type = T;
        };

        template<typename... Args>
        using first_type_t = typename first_type<Args...>::type;

        template<typename T, typename... Ts>
        constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

        template<typename T, typename... Args>
        concept same_as_first_type =
            std::is_same_v<std::remove_cvref_t<T>,
            std::remove_cvref_t<first_type_t<Args...>>>;


        // WEITER ... 
    }
}

void example_more_examples()
{
}

// ===========================================================================
// End-of-File
// ===========================================================================
