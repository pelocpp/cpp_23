# Redesign der STL Algorithmen: Range-basierte Algorithmen

[Zurück](Readme.md)

---

[Quellcode](Ranges_01_Algorithms.cpp)

---

## Einleitung

Durch das Hinzufügen der &ldquo;*Ranges*&rdquo;-Bibliothek in C++ 20 haben nahezu alle zuvor existierenden
Algorithmen aus der Header-Datei `<algorithm>` neue Überladungen erhalten.
Man könnte diese Algorithmen als &ldquo;*Constrained Algorithms*&rdquo; bezeichnen,
da sie mit dem ebenfalls neuen C++ Sprachmittel *Concept* definiert sind.

Über die `<algorithm>` Header-Datei sind folglich

  * alle klassischen Iterator-basierten Algorithmen (`std::begin()`, `std::end()`, Namensraum `std`) und
  * alle neuen, `concept`-basierten Algorithmen (Namensraum `std::ranges`)

verfügbar.

Das *Range*-Konzept ist so definiert:

```cpp
template<typename T>
concept range = requires(T& t) {
  ranges::begin(t);
  ranges::end(t);
};
```

Alle C++&ndash;Programme, die STL-Algorithmen verwenden,
können nun in zwei Varianten geschrieben werden, wie das folgende Beispiel zeigt:

###### C++&ndash;17  Variante:

```cpp
auto values = std::vector{ 9, 2, 5, 3, 4 };
std::sort(std::begin(values), std::end(values));
```

###### C++&ndash;23 &ndash; Variante:

```cpp
auto values = std::vector{ 9, 2, 5, 3, 4 };
std::ranges::sort(values);
```

Wir geben nun einen Überblick für die häufigsten Anwendungsfälle für STL-Algorithmen &ndash;
formuliert in der klassischen und in der bereichs-basierten Notation.

*Hinweis*:
Man beachte, dass für alle Beispiele in diesem Abschnitt die Include-Datei `<ranges>`
**nicht** benötigt wird!

### Iteration über einen Bereich

Das Iterieren eines Bereichs betrachten wir am Beispiel einer `print`-Funktion:

###### C++&ndash;17  Variante

```cpp
01: template <typename T>
02: using ValueType = typename std::remove_reference<T>::type::value_type;
03: 
04: template <typename T>
05: static void print(T&& range) {
06:     std::for_each(
07:         std::begin(std::forward<T>(range)),
08:         std::end(std::forward<T>(range)),
09:         [](ValueType<T> i) { std::cout << i << ' '; }
10:     );
11:     std::cout << std::endl;
12: }
13: 
14: static void test()
15: {
16:     auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
17:     print(vec);
18: }
```

*Ausgabe*:

```
5 4 3 2 1 6 7 8 9
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void print(auto&& range)
02: {
03:     for (const auto& elem : range) {
04:         std::print("{} ", elem);
05:     }
06:     std::println("");
07: }
08: 
09: static void test()
10: {
11:     auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
12:     print(vec);
13: }
14: 
```

### Elemente umwandeln (transformieren)

Der `std::transform`-Algorithmus ruft für jedes Element in einem Bereich eine Funktion auf
und speichert das Resultat in einem Ausgabebereich:

###### C++&ndash;17  Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04:     auto res = std::vector<int>(vec.size());
05:     auto lambda = [](auto&& i) { return i * i; };
06: 
07:     std::transform(std::begin(vec), std::end(vec), std::begin(res), lambda);
08:     print(res);
09: }

```

*Ausgabe*:

```
1 4 9 16 25 36 49 64 81 100
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04:     auto res = std::vector<int>(vec.size());
05:     auto lambda = [](auto&& i) { return i * i; };
06: 
07:     std::ranges::transform(vec, std::begin(res), lambda);
08:     print(res);
09: }

```

### Elemente erzeugen

Um allen Elementen eines Bereichs einen Wert zuzuweisen, gibt es die `std::fill`-Funktion.
Mit `std::generate` können wir diesen Wert auch berechnen, sprich *generieren*.
Damit kann der Bereich mit unterschiedlichen Werten vorbelegt werden.

Ein weiterer Algorithmus zum Erzeugen von Elementen ist `std::iota` (Header-Datei `<numeric>`).
`std::iota` generiert Werte in aufsteigender Reihenfolge, es muss dazu ein Startwert
angegeben werden:

###### C++&ndash;17  Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector<int>(5);
04: 
05:     std::fill(std::begin(vec), std::end(vec), 1);
06:     print(vec);
07: 
08:     std::generate(
09:         std::begin(vec),
10:         std::end(vec),
11:         [count = 1]() mutable { return count++; }
12:     );
13:     print(vec);
14: 
15:     std::iota(
16:         std::begin(vec),
17:         std::end(vec),
18:         10
19:     );
20:     print(vec);
21: }
```

*Ausgabe*:

```
1 1 1 1 1
1 2 3 4 5
41 18467 6334 26500 19169
10 11 12 13 14
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void range3_23_generating()
02: {
03:     auto vec = std::vector<int>(5);
04:     std::ranges::fill(vec, 1);
05:     print(vec);
06: 
07:     std::ranges::generate(vec, [count = 1]() mutable { return count++; });
08:     print(vec);
09: 
10:     auto values{ std::ranges::views::iota(10) | std::ranges::views::take(5) };
11:     print(values);
12: }
```

*Ausgabe*:

```
1 1 1 1 1
1 2 3 4 5
41 18467 6334 26500 19169
10 11 12 13 14
```

### Elemente sortieren

Über das Sortieren brauchen wir nicht viel Worte zu verlieren.
Der einfachste Sortier-Algorithmus verbirgt sich hinter `std::sort`:

###### C++&ndash;17  Variante:

```cpp
01: static void test()
02: {
03:     auto values = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
04:     std::sort(std::begin(values), std::end(values));
05:     print(values);
06: }
```

*Ausgabe*:

```
1 2 3 4 5 6 7 8 9
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto values = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
04:     std::ranges::sort(values);
05:     print(values);
06: }
```

### Elemente suchen

Eine weitere sehr häufige Aufgabe besteht darin, herauszufinden,
ob ein bestimmter Wert in einem Bereich enthalten ist oder nicht.
Oder, wie viele Instanzen von einem bestimmten Wert in einem Bereich vorhanden sind.

Prinzipiell sollten wir bei der Suche nach Elementen die Information mit einbeziehen,
ob der Bereich sortiert vorliegt oder nicht. Im Falle eines sortierten Bereichs führt eine lineare Suche
erheblich schneller ans Ziel als in einem unsortierten Bereich.
Wir beginnen mit dem `find()`-Algorithmus &ndash; er erfordert keinen sortierten Bereich:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto values = std::list{ 4, 3, 2, 3, 1 };
04: 
05:     // if the element we are looking for couldn't be found,
06:     // find() returns end() iterator of the collection
07:     auto it = std::find(
08:         std::begin(values),
09:         std::end(values),
10:         2
11:     );
12:     if (it != std::end(values)) {
13:         std::cout << *it << std::endl;
14:     }
15: }
```

*Ausgabe*:

```
2
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto values = std::list{ 4, 3, 2, 3, 1 };
04: 
05:     auto it = std::ranges::find(values, 2);
06:     if (it != std::end(values)) {
07:         std::println("{}", *it);
08:     }
09: }
```

### Elemente mit binärer Suche suchen

Wenn wir wissen, dass der Bereich bereits sortiert ist, können wir einen der binären Suchalgorithmen verwenden:
`binary_search()`, `equal_range()`, `upper_bound()` oder `lower_bound()`.
Wenn wir diese Funktionen zusammen mit Bereichen verwenden, die wahlfreien Zugriff (*random-access*)
für ihre Elemente ermöglichen, benötigen diese nur einen *O(log n)* Zeitaufwand:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };     // sorted!
04: 
05:     bool sorted = std::is_sorted(
06:         std::begin(vec),
07:         std::end(vec)
08:     );
09:     std::cout << "Sorted: " << std::boolalpha << sorted << std::endl;
10: 
11:     bool found = std::binary_search(
12:         std::begin(vec),
13:         std::end(vec),
14:         3
15:     );
16:     std::cout << "Found:  " << std::boolalpha << found << std::endl;
17: }

```

*Ausgabe*:

```
true
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };     // sorted!
04: 
05:     bool sorted = std::ranges::is_sorted(vec);
06:     std::println("Sorted: {}", sorted);
07: 
08:     bool found = std::ranges::binary_search(vec, 3);
09:     std::println("Found:  {}", found);
10: }
```

### Einen Bereich bzgl. einer Bedingung überprüfen

Es gibt drei sehr praktische Algorithmen `all_of()`, `any_of()` und `none_of()`.
Sie besitzen als Parameter einen Bereich und ein unäres Prädikat &ndash; eine Funktion, die ein Argument besitzt
und `true` oder `false` zurückliefert:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector{ 5, 4, 3, 2, 1, 0, -1, 0, 1, 2 };
04:     auto is_negative = [](auto i) { return i < 0; };
05: 
06:     if (std::none_of(std::begin(vec), std::end(vec), is_negative)) {
07:         std::cout << "Contains only positive numbers" << std::endl;
08:     }
09: 
10:     if (std::all_of(std::begin(vec), std::end(vec), is_negative)) {
11:         std::cout << "Contains only negative numbers" << std::endl;
12:     }
13: 
14:     if (std::any_of(std::begin(vec), std::end(vec), is_negative)) {
15:         std::cout << "Contains at least one negative number" << std::endl;
16:     }
17: }
```

*Ausgabe*:

```
Contains at least one negative number
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto vec = std::vector{ 5, 4, 3, 2, 1, 0, -1, 0, 1, 2 };
04:     auto is_negative = [](auto i) { return i < 0; };
05: 
06:     if (std::ranges::none_of(vec, is_negative)) {
07:         std::println("Contains only positive numbers");
08:     }
09: 
10:     if (std::ranges::all_of(vec, is_negative)) {
11:         std::println("Contains only negative numbers");
12:     }
13: 
14:     if (std::ranges::any_of(vec, is_negative)) {
15:         std::println("Contains at least one negative number");
16:     }
17: }
```

### Elemente zählen

Mit `count()` können wir die Anzahl der Elemente in einem Bereich zählen,
die einem bestimmten Wert entsprechen.
Der Zeitaufwand des `count()`-Algorithmus steht in linearem Verhältnis zur Größe des Bereichs.
Ist der Bereich sortiert, wird der Algorithmus mit Zeitaufwand *O(log n)* ausgeführt:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: static void test()
02: {
03:     auto numbers = std::vector{ 3, 3, 2, 1, 3, 1, 3 };
04: 
05:     // counting in linear time
06:     auto n = std::count(std::begin(numbers), std::end(numbers), 3);
07:     std::cout << n << std::endl;
08: 
09:     // counting in O(log n) time
10:     std::sort(std::begin(numbers), std::end(numbers));
11:     std::pair<std::vector<int>::iterator, std::vector<int>::iterator> result =
12:         std::equal_range(std::begin(numbers), std::end(numbers), 3);
13:     n = std::distance(result.first, result.second);
14:     std::cout << n << std::endl;
15: }
```

*Ausgabe*:

```
4
4
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void range8_23_counting()
02: {
03:     auto numbers = std::vector{ 3, 3, 2, 1, 3, 1, 3 };
04: 
05:     // counting in linear time
06:     auto n = std::ranges::count(numbers, 3);
07:     std::println("{}", n);
08: 
09:     // counting in O(log n) time
10:     std::ranges::sort(numbers);
11:     std::ranges::subrange result = std::ranges::equal_range(numbers, 3);
12:     n = std::ranges::size(result);
13:     std::println("{}", n);
14: }
```

### Minimum, Maximum, und &ldquo;Clamping&rdquo;

Wir betrachten die drei Funktionen `min()`, `max()` und `clamp()` an einem Beispiel:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: // minimum, maximum, and clamping
02: const auto min = 1;
03: const auto max = 100;
04: 
05: int some_func() { return 50; }
06: 
07: static void test()
08: {
09:     auto y{ 0 };
10:     y = std::min(some_func(), max);
11:     y = std::max(std::min(some_func(), max), min);
12:     y = std::clamp(some_func(), min, max);
13: 
14:     auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9, 10 };
15:     auto min_iter = std::min_element(std::begin(vec), std::end(vec));
16:     auto max_iter = std::max_element(std::begin(vec), std::end(vec));
17:     std::cout << "Min: " << *min_iter << ", Max: " << *max_iter << std::endl;
18: }
```

*Ausgabe*:

```
Min: 1, Max: 10
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: // minimum, maximum, and clamping
02: const auto min = 1;
03: const auto max = 100;
04: 
05: int some_func() { return 50; }
06: 
07: static void test()
08: {
09:     auto y{ 0 };
10:     y = std::min(some_func(), max);
11:     y = std::max(std::min(some_func(), max), min);
12:     y = std::clamp(some_func(), min, max);
13: 
14:     const auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9, 10 };
15:     auto min_iter = std::ranges::min_element(vec);
16:     auto max_iter = std::ranges::max_element(vec);
17:     std::println("Min: {}, Max: {}", *min_iter, *max_iter);
18: 
19:     const auto [min, max] = std::ranges::minmax(vec);
20:     std::println("Min: {}, Max: {}", min, max);
21: }
```

*Ausgabe*:

```
Min: 1, Max: 10
Min: 1, Max: 10
```

### Lambda-Funktionen versus &ldquo;*Projektionen*&rdquo;

Manchmal müssen wir Objekte vergleichen, ohne dabei den Standardvergleich zu verwenden,
der &ndash; wenn überhaupt &ndash; durch den `==`-Operator zur Verfügung gestellt wird.
In diesen Fällen akzeptieren viele Algorithmen eine Lambda-Funktion als zusätzliches Argument.

Die &ldquo;*Constrained*&rdquo; Algorithmen, also die Algorithmen im Namensraum `std::ranges`,
offerieren zu diesem Zweck eine abgekürzte Notation, die
man als *Projektion* bezeichnet. *Projektionen* arbeiten in der Regel mit Standardprädikaten
wie `std::less` oder `std::greater` zusammen.

Vergleichen Sie zu diesem Zweck die beiden nachfolgenden Beispiele:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: static void test1()
02: {
03:     auto names = std::vector<std::string>{
04:         "Alexander", "Dennis", "Bjarne", "Ken", "Stephen", "Dave"
05:     };
06: 
07:     std::sort(
08:         std::begin(names),
09:         std::end(names),
10:         [](const std::string& a, const std::string& b) {
11:             return a.size() < b.size();
12:         }
13:     );
14:     print(names);
15: 
16:     // find names with length 3
17:     auto result = std::find_if(
18:         std::begin(names),
19:         std::end(names),
20:         [](const auto& v) { return v.size() == 3; }
21:     );
22:     if (result != std::end(names)) {
23:         std::cout << *result << std::endl;
24:     }
25:     std::cout << std::endl;
26: }
27: 
28: static void test2()
29: {
30:     struct Task {
31:         std::string m_desc{};
32:         unsigned int m_priority{ 0 };
33:     };
34: 
35:     std::vector<Task> tasks{
36:         { "Clean up my apartment", 10 },
37:         { "Finish homework", 5 },
38:         { "Go to the supermarket", 12 }
39:     };
40: 
41:     auto print = [](auto&& t) {
42:         std::cout << t.m_desc << ": Priority: " << t.m_priority << std::endl;
43:     };
44: 
45:     std::cout << "List of tasks:" << std::endl;
46:     std::for_each(std::begin(tasks), std::end(tasks), print);
47:     std::sort(
48:         std::begin(tasks),
49:         std::end(tasks),
50:         [](const Task& a, const Task& b) {
51:             return a.m_priority > b.m_priority;
52:         }
53:     );
54:     std::cout << "Next priorities:" << std::endl;
55:     std::for_each(std::begin(tasks), std::end(tasks), print);
56:     std::cout << std::endl;
57: }
```

*Ausgabe*:

```
Ken Dave Dennis Bjarne Stephen Alexander
Ken
List of tasks:
Clean up my apartment: Priority: 10
Finish homework: Priority: 5
Go to the supermarket: Priority: 12
Next priorities:
Go to the supermarket: Priority: 12
Clean up my apartment: Priority: 10
Finish homework: Priority: 5
```

###### C++&ndash;23 &ndash; Variante:

```cpp
01: static void test1()
02: {
03:     auto names = std::vector<std::string>{
04:         "Alexander", "Dennis", "Bjarne", "Ken", "Stephen", "Dave"
05:     };
06: 
07:     // i) names is passed as the first argument
08:     // ii) {} means the default template argument - in this case, it's std::less<>{} as comparator
09:     // iii) the projection is a callable which takes a single argument
10: 
11:     std::ranges::sort(names, {}, &std::string::size);
12:     std::ranges::sort(names, std::less<>{}, & std::string::size);
13:     print(names);
14: 
15:     // find names with length 3
16:     auto result = std::ranges::find(names, 3, &std::string::size);
17:     if (result != std::end(names)) {
18:         std::println("{}", *result);
19:     }
20:     std::println("");
21: }
22: 
23: static void test2()
24: {
25:     struct Task {
26:         std::string m_desc{};
27:         unsigned int m_priority{ 0 };
28:     };
29: 
30:     std::vector<Task> tasks{
31:         { "Clean up my apartment", 10 },
32:         { "Finish homework", 5 },
33:         { "Go to the supermarket", 12 }
34:     };
35: 
36:     auto print = [](auto&& t) {
37:         std::println("{}: Priority: {}", t.m_desc, t.m_priority);
38:     };
39: 
40:     std::println("List of tasks:");
41:     std::ranges::for_each(tasks, print);
42:     std::ranges::sort(tasks, std::ranges::greater{}, &Task::m_priority); // <<  "extract" a data member 
43:     std::println("Next priorities:");
44:     std::ranges::for_each(tasks, print);
45: }
```

---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen zum großen Teil aus dem Buch

&ldquo;[C++ High Performance](https://www.amazon.de/High-Performance-Master-optimizing-functioning/dp/1839216549/)&rdquo;
von Björn Andrist und Viktor Sehr.

Das Beispiel zu Projektionen wurde

[C++20 Ranges, Projections, `std::invoke` and `if constexpr`](https://www.cppstories.com/2020/10/understanding-invoke.html/)

entnommen.

---

[Zurück](Readme.md)

---
