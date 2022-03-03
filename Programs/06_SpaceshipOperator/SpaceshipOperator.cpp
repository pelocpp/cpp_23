// ===========================================================================
// SpaceshipOperator.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <compare>
#include <set>
#include <vector>

namespace Spaceship_01_Operator_Equality
{
    class Point {
    private:
        int m_x;
        int m_y;

    public:
        Point() = default;
        Point(int x, int y) : m_x{ x }, m_y{ y } { };

        bool operator==(const Point& other) const {
            return m_x == other.m_x && m_y == other.m_y;
        }
    };

    void test_10()
    {
        Point p1{ 1, 1 };
        Point p2{ 2, 2 };

        std::cout << std::boolalpha;
        std::cout << (p1 == p2) << std::endl;
        std::cout << (p1 != p2) << std::endl;   // works since C++ 20 (!)
    }
}

namespace Spaceship_02_BuiltIn
{
    void test_20()
    {
        int a{ 1 };
        int b{ 2 };

        bool b1 = (a <=> b) < 0;        // true if a < b
        bool b2 = (a <=> b) > 0;        // true if a > b
        bool b3 = (a <=> b) == 0;       // true if a is equal / equivalent to b

        std::cout << std::boolalpha;
        std::cout << b1 << std::endl;
        std::cout << b2 << std::endl;
        std::cout << b3 << std::endl;
    }

    void test_21()
    {
        int a{ 10 };
        int b{ 11 };

        auto res = a <=> b;

        if (res < 0)
            std::cout << "a < b" << std::endl;
        else if (res == 0)
            std::cout << "a == b" << std::endl;
        else if (res > 0)
            std::cout << "a > b" << std::endl;

        std::string str1{ "ABC" };
        std::string str2{ "DEF" };

        auto res2 = str1 <=> str2;

        if (res2 < 0)
            std::cout << "str1 < str2" << std::endl;
        else if (res2 == 0)
            std::cout << "str1 == str2" << std::endl;
        else if (res2 > 0)
            std::cout << "str1 > str2" << std::endl;

        std::vector<int> vec1{ 4, 5 };
        std::vector<int> vec2{ 1, 2, 3 };

        auto res3 = vec1 <=> vec2;

        if (res3 < 0)
            std::cout << "vec1 < vec2" << std::endl;
        else if (res3 == 0)
            std::cout << "vec1 == vec2" << std::endl;
        else if (res3 > 0)
            std::cout << "vec1 > vec2" << std::endl;
    }
}

namespace Spaceship_03_Operator_Point
{
    class Point {
    private:
        int m_x;
        int m_y;

    public:
        Point() : Point{ 0, 0 } {}
        Point(int x, int y) : m_x{ x }, m_y{ y } { };

        auto operator<=>(const Point&) const = default;
    };

    void test_30()
    {
        Point p1{ 1, 4 };
        Point p2{ 1, 5 };

        std::cout << std::boolalpha;
        std::cout << (p1 == p2) << std::endl;
        std::cout << (p1 != p2) << std::endl;
        std::cout << (p1 < p2) << std::endl;
        std::cout << (p1 <= p2) << std::endl;
        std::cout << (p1 > p2) << std::endl;
        std::cout << (p1 >= p2) << std::endl;
    }

    class PointEx {
    private:
        int m_x;
        int m_y;

    public:
        PointEx() : PointEx{ 0, 0 } {}
        PointEx(int x, int y) : m_x{ x }, m_y{ y } { };

        std::strong_ordering operator<=>(const PointEx& other) const {

            if (m_x < other.m_x)
                return std::strong_ordering::less;

            if (m_x > other.m_x)
                return std::strong_ordering::greater;

            if (m_y < other.m_y)
                return std::strong_ordering::less;

            if (m_y > other.m_y)
                return std::strong_ordering::greater;

            return std::strong_ordering::equal;
        }

        // Note:
        // We need to explicitly define operator==,
        // because operator<=> is not defaulted !

        bool operator==(const PointEx& other) const = default;
    };

    void test_31()
    {
        PointEx p1{ 1, 1 };
        PointEx p2{ 1, 2 };

        std::cout << std::boolalpha;
        std::cout << (p1 == p2) << std::endl;
        std::cout << (p1 != p2) << std::endl;
        std::cout << (p1 < p2) << std::endl;
        std::cout << (p1 <= p2) << std::endl;
        std::cout << (p1 > p2) << std::endl;
        std::cout << (p1 >= p2) << std::endl;
    }
}

namespace Spaceship_04_Operator_Fraction
{
    class Fraction
    {
    private:
        // private member data
        int m_num;    // numerator
        int m_denom;  // denominator

    public:
        // c'tors
        Fraction() : m_num{ 0 }, m_denom{ 1 } { };
        Fraction(int num, int denom) : m_num(num), m_denom(denom) { };

        bool operator==(const Fraction& other) const = default;

        std::strong_ordering operator<=>(const Fraction& other) const {

            if (m_num == other.m_num && m_denom == other.m_denom) {
                return std::strong_ordering::equal;
            }

            if (m_num * other.m_denom < m_denom * other.m_num)
            {
                return std::strong_ordering::less;
            }
            else {
                return std::strong_ordering::greater;
            }
        }

        // input / output operators
        friend std::ostream& operator<< (std::ostream&, const Fraction&);
    };

    // output operator
    std::ostream& operator<< (std::ostream& os, const Fraction& f)
    {
        os << f.m_num << '/' << f.m_denom;
        return os;
    }

    void test_41()
    {
        Fraction f1{ 1, 7 };
        Fraction f2{ 3, 7 };

        std::cout << std::boolalpha;
        std::cout << (f1 == f2) << std::endl;
        std::cout << (f1 != f2) << std::endl;
        std::cout << (f1 < f2) << std::endl;
        std::cout << (f1 <= f2) << std::endl;
        std::cout << (f1 > f2) << std::endl;
        std::cout << (f1 >= f2) << std::endl;
    }

    void test_42()
    {
        std::set<Fraction> numbers;

        numbers.insert(Fraction{ 3, 8 });
        numbers.insert(Fraction{ 1, 7 });
        numbers.insert(Fraction{ 1, 2 });
        numbers.insert(Fraction{ 1, 7 });

        for (const auto& number : numbers) {
            std::cout << number << std::endl;
        }
    }
}

// ===============================================================

void test_spaceship_operator()
{
    using namespace Spaceship_01_Operator_Equality;
    test_10();

    using namespace Spaceship_02_BuiltIn;
    test_20();
    test_21();

    using namespace Spaceship_03_Operator_Point;
    test_30();
    test_31();

    using namespace Spaceship_04_Operator_Fraction;
    test_41();
    test_42();
}

// ===========================================================================
// End-of-File
// ===========================================================================
