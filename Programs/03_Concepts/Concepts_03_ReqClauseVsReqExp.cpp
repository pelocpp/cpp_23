// ===========================================================================
// Concepts_03_ReqClauseVsReqExp.cpp
// ===========================================================================

#include <iostream>
#include <concepts>
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

    static_assert(not (Small<double> == true));
    static_assert(not (Small<long double> == true));
    static_assert(not (Small<long long> == true));
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
    static_assert(not (Addable<int, Empty> == true));
    static_assert(not (Addable<int, std::string> == true));
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
    static_assert(not (HasValueType<Empty> == true));
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
    static_assert(not (EqualityComparable<std::thread> == true));
    static_assert(not (EqualityComparable<std::future<int>> == true));
}

// ---------------------------------------------------------------------------
// Compound Requirement
// Another Example

template <typename T>
concept HasConvertibleToIntSquare = requires (T t) {
    { t.square() } -> std::convertible_to<int>;
};

template <typename T>
concept HasIntSquare = requires (T t) {
    { t.square() } -> std::same_as<int>;
};

class IntWithIntSquare {
public:
    IntWithIntSquare(int num) : m_num(num) {}
    int square() const {
        return m_num * m_num;
    }
private:
    int m_num;
};

class IntWithLongSquare {
public:
    IntWithLongSquare(int num) : m_num(num) {}
    long square() const {
        return m_num * m_num;
    }
private:
    int m_num;
};

class IntWithVoidSquare {
public:
    IntWithVoidSquare(int num) : m_num(num) {}
    void square() const {
        std::cout << m_num * m_num << '\n';
    }
private:
    int m_num;
};


template <typename T>
requires HasIntSquare<T>
void printSquareSame_01(T number) {
    std::cout << number.square() << '\n';
}

template <typename T>
void printSquareSame_02(T number) requires HasIntSquare<T> {
    std::cout << number.square() << '\n';
}

template <HasIntSquare T>
void printSquareSame_03(T number) {
    std::cout << number.square() << '\n';
}

// using abbreviated function template syntax
void printSquareSame(HasIntSquare auto number) {
    std::cout << number.square() << '\n';
}

// using abbreviated function template syntax
void printSquareConvertible(HasConvertibleToIntSquare auto number) {
    std::cout << number.square() << '\n';
}

void concept_detail_06()
{
    static_assert (HasIntSquare<IntWithIntSquare> == true);
    static_assert (not (HasIntSquare<IntWithLongSquare> == true));
    static_assert (not (HasIntSquare<IntWithVoidSquare> == true));

    static_assert (HasConvertibleToIntSquare<IntWithIntSquare> == true);
    static_assert (HasConvertibleToIntSquare<IntWithLongSquare> == true);
    static_assert (not (HasConvertibleToIntSquare<IntWithVoidSquare> == true));

    printSquareSame(IntWithIntSquare{ 1 });               // int same as int
    //printSquareSame(IntWithLongSquare{2});              // long not same as int
    //printSquareSame(IntWithVoidSquare{3});              // void not same as int
    printSquareConvertible(IntWithIntSquare{ 4 });        // int convertible to int
    printSquareConvertible(IntWithLongSquare{ 5 });       // int convertible to int
    // printSquareConvertible(IntWithVoidSquare{6});      // void not convertible to int
}

// In the above example, we can observe that the class with void square() const
// doesn’t satisfy either the HasIntSquare or the HasConvertibleToIntSquare concepts.

// IntWithLongSquare, so the class with the function long square() const
// doesn’t satisfy the concept HasIntSquare as long is not the same as int,
// but it does satisfy the HasConvertibleToIntSquare concept as long is convertible to int.

// Class IntWithIntSquare satisfies both concepts as an int is obviously the same
// as intand it’s also convertible to an int.


// ---------------------------------------------------------------------------
// Nested Requirement

struct AwesomeCabrio {
    void openRoof() {}
    void startEngine() {}
};

struct CoolCoupe {
    void startEngine() {}
};

template<typename TCar>
concept Car = requires (TCar car) {
    car.startEngine();
};

template<typename TCar>
concept Convertible = Car<TCar> && requires (TCar car) {  // Convertible Car == Kabrio
    car.openRoof();
};

// Nested Concept:
template<typename TCar>
concept Coupe = Car<TCar> && requires (TCar car) {       // Coupe == Zweisitzer
    requires !Convertible<TCar>;
};

// Same Concept -- written as Simple Concept:
template<typename TCar>
concept Coupe_02 = Car<TCar> && !Convertible<TCar>;

void concept_detail_07()
{
    static_assert (Convertible<AwesomeCabrio> == true);
    static_assert (not (Coupe<AwesomeCabrio> == true));  // nested requirement '! Convertible<C>' is not satisfied
    static_assert (Coupe<CoolCoupe> == true);
}

// ---------------------------------------------------------------------------
// Nested Requirement
// Another Example

struct Droid {
    Droid clone() {
        return Droid{};
    }
};

struct Droid_Version_02 {
    Droid clones() {
        return Droid{};   // obviously an error
    }
};

// In this example, we have two droid types, Droid and Droid_Version_02.
// We expect that droids should be clonable meaning that each type
// should have a clone method that returns another droid of the same type.
// With Droid_Version_02 we made a mistakeand it still returns Droid.

//struct Droid_Version_02 {
//    Droid_Version_02 clone() {
//        return Droid_Version_02{}; // correct version
//    }
//};

template<typename C>
concept Clonable = requires (C clonable) {
    clonable.clone();
    requires std::same_as<C, decltype(clonable.clone())>;
};

template<typename C>
concept Clonable_02 = requires (C clonable) {
    { clonable.clone() } -> std::same_as<C>;
};

void concept_detail_08()
{
    Clonable auto c = Droid{};
    // Clonable auto c2 = Droid_Version_02{};    // nested requirement 'same_as<C, decltype (clonable.clone())>' is not satisfied

    Clonable_02 auto c3 = Droid{};
    // Clonable_02 auto c4 = Droid_Version_02{}; // nested requirement 'same_as<C, decltype (clonable.clone())>' is not satisfied

    static_assert (Clonable<Droid> == true);
    static_assert (not (Clonable<Droid_Version_02> == true));

    static_assert (Clonable_02<Droid> == true);
    static_assert (not (Clonable_02<Droid_Version_02> == true));
}


// ======
// 
// 


template<typename T, typename U>
    requires requires (T a, U b) { { a > b } -> Same<bool>; }
auto max(T a, U b) -> std::remove_reference_t<decltype(a > b ? a : b)>
{
    return a > b ? a : b;
}



// ---------------------------------------------------------------------------

void example_more_details()
{
    concept_detail_01();
    concept_detail_02();
    concept_detail_03();
    concept_detail_04();
    concept_detail_05();
    concept_detail_06();
    concept_detail_07();
    concept_detail_08();
}

// ===========================================================================
// End-of-File
// ===========================================================================
