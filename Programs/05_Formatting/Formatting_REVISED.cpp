// ===========================================================================
// Formatting.cpp
// ===========================================================================

#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <vector>

// import std;

// custom formatter for displaying vectors
template <typename T>
struct std::formatter<std::vector<T>> : std::formatter<T>
{
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(std::vector<T> v, FormatContext& ctx) const {
        std::vector<std::string> v_str;
        v_str.reserve(v.size());
        const auto fmt_str = [&]() {
            if constexpr (std::is_integral<T>::value) {
                return "{:+5}";
            }
            else if constexpr (std::is_floating_point<T>::value) {
                return "{:+5.2}";
            }
            else {
                return "{}";
            }
            }();
        for (auto& e : v) {
            v_str.push_back(std::format(fmt_str, e));
        }
        return format_to(ctx.out(), "{}", v);
    }
};

namespace Formatting_Examples_Revised
{
    static void test_01()
    {
        std::println("Hello, world!");
    }

    static void test_02()
    {
        int value{ 123 };
        std::println("Value: {}", value);
    }

    static void test_03()
    {
        int firstValue{ 123 };
        int secondValue{ 456 };

        std::println("First Value: {0}, Second Value: {1}", firstValue, secondValue);
    }


    static void test_04()
    {
        std::vector<int> vec = { 1, 2, 3, 4, 5 };
        std::println("{}", vec);

        //std::vector<double> v = { 1.324353, 4.432345 };
        //std::print("{::+5.2}\n", v);
    }

    static void test_05()
    {
        int value{ 123 };

        std::println("{}", value);         // 123
        std::println("{0}", value);        // 123
        std::println("{:10}", value);      //        123
        std::println("!{:_<10}!", value);  // !123_______!
        std::println("!{:_>10}!", value);  // !_______123!
    }

    // ===========================================================================
    // ===========================================================================
// ===========================================================================



    static void test_01b()
    {
        int age = 30;
        std::string name{ "Hans" };

        std::string result{
            std::format("My name is {} and I'm {} years old", name, age)
        };
        std::cout << result << std::endl;
    }

    //static void test_01()
    //{
    //    //test_01a();
    //    //test_01b();
    //}

    // Reihenfolge der Argumente definieren
    static void test_02a(std::string_view format)
    {
        std::string_view one{ "One" };
        std::string_view two{ "Two" };
        std::string_view three{ "Three" };

        // doesn't compile with the lastest C++ compiler version -  needs to be fixed
         //std::string result = std::format(format, one, two, three);
         //std::cout << result << std::endl;
    }

    static void test_02b(std::string_view format)
    {
        std::string_view one{ "One" };
        std::string_view two{ "Two" };
        std::string_view three{ "Three" };

        // doesn't compile with the lastest C++ compiler version -  needs to be fixed
        // std::string result = std::format(format, one, two, three);
        // std::cout << result << std::endl;
    }

    static void test_02222()
    {
        std::string_view format1{ "{0}, {1}, {2}" };
        test_02a(format1);

        // doesn't compile with the lastest C++ compiler version -  needs to be fixed
        // std::string_view format2{ "{2}, {1}, {0}" };
        // test_02b(format2);
    }



    // from https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification
    static void test_04444()
    {
        float pi = 3.14f;
        std::cout << std::format("{:10f}", pi) << std::endl;           // s1 = "  3.140000" (width = 10)
        std::cout << std::format("{:{}f}", pi, 10) << std::endl;       // s2 = "  3.140000" (width = 10)
        std::cout << std::format("{:.3f}", pi) << std::endl;           // s3 = "3.14000" (precision = 3)
        std::cout << std::format("{:.{}f}", pi, 3) << std::endl;       // s4 = "3.14000" (precision = 3)
        std::cout << std::format("{:10.5f}", pi) << std::endl;         // s5 = "   3.14000" (width = 10, precision = 5)
        std::cout << std::format("{:{}.{}f}", pi, 10, 5) << std::endl; // s6 = "   3.14000" (width = 10, precision = 5)
    }

    // Ganze Zahlen in unterschiedlichen Formaten
    static void test_05555()
    {
        std::cout << std::format("Decimal:     {} {:d} {:06} {:06d} {:0} {:+} {:+}", 1, 2, 3, 4, 5, 6, -7) << std::endl;
        std::cout << std::format("Hexadecimal: {:x} {:x} {:X} {:#x}", 5, 10, 10, 5) << std::endl;
        std::cout << std::format("Octal:       {:o} {:#o} {:#o}", 12, 12, 4) << std::endl;
        std::cout << std::format("Binary:      {:b} {:#b} {:#B}", 15, 15, 5) << std::endl;
    }

    // Flie�kommazahlen
    static void test_06666()
    {
        std::cout << std::format("Default:     {} {:} {:g} {:g}", 1.5, 1.5, 1.5, 1e20) << std::endl;
        std::cout << std::format("Rounding:    {:f} {:.0f} {:.20f}", 1.5, 1.5, 1.5) << std::endl;
        std::cout << std::format("Padding:     {:05.2f} {:.2f} {:5.2f}", 1.5, 1.5, 1.5) << std::endl;
        std::cout << std::format("Scientific:  {:E} {:e}", 1.5, 1.5) << std::endl;
    }

    static void test_0777()
    {
        std::cout << std::format("Default:     {} {:} {:g} {:g}", 1.5, 1.5, 1.5, 1e20) << std::endl;
        std::cout << std::format("Rounding:    {:f} {:.0f} {:.20f}", 1.5, 1.5, 1.5) << std::endl;
        std::cout << std::format("Padding:     {:05.2f} {:.2f} {:5.2f}", 1.5, 1.5, 1.5) << std::endl;
        std::cout << std::format("Scientific:  {:E} {:e}", 1.5, 1.5) << std::endl;
    }
}

// ===============================================================
// ===========================================================================

// See also Nicolai Josuttis

// https://www.modernescpp.com/index.php/formatting-user-defined-types-in-c20/

// https://www.cppstories.com/2022/custom-stdformat-cpp20/

// https://www.cppstories.com/2020/02/extra-format-cpp20.html/#user-defined-types

class SingleValue {                                          // (1)
public:
    SingleValue() = default;
    explicit SingleValue(int s) : singleValue{ s } {}
    int getValue() const {                                    // (2)
        return singleValue;
    }
private:
    int singleValue{};
};

template<>                                                   // (3)
struct std::formatter<SingleValue>
{
    // https://stackoverflow.com/questions/56568230/constexpr-and-stdcout-working-on-function-but-not-in-lambda
    // 
    // for debugging only
    constexpr formatter()
    {
#pragma message("ABC")
    }

    constexpr auto parse(std::format_parse_context& ctx) { // (4)
        return ctx.begin();
    }

    auto format(const SingleValue& sv, std::format_context& ctx) const {  // (5)

        auto x = ctx.arg(0);

        return std::format_to(ctx.out(), "{}", sv.getValue());
    }
};


namespace Formatting_Examples
{
    static void test_08() {

        std::cout << '\n';

        SingleValue sVal0;
        SingleValue sVal2020{ 2020 };
        SingleValue sVal2023{ 2023 };

        std::cout << std::format("Single Value: {} {} {}\n", sVal0, sVal2020, sVal2023);
        std::cout << std::format("Single Value: {1} {1} {1}\n", sVal0, sVal2020, sVal2023);
        std::cout << std::format("Single Value: {2} {1} {0}\n", sVal0, sVal2020, sVal2023);

        std::cout << '\n';

    }
}

// ===========================================================================
// ===========================================================================

struct Color
{
    uint8_t r{ 0 };
    uint8_t g{ 0 };
    uint8_t b{ 0 };
};
//
//template <>
//struct std::formatter<Color> {
//    constexpr auto parse(std::format_parse_context& ctx) {
//        return ctx.begin();
//    }
//
//    auto format(const Color& col, std::format_context& ctx) const {
//        return std::format_to(ctx.out(), "[{}:{}:{}]", col.r, col.g, col.b);
//    }
//};

template <>
struct std::formatter<Color> : std::formatter<string_view> {
    auto format(const Color& col, std::format_context& ctx) const {
        std::string temp;
        std::format_to(std::back_inserter(temp), "[{}:{}:{}]",
            col.r, col.g, col.b);
        return std::formatter<string_view>::format(temp, ctx);
    }
};

namespace Formatting_Examples
{
    static void test_09() {

        struct Color myCol { 200, 200, 200 };

        std::cout << std::format("struct Color: {}\n", myCol);
        //std::cout << std::format("Single Value: {1} {1} {1}\n", sVal0, sVal2020, sVal2023);
        //std::cout << std::format("Single Value: {2} {1} {0}\n", sVal0, sVal2020, sVal2023);

        //std::cout << '\n';

    }
}

// ===========================================================================
// ===========================================================================


void test_formatting_revised()
{
    using namespace Formatting_Examples_Revised;

    //test_01();
    //test_02();
    //test_03();
    test_04();
    //test_05();
    //test_06();
    //test_07();
    //test_08();
    //test_09();
}

// ===========================================================================
// End-of-File
// ===========================================================================
