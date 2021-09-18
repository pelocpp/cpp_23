// ===========================================================================
// ConceptsExercises.cpp
// ===========================================================================

#include <string>
#include <iostream>

namespace Exercises {

    namespace Exercise_01 {

        // concepts
        template <typename T>
        concept IsString =
            std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::string>::value;

        template <typename T>
        concept IsStringVerbose =
            std::is_same<typename std::remove_cv<typename std::remove_reference<T>::type>::type, std::string>::value;

        template<IsString... ARGS>
        std::string concat(const ARGS& ... args)
        {
            // calculate length of result string
            size_t len{};
            for (auto s : { args ... }) {
                len += s.size();
            }

            std::string result;
            result.reserve(len);   // preallocate

            // concatenate strings
            for (auto s : { args... }) {
                result += s;
            }

            return result;
        }

        template<IsString... ARGS>
        std::string concatImproved(const ARGS& ... args)
        {
            // calculate length of result string
            size_t len{};
            for (auto s : { args ... }) {
                len += s.size();
            }

            std::string result(len, '\0');   // preallocate

            // concatenate strings
            std::string::iterator it = std::begin(result);
            for (auto s : { args... }) {
                std::copy(std::begin(s), std::end(s), it);
                it += s.size();
            }

            return result;
        }

        void testExercise_01a() {

            std::string s1{ "111" };
            std::string s2{ "222" };
            std::string s3{ "333" };
            std::string s4{ "444" };

            std::string result{ concat(
                s1, std::string{ "/" },
                s2, std::string{ "/" },
                s3, std::string{ "/" },
                s4, std::string{ "." }
            ) };
            std::cout << result << std::endl;
        }

        void testExercise_01b() {

            std::string s1{ "111" };
            std::string s2{ "222" };
            std::string s3{ "333" };
            std::string s4{ "444" };

            std::string result{ concatImproved(
                s1, std::string{ "/" },
                s2, std::string{ "/" },
                s3, std::string{ "/" },
                s4, std::string{ "." }
            ) };
            std::cout << result << std::endl;
        }

        void testExercise_01c() {

            std::string s1{ "111/" };
            std::string& s2{ s1 };
            const std::string s3{ "333/" };
            const std::string& s4{ s3 };

            std::string result{ concat(
                s1, 
                s2, 
                s3, 
                s4,
                std::string { "555/" },
                "7" + std::string { "7" } + "7"
            ) };
            std::cout << result << std::endl;
        }

        void testExercise_01() {
            testExercise_01a();
            testExercise_01b();
            testExercise_01c();
        }
    }
}

void test_exercises()
{
    using namespace Exercises;
    Exercise_01::testExercise_01();
}


// ===========================================================================
// End-of-File
// ===========================================================================
