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


}

void example_more_details()
{
}

// ===========================================================================
// End-of-File
// ===========================================================================
