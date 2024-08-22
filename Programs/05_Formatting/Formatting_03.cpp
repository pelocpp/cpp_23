// ===========================================================================
// Formatting_02.cpp
// ===========================================================================

#include <iostream>
#include <format>

#include <string>
#include <string_view>

namespace Formatting_Examples_Bancila
{
    struct MyColor
    {
        uint8_t r{ 0 };
        uint8_t g{ 0 };
        uint8_t b{ 0 };
    };
}

namespace std
{
    using namespace Formatting_Examples_Bancila;

    // Realisierung 1
    //template <>
    //struct formatter<MyColor>
    //{
    //    constexpr auto parse(std::format_parse_context& ctx) {
    //        return ctx.begin();
    //    }

    //    auto format(const MyColor& col, std::format_context& ctx) const {
    //        return std::format_to(ctx.out(), "({}, {}, {})", col.r, col.g, col.b);
    //    }
    //};

    // Realisierung 2
    template <>
    struct std::formatter<MyColor> : std::formatter<string_view> {
        auto format(const MyColor& col, std::format_context& ctx) const {
            std::string temp;

            std::format_to(std::back_inserter(temp), "({:03d}, {:03d}, {:03d})",
                col.r, col.g, col.b);

            return std::formatter<string_view>::format(temp, ctx);
        }
    };
}

static void test_01()
{
    using namespace Formatting_Examples_Bancila;

    MyColor col{ 1, 2, 3 };

    std::cout << std::format("Color {}\n", col);
}

void test_formatting_03()
{
    using namespace Formatting_Examples_Bancila;

    test_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
