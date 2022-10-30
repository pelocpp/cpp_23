# Weitere Beispiele

[Zur�ck](Readme.md)

---

[Quellcode](Concepts_04_MoreExamples.cpp)

---



## Ein gr��eres Beispiel


###### Beispiel:

*Anforderungen*:

  1. Der Typ muss den Operator `+` bereitstellen.
  2. Nur gleiche Typen d�rfen an `arg` �bergeben werden.
  3. Mindestens zwei Parameter sind erforderlich (kein leeres Parameter Pack).
  4. Operator `+` gibt ein Objekt des gleichen Typs zur�ck.


*Vorbereitungen*:

```cpp
template<typename T, typename...>
struct first_type
{
    using type = T;
};

template<typename... Args>
using first_type_t = typename first_type<Args...>::type;

template<typename T, typename... Ts>
constexpr bool are_same_v = std::conjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Args>
concept same_as_first_type =
    std::is_same_v<std::remove_cvref_t<T>,
    std::remove_cvref_t<first_type_t<Args...>>>;
```


*Haupteil*:


```cpp
template<typename... Args>
requires requires(Args... args)
{
    (... + args);                    // Simple requirement
    requires are_same_v<Args...>;    // Nested requirement with type - trait
    requires sizeof...(Args) > 1;    // Nested requirement with a boolean expression asserts at least 2 parameters
    {
        (... + args)
    }
    ->same_as_first_type<Args...>;   // Same compound requirement ensuring same type
}
auto add(Args&&... args)
{
    return (... + args);
}
```

---


## Literaturhinweise:

Die Anregungen zu den Beispielen stammen aus einem Blog von *Andreas Fertig*:

[C++20 Concepts: Testing constrained functions](https://andreasfertig.blog/2020/08/cpp20-concepts-testing-constrained-functions/)


---

[Zur�ck](Readme.md)

---
