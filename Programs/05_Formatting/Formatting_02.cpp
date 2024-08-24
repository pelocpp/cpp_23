// ===========================================================================
// Formatting_02.cpp
// ===========================================================================

#include <iostream>
#include <format>

#include <string>
#include <string_view>

namespace Formatting_Examples_Josuttis
{
    class Always111 {
    public:
        int getValue() const { return 111; }
    };

    class Always222 {
    public:
        int getValue() const { return 222; }
    };

    class Always333 {
    public:
        int getValue() const { return 333; }
    };

    class Always444 {
    public:
        int getValue() const { return 444; }
    };
}

namespace std
{
    using namespace Formatting_Examples_Josuttis;

    template<>
    struct formatter<Always111>
    {
        // parse the format string for this type:
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin(); // return position of } (hopefully there)
        }
        // format by always writing its value:
        auto format(const Always111& obj, std::format_context& ctx) const {
            return std::format_to(ctx.out(), "{}", obj.getValue());
        }
    };
}

namespace std
{
    using namespace Formatting_Examples_Josuttis;

    template<>
    class formatter<Always222>
    {
    private:
        int width = 0; // specified width of the field

    public:
        // parse the format string for this type:
        constexpr auto parse(std::format_parse_context& ctx)
        {
            auto pos = ctx.begin();
            while (pos != ctx.end() && *pos != '}') {
                if (*pos < '0' || *pos > '9') {
                    throw std::format_error{ std::format("invalid format '{}'", *pos) };
                }
                width = width * 10 + *pos - '0'; // new digit for the width
                ++pos;
            }
            return pos; // return position of }
        }

        // format by always writing its value:
        auto format(const Always222& obj, std::format_context& ctx) const {
            return std::format_to(ctx.out(), "{:{}}", obj.getValue(), width);
        }
    };
}

namespace std
{
    //*** formatter for type Always333:
    template<>
    class formatter<Always333>
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
        auto format(const Always333& obj, std::format_context& ctx) const {
            return m_formatter.format(obj.getValue(), ctx);
        }
    };
}

namespace std
{
    //*** formatter for type Always42:
    // - use standard int formatter
    template<>
    struct std::formatter<Always444> : std::formatter<int>
    {
        auto format(const Always444& obj, std::format_context& ctx) const {
            // delegate formatting of the value to the standard formatter:
            return std::formatter<int>::format(obj.getValue(), ctx);
        }
    };
}


namespace Formatting_Examples_Josuttis
{
    enum class Color { red, green, blue };
}

namespace std
{
    using namespace Formatting_Examples_Josuttis;

    // *** formatter for enum type Color:
    template<>
    struct std::formatter<Color> : public std::formatter<std::string>
    {
        auto format(Color c, format_context& ctx) const {
            // initialize a string for the value:
            std::string value;
            switch (c) {
             //   using enum Color;
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
                value = std::format("Color{}", static_cast<int>(c));
                break;
            }
            // and delegate the rest of formatting to the string formatter:
            return std::formatter<std::string>::format(value, ctx);
        }
    };

    //*** formatter for enum type Color:
    //template<>
    //struct formatter<MyColor> : public std::formatter<std::string>
    //{
    //    auto format(MyColor c, format_context& ctx) const {
    //        // initialize a string for the value:

    //        std::string value{};

    //        switch (c) {
    //            
    //        case MyColor::red:
    //            value = "red";
    //            break;
    //        case MyColor::green:
    //            value = "green";
    //            break;
    //        case MyColor::blue:
    //            value = "blue";
    //            break;
    //        default:
    //            value = std::format("Color{}", static_cast<int>(c));
    //            break;
    //        }

    //        // and delegate the rest of formatting to the string formatter:
    //        return std::formatter<std::string>::format(value, ctx);
    //    }
    //};
}

//namespace std
//{
//    using namespace Formatting_Examples_Josuttis;
//
//    //*** formatter for enum type Color:
//    template<>
//    class formatter<MyColor> // : public std::formatter<std::string>
//    {
//    private:
//        // use a standard int formatter that does the work:
//        mutable std::formatter<std::string> m_formatter;
//
//    public:
//        // delegate parsing to the standard formatter:
//        constexpr auto parse(std::format_parse_context& ctx) {
//            return m_formatter.parse(ctx);
//        }
//
//        auto format(const MyColor& obj, std::format_context& ctx) const {
//
//            std::string value{"ROT"};
//
//            // return m_formatter.format(obj.getValue(), ctx);
//            return m_formatter.format(value, ctx);
//        }
//
//        //auto format(Color c, format_context& ctx) const {
//        //    // initialize a string for the value:
//
//        //    std::string value{};
//
//        //    switch (c) {
//
//        //    case Color::red:
//        //        value = "red";
//        //        break;
//        //    case Color::green:
//        //        value = "green";
//        //        break;
//        //    case Color::blue:
//        //        value = "blue";
//        //        break;
//        //    default:
//        //        // value = std::format("Color{}", static_cast<int>(c));
//        //        break;
//        //    }
//
//        //    // and delegate the rest of formatting to the string formatter:
//        //    return std::formatter<std::string>::format(value, ctx);
//        //}
//    };
//}


namespace Formatting_Examples_Josuttis
{
    static void test_01()
    {
        Always111 val;
        std::cout << std::format("Value: {}\n", val);
        std::cout << std::format("Twice: {0} {0}\n", val);
    }

    static void test_02()
    {
        try {
            Always222 val;
            std::cout << val.getValue() << '\n';
            std::cout << std::format("Value: {}\n", val);
            std::cout << std::format("Twice: {0} {0}\n", val);
            std::cout << std::format("With width: '{:10}'\n", val);
            std::cout << std::format("Twice with width: '{0:10}' = '{1:10}'\n", val, val);
          //  std::cout << std::format("With width: '{:f}'\n", val);
        }
        catch (std::format_error& e) {
            std::cerr << "Format Error: " << e.what() << std::endl;
        }
    }

    static void test_03()
    {
        try {
            Always333 val;
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

    static void test_04()
    {
        try {
            Always444 val;
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

    static void test_05()
    {

    }
}

// ===========================================================================
// ===========================================================================

void test_formatting_02()
{
    using namespace Formatting_Examples_Josuttis;

    test_01();
    test_02();
    test_03();
    test_04();
    test_05();

}

// ===========================================================================
// End-of-File
// ===========================================================================
