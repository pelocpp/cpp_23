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

static void test_01();
static void test_02();

// #define StdFormatter_01_Basic_Formatter_API
#define StdFormatter_02_Parsing_Format_String
//#define StdFormatter_03_Delegating_Formatting_to_Standard_Formatter

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

static void test_01()
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

        // parse the format string for this type:
        constexpr auto parse(std::format_parse_context& ctx)
        {
            auto pos = ctx.begin();
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

// WEITER: Drittes Beispiel !!!

static void test_02()
{
    using namespace Formatting_Examples_Revised;

    try {
        SimpleClass obj{ 123 };
        std::cout << obj.getValue() << '\n';
        std::println("Value: {}", obj);
        std::println("Twice: {0} {0}", val);
        std::println("With width:       '{:6}'", val);
        std::println("Twice with width: '{0:6}' = '{1:6}'", val, val);
    }
    catch (std::format_error& e) {
        std::cerr << "Format Error: " << e.what() << std::endl;
    }
}

#endif // StdFormatter_02_Parsing_Format_String

// ===========================================================================
// ===========================================================================

#ifdef StdFormatter_03_Delegating_Formatting_to_Standard_Formatter

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

static void test_03()
{
    using namespace Formatting_Examples_Revised;

    try {
        SimpleClass val;
        std::cout << val.getValue() << '\n';
        std::cout << std::format("Value: {}\n", val);
        std::cout << std::format("Twice: {0} {0}\n", val);
        std::cout << std::format("With width: '{:>20}'\n", val);
        std::cout << std::format("With all:   '{:.^20}'\n", val);
    }
    catch (std::format_error& e) {
        std::cerr << "Format Error: " << e.what() << std::endl;
    }
}

#endif // StdFormatter_03_Delegating_Formatting_to_Standard_Formatter

// ===========================================================================
// ===========================================================================

void test_formatting_revised_02()
{
    test_01();
    test_02();
    test_03();

}

// ===========================================================================
// End-of-File
// ===========================================================================
