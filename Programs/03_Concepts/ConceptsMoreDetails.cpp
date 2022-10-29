// ===========================================================================
// ConceptsMoreDetails.cpp
// ===========================================================================

#include <iostream>
#include <type_traits>
#include <string>

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

class Foo {};

void concept_detail_03()
{
    static_assert(Addable<int, int> == true);
    static_assert(Addable<int, long> == true);
    static_assert(not Addable<int, Foo> == true);
    static_assert(not Addable<int, std::string> == true);
}


// ---------------------------------------------------------------------------

void example_more_details()
{
}

// ===========================================================================
// End-of-File
// ===========================================================================
