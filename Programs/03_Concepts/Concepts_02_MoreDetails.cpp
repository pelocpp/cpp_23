// ===========================================================================
// Concepts_02_MoreDetails.cpp
// ===========================================================================

#include <iostream>
#include <type_traits>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <future>

// ---------------------------------------------------------------------------
// Empty Test Class

class Empty {};

// ---------------------------------------------------------------------------
// Concept Definition

template <typename T>
concept Small = sizeof(T) <= sizeof(int);

void concept_detail_01()
{
    static_assert(Small<char> == true);
    static_assert(Small<short> == true);
    static_assert(Small<int> == true);

    static_assert(not Small<double> == true);
    static_assert(not Small<long double> == true);
    static_assert(not Small<long long> == true);
}


template <class T>
concept Integral = std::is_integral_v<T>;

template <class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template <class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

void concept_detail_02()
{
    static_assert(UnsignedIntegral<unsigned short> == true);
    static_assert(UnsignedIntegral<unsigned int> == true);

    static_assert(SignedIntegral<int> == true);
    static_assert(SignedIntegral<long> == true);
}

// ---------------------------------------------------------------------------
// Simple Requirement

template<class T, class U>
concept Addable = requires (const T & t, const U & u) 
{
    t + u;
    u + t;
};

void concept_detail_03()
{
    static_assert(Addable<int, int> == true);
    static_assert(Addable<int, long> == true);
    static_assert(not Addable<int, Empty> == true);
    static_assert(not Addable<int, std::string> == true);
}

// ---------------------------------------------------------------------------
// Type Requirement

template<class T>
concept HasValueType = requires {
    typename T::value_type;
};

void concept_detail_04()
{
    static_assert(HasValueType<std::vector<int>> == true);
    static_assert(HasValueType<std::vector<std::string>> == true);
    static_assert(not HasValueType<Empty> == true);
}

// ---------------------------------------------------------------------------
// Compound Requirement

template<class T, class U>
concept Same = std::is_same<T, U>::value;

template<class T>
concept EqualityComparable = requires (const T& a, const T& b) {
    { a == b } -> Same<bool>;
    { a != b } -> Same<bool>;
};

void concept_detail_05()
{
    static_assert(EqualityComparable<int> == true);
    static_assert(EqualityComparable<std::unique_ptr<int>> == true);
    static_assert(not EqualityComparable<std::thread> == true);
    static_assert(not EqualityComparable<std::future<int>> == true);
}

template <typename T>
concept HasSquare = requires (T t) {
    { t.square() } -> std::convertible_to<int>;
};


// ---------------------------------------------------------------------------
// Nested Requirement

// ---------------------------------------------------------------------------

void example_more_details()
{
}

// ===========================================================================
// End-of-File
// ===========================================================================
