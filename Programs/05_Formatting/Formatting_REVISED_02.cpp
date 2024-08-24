// ===========================================================================
// Formatting.cpp
// ===========================================================================

// import std;

#include <print>
#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <algorithm>


// #define StdFormatter_01_Basic_Formatter_API
// #define StdFormatter_02_Parsing_Format_String
// #define StdFormatter_03_Delegating_Formatting_to_Standard_Formatters
// #define StdFormatter_04_Inheriting_From_Standard_Formatters
// #define StdFormatter_05_Using_Standard_Formatters_for_Strings
#define StdFormatter_06_Using_Standard_Formatters_for_StdVector

namespace Formatting_Examples_Revised
{
    class SimpleClass
    {
    private:
        int m_value;

    public:
        SimpleClass() : SimpleClass{ 0 } {}

        explicit SimpleClass(int value) : m_value{ value } {}

        int getValue() const
        {
            return m_value;
        }
    };
}

#ifdef StdFormatter_01_Basic_Formatter_API

// Basic Formatter API

namespace std
{
    using namespace Formatting_Examples_Revised;

    template<>
    struct formatter<SimpleClass>
    {
        // parse the format string for this type
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin(); // should return position of '}' (hopefully)
        }

        // format by always writing its value:
        auto format(const SimpleClass& obj, std::format_context& ctx) const {
            return std::format_to(ctx.out(), "{}", obj.getValue());
        }
    };
}

static void test()
{
    using namespace Formatting_Examples_Revised;

    SimpleClass obj{ 123 };

    std::println("Value: {}", obj);

    std::println("Two Values: {0} - {0}", obj);
}

#endif // StdFormatter_01_Basic_Formatter_API

// ===========================================================================
// ===========================================================================

#ifdef StdFormatter_02_Parsing_Format_String

namespace std
{
    using namespace Formatting_Examples_Revised;

    template<>
    class formatter<SimpleClass>
    {
    private:
        int m_width; // specified width of the field

    public:
        constexpr formatter() : m_width{ 0} {}

        // parse the format string for this type
        constexpr auto parse(std::format_parse_context& ctx)
        {
            auto pos{ ctx.begin() };
            while (pos != ctx.end() && *pos != '}') {
                if (*pos < '0' || *pos > '9') {
                    throw std::format_error{ std::format("invalid format '{}'", *pos) };
                }

                m_width = m_width * 10 + *pos - '0'; // new digit for the width
                ++pos;
            }
            return pos; // shpuld return position of '}'
        }

        // format by always writing its value
        auto format(const SimpleClass& obj, std::format_context& ctx) const {
            return std::format_to(ctx.out(), "{:{}}", obj.getValue(), m_width);
        }
    };
}

static void test()
{
    using namespace Formatting_Examples_Revised;

    try {
        SimpleClass obj{ 123 };
        std::println("{}", obj.getValue());
        std::println("Value: {}", obj);
        std::println("Twice: {0} {0}", obj);
        std::println("With width:       '{:6}'", obj);
        std::println("Twice with width: '{0:6}' = '{1:6}'", obj, obj);
    }
    catch (std::format_error& e) {
        std::println(std::cerr, "Format Error: {}", e.what());
    }
}

#endif // StdFormatter_02_Parsing_Format_String

// ===========================================================================
// ===========================================================================

#ifdef StdFormatter_03_Delegating_Formatting_to_Standard_Formatters

namespace std
{
    using namespace Formatting_Examples_Revised;

    // delegating formatting to standard formatters
    template<>
    class formatter<SimpleClass>
    {
    private:
        // use a standard int formatter that does the work:
        std::formatter<int> m_formatter;

    public:
        // delegate parsing to the standard formatter:
        constexpr auto parse(std::format_parse_context& ctx) {
            return m_formatter.parse(ctx);
        }

        // delegate formatting of the value to the standard formatter:
        auto format(const SimpleClass& obj, std::format_context& ctx) const {
            return m_formatter.format(obj.getValue(), ctx);
        }
    };
}

static void test()
{
    using namespace Formatting_Examples_Revised;

    try {
        SimpleClass obj{ 123 };
        std::println("{}", obj.getValue());
        std::println("Value: {}", obj);
        std::println("Twice: {0} {0}", obj);
        std::println("With width: '{:>20}'", obj);
        std::println("With all:   '{:.^20}'", obj);
    }
    catch (std::format_error& e) {
        std::println(std::cerr, "Format Error: {}", e.what());
    }
}

#endif // StdFormatter_03_Delegating_Formatting_to_Standard_Formatters

// ===========================================================================
// ===========================================================================

#ifdef StdFormatter_04_Inheriting_From_Standard_Formatters

namespace std
{
    using namespace Formatting_Examples_Revised;

    // inheriting From Standard Formatters
    template<>
    struct std::formatter<SimpleClass> : std::formatter<int>
    {
        auto format(const SimpleClass& obj, std::format_context& ctx) const {
            // delegate formatting of the value to the standard formatter
            return std::formatter<int>::format(obj.getValue(), ctx);
        }
    };
}

static void test()
{
    using namespace Formatting_Examples_Revised;

    try {
        SimpleClass obj{ 123 };
        std::println("{}", obj.getValue());
        std::println("Value: {}", obj);
        std::println("Twice: {0} {0}", obj);
        std::println("With width: '{:>20}'", obj);
        std::println("With all:   '{:.^20}'", obj);
    }
    catch (std::format_error& e) {
        std::println(std::cerr, "Format Error: {}", e.what());
    }
}

#endif // StdFormatter_04_Inheriting_From_Standard_Formatters

// ===========================================================================
// ===========================================================================

#ifdef StdFormatter_05_Using_Standard_Formatters_for_Strings

namespace Formatting_Examples_Revised
{
    enum class Color { red, green, blue };
}

namespace std
{
    using namespace Formatting_Examples_Revised;

    // formatter for user defined enum type Color
    template<>
    struct std::formatter<Color> : public std::formatter<std::string>
    {
        auto format(Color col, format_context& ctx) const {

            std::string value{};

            switch (col)
            {
            case Color::red:
                value = "red";
                break;
            case Color::green:
                value = "green";
                break;
            case Color::blue:
                value = "blue";
                break;
            default:
                value = std::format("Color{}", static_cast<int>(col));
                break;
            }

            // delegate the rest of formatting to the string formatter
            return std::formatter<std::string>::format(value, ctx);
        }
    };
}

static void test()
{
    using namespace Formatting_Examples_Revised;

    // using user-provided formatter for enum Color
    for (auto val : { Color::red, Color::green, Color::blue, Color{ 123 } })
    {
        std::println("Color {:_>8} has value 0X{:02X}", val, static_cast<int>(val));
    }
}

#endif // StdFormatter_05_Using_Standard_Formatters_for_Strings

// ===========================================================================
// ===========================================================================

#ifdef StdFormatter_06_Using_Standard_Formatters_for_StdVector

namespace std
{
    using namespace Formatting_Examples_Revised;

    // formatter for std::vector
    template <typename T>
    struct std::formatter<std::vector<T>> : std::formatter<std::string_view>
    {
        constexpr auto parse(format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const std::vector<T>& vec, std::format_context& ctx) const {

            std::string tmp{};

            const auto fmt_str = [&] () {
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

            const auto header = [&]() {

                if (std::is_same<T, int>::value) {
                    return "std::vector<int>";
                }
                else if (std::is_same<T, long>::value) {
                    return "std::vector<long>";
                }
                else if (std::is_same<T, short>::value) {
                    return "std::vector<short>";
                }
                else if (std::is_same<T, float>::value) {
                    return "std::vector<float>";
                }
                else if (std::is_same<T, double>::value) {
                    return "std::vector<double>";
                }
                else {
                    return "std::vector<>";
                }
            }();

            std::format_to(std::back_inserter(tmp), "{} - ", header);

            T lastElem = vec.back();

            std::for_each(
                vec.begin(),
                std::prev(vec.end()),
                [&](const auto& elem){
                    std::format_to(std::back_inserter(tmp), "{}, ", elem);
                }
            );

            std::format_to(std::back_inserter(tmp), "{}", lastElem);

            return std::formatter<string_view>::format(tmp, ctx);
        }
    };
}

static void test()
{
    std::vector<int> intVec = { 1, 2, 3, 4, 5 };
    std::println("{}", intVec);

    std::vector<double> doublesVec = { 1.5, 2.5, 3.5, 4.5, 5.5 };
    std::println("{}", doublesVec);
}

#endif // StdFormatter_06_Using_Standard_Formatters_for_StdVector

// ===========================================================================
// ===========================================================================

void test_formatting_revised_02()
{
    test();
}

// ===========================================================================
// End-of-File
// ===========================================================================
