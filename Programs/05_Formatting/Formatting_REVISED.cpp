//// ===========================================================================
//// Formatting.cpp
//// ===========================================================================
//
//#include <print>
//#include <iostream>
//#include <string>
//#include <string_view>
//#include <format>
//#include <vector>
//
//
//namespace Formatting_Examples_Revised
//{
//    static void test_01()
//    {
//        std::println("Hello, world!");
//    }
//
//    static void test_02()
//    {
//        int value{ 123 };
//        std::println("Value: {}", value);
//    }
//
//    static void test_03()
//    {
//        int firstValue{ 123 };
//        int secondValue{ 456 };
//
//        std::println("First Value: {0}, Second Value: {1}", firstValue, secondValue);
//    }
//
//
//    static void test_04()
//    {
//        std::vector<int> vec = { 1, 2, 3, 4, 5 };
//        std::println("{}", vec);
//    }
//
//    static void test_05()
//    {
//        int value{ 123 };
//
//        std::println("{}", value);             // "123"
//        std::println("{0}", value);            // "123"
//        std::println("{:10}", value);          // "       123"
//        std::println("!{:_<10}!", value);      // "!123_______!"
//        std::println("!{:_>10}!", value);      // "!_______123!"
//    }
//
//    static void test_06()
//    {
//        int value{ 123 };
//
//        std::println("{}", value);             // "123"
//        std::println("{:d}", value);           // "123"
//        std::println("{:010}", value);         // "0000000123"
//        std::println("{:010d}", value);        // "0000000123"
//        std::println("{:0}", value);           // "123"
//        std::println("{:+}", value);           // "+123"
//        std::println("{:+}", -value);          // "-123"
//        std::println("{:+10}", value);         // "      +123"
//        std::println("{:+10}", -value);        // "      -123"
//        std::println("{:+010}", value);        // "+000000123"
//        std::println("{:+010}", -value);       // "-000000123"
//    }
//
//    static void test_07()
//    {
//        float pi{ 3.1415926535f };
//
//        std::println("{}", pi);                // "3.1415927"
//        std::println("{0}", pi);               // "3.1415927"
//        std::println("{:15f}", pi);            // "       3.141593"    (width = 15)
//        std::println("{:{}f}", pi, 15);        // "       3.141593"    (width = 15)
//        std::println("{:.12f}", pi);           // "3.141592741013"     (precision = 12)
//        std::println("{:.{}f}", pi, 3);        // "3.142"              (precision = 3)
//        std::println("{:15.12f}", pi);         // " 3.141592741013"    (width = 15, precision = 12)
//        std::println("{:{}.{}f}", pi, 15, 12); // " 3.141592741013"    (width = 15, precision = 12)
//    }
//
//    static void test_08()
//    {
//        double pi{ 3.1415926535f };
//
//        std::println("{}", pi);                // "3.1415927"
//        std::println("{0}", pi);               // "3.1415927"
//        std::println("{:15g}", pi);            // "       3.141593"    (width = 15)
//        std::println("{:{}g}", pi, 15);        // "       3.141593"    (width = 15)
//        std::println("{:.12g}", pi);           // "3.141592741013"     (precision = 12)
//        std::println("{:.{}g}", pi, 3);        // "3.142"              (precision = 3)
//        std::println("{:15.12g}", pi);         // " 3.141592741013"    (width = 15, precision = 12)
//        std::println("{:{}.{}g}", pi, 15, 12); // " 3.141592741013"    (width = 15, precision = 12)
//    }
//
//    // Ganze Zahlen in unterschiedlichen Formaten
//    static void test_09()
//    {
//        std::println("Hexadecimal: {:x} {:x} {:X} {:#x} {:#X}", 6, 30, 30, 30, 30);
//        std::println("Octal:       {:o} {:#o} {:#o}", 12, 12, 4);
//        std::println("Binary:      {:b} {:#b} {:#B}", 15, 15, 5);
//    }
//
//
//
//    // ===========================================================================
//    // ===========================================================================
//// ===========================================================================
//
//
//
//
//
//
//}
//
//// ===============================================================
//// ===========================================================================
//
//// See also Nicolai Josuttis
//
//// https://www.modernescpp.com/index.php/formatting-user-defined-types-in-c20/
//
//// https://www.cppstories.com/2022/custom-stdformat-cpp20/
//
//// https://www.cppstories.com/2020/02/extra-format-cpp20.html/#user-defined-types
//
//class SingleValue {                                          // (1)
//public:
//    SingleValue() = default;
//    explicit SingleValue(int s) : singleValue{ s } {}
//    int getValue() const {                                    // (2)
//        return singleValue;
//    }
//private:
//    int singleValue{};
//};
//
//template<>                                                   // (3)
//struct std::formatter<SingleValue>
//{
//    // https://stackoverflow.com/questions/56568230/constexpr-and-stdcout-working-on-function-but-not-in-lambda
//    // 
//    // for debugging only
//    constexpr formatter()
//    {
//#pragma message("ABC")
//    }
//
//    constexpr auto parse(std::format_parse_context& ctx) { // (4)
//        return ctx.begin();
//    }
//
//    auto format(const SingleValue& sv, std::format_context& ctx) const {  // (5)
//
//        auto x = ctx.arg(0);
//
//        return std::format_to(ctx.out(), "{}", sv.getValue());
//    }
//};
//
//
//namespace Formatting_Examples
//{
//    static void test_08() {
//
//        std::cout << '\n';
//
//        SingleValue sVal0;
//        SingleValue sVal2020{ 2020 };
//        SingleValue sVal2023{ 2023 };
//
//        std::cout << std::format("Single Value: {} {} {}\n", sVal0, sVal2020, sVal2023);
//        std::cout << std::format("Single Value: {1} {1} {1}\n", sVal0, sVal2020, sVal2023);
//        std::cout << std::format("Single Value: {2} {1} {0}\n", sVal0, sVal2020, sVal2023);
//
//        std::cout << '\n';
//
//    }
//}
//
//// ===========================================================================
//// ===========================================================================
//
//struct Color
//{
//    uint8_t r{ 0 };
//    uint8_t g{ 0 };
//    uint8_t b{ 0 };
//};
////
////template <>
////struct std::formatter<Color> {
////    constexpr auto parse(std::format_parse_context& ctx) {
////        return ctx.begin();
////    }
////
////    auto format(const Color& col, std::format_context& ctx) const {
////        return std::format_to(ctx.out(), "[{}:{}:{}]", col.r, col.g, col.b);
////    }
////};
//
//template <>
//struct std::formatter<Color> : std::formatter<string_view> {
//    auto format(const Color& col, std::format_context& ctx) const {
//        std::string temp;
//        std::format_to(std::back_inserter(temp), "[{}:{}:{}]",
//            col.r, col.g, col.b);
//        return std::formatter<string_view>::format(temp, ctx);
//    }
//};
//
//namespace Formatting_Examples
//{
//    static void test_09() {
//
//        struct Color myCol { 200, 200, 200 };
//
//        std::cout << std::format("struct Color: {}\n", myCol);
//        //std::cout << std::format("Single Value: {1} {1} {1}\n", sVal0, sVal2020, sVal2023);
//        //std::cout << std::format("Single Value: {2} {1} {0}\n", sVal0, sVal2020, sVal2023);
//
//        //std::cout << '\n';
//
//    }
//}
//
//// ===========================================================================
//// ===========================================================================
//
//
//void test_formatting_revised()
//{
//    using namespace Formatting_Examples_Revised;
//
//    //test_01();
//    //test_02();
//    //test_03();
//    //test_04();
//    //test_05();
//   // test_06();
//    //std::println("===============");
//    //test_07();
//    //test_08();
//    test_09();
//}
//
//// ===========================================================================
//// End-of-File
//// ===========================================================================
