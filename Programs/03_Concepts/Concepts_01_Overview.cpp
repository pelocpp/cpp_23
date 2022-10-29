// ===========================================================================
// Concepts_01_Overview.cpp
// ===========================================================================

#include <iostream>

template <typename A, typename B>
void sumAndPrint_01(const A& a, const B& b)
{
    std::cout << (a + b);
    std::cout << std::endl;
}

template <typename T, typename U = T>
concept Sumable =
requires(T a, U b)
{
    { a + b };
    { b + a };
};

template <Sumable A, Sumable B>
void sumAndPrint_02(const A& a, const B& b)
{
    std::cout << (a + b);
    std::cout << std::endl;
}

template <typename T>
concept SumableOnlyForIntsAndDoubles = std::is_same<T, int>::value || std::is_same<T, double>::value;

template <SumableOnlyForIntsAndDoubles A, SumableOnlyForIntsAndDoubles B>
void sumAndPrint_03(const A& a, const B& b)
{
    std::cout << (a + b);
    std::cout << std::endl;
}

void example_concepts_01()
{
    sumAndPrint_01(1, 2);
    sumAndPrint_01(1.0F, 2);

    sumAndPrint_02(1, 2);
    sumAndPrint_02(1.0F, 2);

    sumAndPrint_03(1, 2);
    // sumAndPrint_03(1.0F, 2);  // the associated constraints are not satisfied
}

// ===============================================================

class Adder
{
private:
    int m_value{};

public:
    Adder (int value) : m_value { value } {}

    Adder operator+(const Adder& other) const
    {
        return { m_value + other.m_value };
    }

    int operator()() const { return m_value; }
};

void example_concepts_02()
{
    sumAndPrint_02(Adder{ 1 }, Adder{ 2 });
}

template <typename T, typename U = T>
concept Sumable2 =
requires(T a, U b)
{
    { a + b };
    { b + a };
}
&& requires(std::ostream& os, const T& a)
{
    { os << a };
};

std::ostream& operator<<(std::ostream& os, const Adder& n)
{
    os << n();
    return os;
}

template <Sumable2 A, Sumable2 B>
void sumAndPrint_04(const A& a, const B& b)
{
    std::cout << (a + b);
    std::cout << std::endl;
}

void example_concepts_03()
{
    // sumAndPrint_03(Adder{ 1 }, Adder{ 2 });  // the associated constraints are not satisfied
    sumAndPrint_04(Adder{ 1 }, Adder{ 2 });
}

// ===============================================================

template <typename G>
concept Greeter = requires(G g)
{
    { g.say_hi() } -> std::convertible_to<void>;
};

struct SpanishGreeter
{
    void say_hi() const { 
        std::cout << "Hola amigos" << std::endl;
    }
};

struct EnglishGreeter
{
    void say_hi() const { 
        std::cout << "Hello my friends" << std::endl;
    }
};

struct ItalianGreeter
{
    void say_hi() const {
        std::cout << "Ciao Ragazzi" << std::endl;
    }
};

// ===============================================================

template <Greeter G>
void greet(const G& greeter)
{
    greeter.say_hi();
}

void example_concepts_04()
{
    greet(SpanishGreeter{});
    greet(EnglishGreeter{});
    greet(ItalianGreeter{});
}

// ===============================================================

void example_overview()
{
    example_concepts_01();
    example_concepts_02();
    example_concepts_03();
    example_concepts_04();
}

// ===========================================================================
// End-of-File
// ===========================================================================
