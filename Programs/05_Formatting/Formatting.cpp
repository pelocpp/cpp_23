// ===========================================================================
// Formatting.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <format>

namespace Formatting_Examples
{
    // Verwendung einfacher Variablenersetzungen
    void test_01a()
    {
        std::string result{ std::format("Format arguments: {}, {}, {}.", 1, 2, 3) };
        std::cout << result << std::endl;
    }

    void test_01b()
    {
        int age = 30;
        std::string name{ "Hans" };

        std::string result{
            std::format("My name is {} and I'm {} years old", name, age)
        };
        std::cout << result << std::endl;
    }

    void test_01()
    {
        test_01a();
        test_01b();
    }

    // Reihenfolge der Argumente definieren
    void test_02a(std::string_view format)
    {
        std::string_view one{ "One" };
        std::string_view two{ "Two" };
        std::string_view three{ "Three" };

        std::string result = std::format(format, one, two, three);
        std::cout << result << std::endl;
    }

    void test_02b(std::string_view format)
    {
        std::string_view one{ "One" };
        std::string_view two{ "Two" };
        std::string_view three{ "Three" };

        std::string result = std::format(format, one, two, three);
        std::cout << result << std::endl;
    }

    void test_02()
    {
        std::string_view format1{ "{0}, {1}, {2}" };
        test_02a(format1);

        std::string_view format2{ "{2}, {1}, {0}" };
        test_02b(format2);
    }

    // from https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification
    void test_03()
    {
        char c = 120;
        std::cout << std::format("{:6}", 42) << std::endl;     // value of s0 is "    42"
        std::cout << std::format("{:6}", 'x') << std::endl;    // value of s1 is "x     "
        std::cout << std::format("{:*<6}", 'x') << std::endl;  // value of s2 is "x*****"
        std::cout << std::format("{:*>6}", 'x') << std::endl;  // value of s3 is "*****x"
        std::cout << std::format("{:*^6}", 'x') << std::endl;  // value of s4 is "**x***"
        std::cout << std::format("{:6d}", c) << std::endl;     // value of s5 is "   120"
        std::cout << std::format("{:6}", true) << std::endl;   // value of s6 is "true  "
    }

    // from https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification
    void test_04()
    {
        float pi = 3.14f;
        std::cout << std::format("{:10f}", pi) << std::endl;           // s1 = "  3.140000" (width = 10)
        std::cout << std::format("{:{}f}", pi, 10) << std::endl;       // s2 = "  3.140000" (width = 10)
        std::cout << std::format("{:.5f}", pi) << std::endl;           // s3 = "3.14000" (precision = 5)
        std::cout << std::format("{:.{}f}", pi, 5) << std::endl;       // s4 = "3.14000" (precision = 5)
        std::cout << std::format("{:10.5f}", pi) << std::endl;         // s5 = "   3.14000" (width = 10, precision = 5)
        std::cout << std::format("{:{}.{}f}", pi, 10, 5) << std::endl; // s6 = "   3.14000" (width = 10, precision = 5)
    }

    // Ganze Zahlen in unterschiedlichen Formaten
    void test_05()
    {
        std::cout << std::format("Decimal:     {} {:d} {:06} {:06d} {:0} {:+} {:+}", 1, 2, 3, 4, 5, 6, -7) << std::endl;
        std::cout << std::format("Hexadecimal: {:x} {:x} {:X} {:#x}", 5, 10, 10, 5) << std::endl;
        std::cout << std::format("Octal:       {:o} {:#o} {:#o}", 12, 12, 4) << std::endl;
        std::cout << std::format("Binary:      {:b} {:#b} {:#B}", 15, 15, 5) << std::endl;
    }

    // Fließkommazahlen
    void test_06()
    {
        std::cout << std::format("Default:     {} {:} {:g} {:g}", 1.5, 1.5, 1.5, 1e20) << std::endl;
        std::cout << std::format("Rounding:    {:f} {:.0f} {:.20f}", 1.5, 1.5, 1.5) << std::endl;
        std::cout << std::format("Padding:     {:05.2f} {:.2f} {:5.2f}", 1.5, 1.5, 1.5) << std::endl;
        std::cout << std::format("Scientific:  {:E} {:e}", 1.5, 1.5) << std::endl;
    }
}

// ===============================================================

void test_formatting()
{
    using namespace Formatting_Examples;

    test_01();
    test_02();
    test_03();
    test_04();
    test_05();
    test_06();
}

// ===========================================================================
// End-of-File
// ===========================================================================
