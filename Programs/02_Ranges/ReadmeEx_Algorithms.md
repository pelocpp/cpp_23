# Redesign der Standard Library Algorithmen

[Zurück](ReadmeEx.md)

---

[Quellcode: Ranges01.cpp](Ranges01.cpp)<br/>
[Quellcode: Ranges02.cpp](Ranges02.cpp)

---

## Einleitung

Durch das Hinzufügen der &ldquo;*Ranges*&rdquo;-Bibliothek in C++ 20 haben nahezu alle zuvor existierenden
Algorithmen aus der Header-Datei `<algorithm>` neue Überladungen erhalten.
Man könnte diese Algorithmen als &ldquo;*Constrained Algorithms*&rdquo; bezeichnen,
da sie mit dem ebenfalls neuen C++ Sprachmittel `concept` definiert sind.

Über die `<algorithm>` Header-Datei sind folglich

  * alle klassischen iterator-basierten Algorithmen (`std::begin()`, `std::end()`, Namensraum `std`) und
  * alle neuen, `concept`-basierten Algorithmen (Namensraum `std::ranges`)

verfügbar.

Das `range`-Konzept ist so definiert:

```cpp
template< class T >
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

###### C++&ndash;20 &ndash; Variante:

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
05: void print(T&& r) {
06:     std::for_each(
07:         std::begin(std::forward<T>(r)),
08:         std::end(std::forward<T>(r)),
09:         [](ValueType<T> i) { std::cout << i << ' '; }
10:     );
11:     std::cout << std::endl;
12: }
13: 
14: template <typename T>
15: void print2(T&& r) {
16:     std::for_each(
17:         std::begin(std::forward<T>(r)),
18:         std::end(std::forward<T>(r)),
19:         [](auto&& i) { std::cout << i << ' '; }
20:     );
21:     std::cout << std::endl;
22: }
23: 
24: void test()
25: {
26:     auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
27:     print(vec);
28: }
```

*Ausgabe*:

```
5 4 3 2 1 6 7 8 9
```

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void print(auto&& r) {
02:     std::ranges::for_each(r, [](auto&& i) { std::cout << i << ' '; });
03:     std::cout << std::endl;
04: }
05: 
06: template <typename T>
07: void print2(T&& r) {
08:     std::ranges::for_each(std::forward<T>(r), [](auto&& i) { std::cout << i << ' '; });
09:     std::cout << std::endl;
10: }
11: 
12: void test()
13: {
14:     auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9 };
15:     print(vec);
16: }
```

### Elemente umwandeln (transformieren)

Der `std::transform`-Algorithmus ruft für jedes Element in einem Bereich eine Funktion auf
und speichert das Resultat in einem Ausgabebereich:

###### C++&ndash;17  Variante:

```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04:     auto res = std::vector<int>(vec.size());
05:     auto lambda = [](auto&& i) { return i * i; };
06:     std::transform(std::begin(vec), std::end(vec), std::begin(res), lambda);
07:     print(res);
08: }
```

*Ausgabe*:

```
1 4 9 16 25 36 49 64 81 100
```

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04:     auto res = std::vector<int>(vec.size());
05:     auto lambda = [](auto&& i) { return i * i; };
06:     std::ranges::transform(vec, std::begin(res), lambda);
07:     print(res);
08: }
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
01: void test()
02: {
03:     std::fill(
04:         std::begin(vec),
05:         std::end(vec),
06:         1
07:     );
08:     print(vec);
09: 
10:     std::generate(
11:         std::begin(vec),
12:         std::end(vec),
13:         [count = 1]() mutable { return count++; }
14:     );
15:     print(vec);
16: 
17:     std::generate(
18:         std::begin(vec),
19:         std::end(vec),
20:         std::rand
21:     );
22:     print(vec);
23: 
24:     std::iota(
25:         std::begin(vec),
26:         std::end(vec),
27:         10
28:     );
29:     print(vec);
30: }
```

*Ausgabe*:

```
1 1 1 1 1
1 2 3 4 5
41 18467 6334 26500 19169
10 11 12 13 14
```

###### C++&ndash;20 &ndash; Variante:

*Bemerkung*: `std::ranges::iota` wird aktuell von C++ 20 noch nicht unterstützt.

```cpp
01: auto vec = std::vector<int>(5);
02: std::ranges::fill(vec, 1);
03: print(vec);
04: std::ranges::generate(vec, [count = 1]() mutable { return count++; });
05: print(vec);
06: std::ranges::generate(vec, std::rand);
07: print(vec);
```

*Ausgabe*:

```
1 1 1 1 1
1 2 3 4 5
41 18467 6334 26500 19169
```

### Elemente sortieren

Über das Sortieren brauchen wir nicht viel Worte zu verlieren.
Der einfachste Sortier-Algorithmus verbirgt sich hinter `std::sort`:

###### C++&ndash;17  Variante:

```cpp
01: void test()
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

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
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
01: void test()
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

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
02: {
03:     auto values = std::list{ 4, 3, 2, 3, 1 };
04:     auto it = std::ranges::find(values, 2);
05:     if (it != std::end(values)) {
06:         std::cout << *it << std::endl;
07:     }
08: }
```

### Elemente mit binärer Suche suchen

Wenn wir wissen, dass der Bereich bereits sortiert ist, können wir einen der binären Suchalgorithmen verwenden:
`binary_search()`, `equal_range()`, `upper_bound()` oder `lower_bound()`.
Wenn wir diese Funktionen zusammen mit Bereichen verwenden, die wahlfreien Zugriff (*random-access*)
für ihre Elemente ermöglichen, benötigen diese nur einen *O(log n)* Zeitaufwand:

###### C++&ndash;17 &ndash; Variante:

```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };   // sorted!
04: 
05:     assert(std::is_sorted(
06:         std::begin(vec),
07:         std::end(vec))
08:     );
09: 
10:     bool found = std::binary_search(
11:         std::begin(vec),
12:         std::end(vec),
13:         3
14:     );
15:     std::cout << std::boolalpha << found << std::endl;
16: }
```

*Ausgabe*:

```
true
```

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 2, 2, 3, 3, 3, 4, 5 };     // sorted!
04:     assert(std::ranges::is_sorted(vec));
05:     bool found = std::ranges::binary_search(vec, 3);
06:     std::cout << std::boolalpha << found << std::endl;
07: }
```

### Einen Bereich bzgl. einer Bedingung überprüfen

Es gibt drei sehr praktische Algorithmen `all_of()`, `any_of()` und `none_of()`.
Sie besitzen als Parameter einen Bereich und ein unäres Prädikat &ndash; eine Funktion, die ein Argument besitzt
und `true` oder `false` zurückliefert:

###### C++&ndash;17 &ndash; Variante:

```cpp
01:     auto vec = std::vector{ 5, 4, 3, 2, 1, 0, -1, 0, 1, 2 };
02:     auto is_negative = [](auto i) { return i < 0; };
03: 
04:     if (std::none_of(std::begin(vec), std::end(vec), is_negative)) {
05:         std::cout << "Contains only positive numbers" << std::endl;
06:     }
07: 
08:     if (std::all_of(std::begin(vec), std::end(vec), is_negative)) {
09:         std::cout << "Contains only negative numbers" << std::endl;
10:     }
11: 
12:     if (std::any_of(std::begin(vec), std::end(vec), is_negative)) {
13:         std::cout << "Contains at least one negative number" << std::endl;
14:     }
```

*Ausgabe*:

```
Contains at least one negative number
```

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 5, 4, 3, 2, 1, 0, -1, 0, 1, 2 };
04:     auto is_negative = [](auto i) { return i < 0; };
05: 
06:     if (std::ranges::none_of(vec, is_negative)) {
07:         std::cout << "Contains only positive numbers" << std::endl;
08:     }
09: 
10:     if (std::ranges::all_of(vec, is_negative)) {
11:         std::cout << "Contains only negative numbers" << std::endl;
12:     }
13: 
14:     if (std::ranges::any_of(vec, is_negative)) {
15:         std::cout << "Contains at least one negative number" << std::endl;
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
01: void test()
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

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
02: {
03:     auto numbers = std::vector{ 3, 3, 2, 1, 3, 1, 3 };
04: 
05:     // counting in linear time
06:     auto n = std::ranges::count(numbers, 3);
07:     std::cout << n << std::endl;
08: 
09:     // counting in O(log n) time
10:     std::ranges::sort(numbers);
11:     std::ranges::subrange result = std::ranges::equal_range(numbers, 3);
12:     n = std::ranges::size(result);
13:     std::cout << n << std::endl;
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
07: void test()
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

###### C++&ndash;20 &ndash; Variante:

```cpp
01: // minimum, maximum, and clamping
02: const auto min = 1;
03: const auto max = 100;
04: 
05: int some_func() { return 50; }
06: 
07: void test()
08: {
09:     auto y{ 0 };
10:     y = std::min(some_func(), max);
11:     y = std::max(std::min(some_func(), max), min);
12:     y = std::clamp(some_func(), min, max);
13: 
14:     const auto vec = std::vector{ 5, 4, 3, 2, 1, 6, 7, 8, 9, 10 };
15:     auto min_iter = std::ranges::min_element(vec);
16:     auto max_iter = std::ranges::max_element(vec);
17:     std::cout << "Min: " << *min_iter << ", Max: " << *max_iter << std::endl;
18: 
19:     const auto [min, max] = std::ranges::minmax(vec);
20:     std::cout << "Min: " << min << ", Max: " << max << std::endl;
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
01: void test()
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
25: }
26: 
27: void test2()
28: {
29:     struct Task {
30:         std::string m_desc{};
31:         unsigned int m_priority{ 0 };
32:     };
33: 
34:     std::vector<Task> tasks{
35:         { "Clean up my apartment", 10 },
36:         { "Finish homework", 5 },
37:         { "Go to the supermarket", 12 }
38:     };
39: 
40:     auto print = [](auto&& t) {
41:         std::cout << t.m_desc << ": Priority: " << t.m_priority << std::endl;
42:     };
43: 
44:     std::cout << "List of tasks:" << std::endl;
45:     std::for_each(std::begin(tasks), std::end(tasks), print);
46:     std::sort(
47:         std::begin(tasks),
48:         std::end(tasks),
49:         [](const Task& a, const Task& b) {
50:             return a.m_priority > b.m_priority;
51:         }
52:     );
53:     std::cout << "Next priorities:" << std::endl;
54:     std::for_each(std::begin(tasks), std::end(tasks), print);
55: }
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

###### C++&ndash;20 &ndash; Variante:

```cpp
01: void test()
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
18:         std::cout << *result << std::endl;
19:     }
20: }
21: 
22: void test2()
23: {
24:     struct Task {
25:         std::string m_desc{};
26:         unsigned int m_priority{ 0 };
27:     };
28: 
29:     std::vector<Task> tasks{
30:         { "Clean up my apartment", 10 },
31:         { "Finish homework", 5 },
32:         { "Go to the supermarket", 12 }
33:     };
34: 
35:     auto print = [](auto&& t) {
36:         std::cout << t.m_desc << ": Priority: " << t.m_priority << std::endl;
37:     };
38: 
39:     std::cout << "List of tasks:" << std::endl;
40:     std::ranges::for_each(tasks, print);
41:     std::ranges::sort(tasks, std::ranges::greater{}, &Task::m_priority); // "extract" a data member 
42:     std::cout << "Next priorities:" << std::endl;
43:     std::ranges::for_each(tasks, print);
44: }
```

---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen zum großen Teil aus dem Buch

&ldquo;[C++ High Performance](https://www.amazon.de/High-Performance-Master-optimizing-functioning/dp/1839216549/)&rdquo;
von Björn Andrist und Viktor Sehr.

Das Beispiel zu Projektionen wurde

[C++20 Ranges, Projections, std::invoke and if constexpr](https://www.cppstories.com/2020/10/understanding-invoke.html/)

entnommen.

---

[Zurück](ReadmeEx.md)

---
