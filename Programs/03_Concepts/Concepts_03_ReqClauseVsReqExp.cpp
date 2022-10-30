// ===========================================================================
// Concepts_03_ReqClauseVsReqExp.cpp
// ===========================================================================

#include <iostream>
#include <sstream>
#include <concepts>
#include <type_traits>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <future>
#include <utility>

// ---------------------------------------------------------------------------
// Concept with all 4 kinds of requires expressions

template <typename T>
concept Fooable = requires(T a)
{
    // simple requirements
    a++;                                      // can be post-incremented
    ++a;                                      // can be pre-incremented

    // type requirements
    typename T::value_type;                   // has inner type member value_type

    // compound requirements
    { a + 1 } -> std::convertible_to<T>;      // a + 1 is a valid expression AND
    // its result must be convertible to T

    // nested requirements
    requires std::same_as<T*, decltype(&a)>;  // operator& returns the same type as T*
};

// ---------------------------------------------------------------------------
// A requires expression is a compile-time expression of the type bool

struct point
{
    int x;
    int y;
};

std::ostream& operator<<(std::ostream& os, point const& p)
{
    os << '(' << p.x << ',' << p.y << ')';
    return os;
}

template <typename T>
constexpr bool always_false = std::false_type::value;

template <typename T>
std::string as_string(T a)
{
    constexpr bool has_to_string = requires(T x)
    {
        { std::to_string(x) } -> std::convertible_to<std::string>;
    };

    constexpr bool has_stream = requires(T x, std::ostream & os)
    {
        {os << x} -> std::same_as<std::ostream&>;
    };

    if constexpr (has_to_string)
    {
        return std::to_string(a);
    }
    else if constexpr (has_stream)
    {
        std::stringstream s;
        s << a;
        return s.str();
    }
    else
        static_assert(always_false<T>, "The type cannot be serialized");
}

void requires_expression_01()
{
    std::cout << as_string(42) << '\n';
    std::cout << as_string(point{ 1, 2 }) << '\n';
   // std::cout << as_string(std::pair<int, int>{1, 2}) << '\n'; // error: The type cannot be serialized
}

// ---------------------------------------------------------------------------
// curious 'requires requires expr' syntax

template <typename T>
T increment(T a) requires std::integral<T>
{
    return a + 1;
}

template <typename T> requires std::integral<T>
T increment(T a)
{
    return a + 1;
}

template <typename T>
T increment(T a) requires std::integral<T> || std::floating_point<T>
{
    return a + 1;
}

template <typename T> requires std::integral<T> || std::floating_point<T>
T increment(T a)
{
    return a + 1;
}

template <typename T>
T increment(T a) requires requires (T x) { x + 1; }
{
    return a + 1;
}

template <typename T> requires requires (T x) { x + 1; }
T increment(T a)
{
    return a + 1;
}

//template <typename T>
//T increment(T a) requires !std::floating_point<T>   // wrapping parenthesis must be used
//{
//    return a + 1;
//}

template <typename T>
T increment(T a) requires (!std::floating_point<T>)   // wrapping parenthesis must be used
{
    return a + 1;
}


// ---------------------------------------------------------------------------


void example_requires_clause_vs_requires_expression()
{
    requires_expression_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
