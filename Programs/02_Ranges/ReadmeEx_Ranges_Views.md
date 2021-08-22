# Ranges und Views / Bereiche und Sichten

[Zurück](ReadmeEx.md)

---

[Quellcode: Ranges01.cpp](Ranges01.cpp)<br/>
[Quellcode: Ranges02.cpp](Ranges02.cpp)

---

## Einleitung

Folgende Beweggründe haben zur Entwicklung der &ldquo;*Ranges*&rdquo;-Bibliothek geführt:

  * Die Fähigkeit, Algorithmen miteinander verknüpfen zu können &ndash; und dies ohne unnötige temporäre Container-Kopien.
  * Mit dem Sprachmittel `concept`, das *Requirements* an Iteratoren und Bereiche stellt, kann der Quellcode
    jetzt besser vom Compiler geprüft werden.
  * *Views* aus der &ldquo;*Ranges*&rdquo;-Bibliothek sind *das* mächtige Hilfsmittel, die es ermöglichen,
    auf einem Ausgangsbereich mehrere Transformationen in einer &ldquo;*lazy*&rdquo; evaluierten Darstellung miteinander zu verknüpfen.
    Dabei kommt es **nicht** zur Erzeugung von überflüssigen Zwischenkopien des Ausgangsbereichs.


## Motivation

Wir wollen die Aussagen aus der Einleitung an einem Beispiel verdeutlichen:

###### C++&ndash;17  Variante

<pre>
01: void test()
02: {
03:     struct Student {
04:         std::string m_name{};
05:         int m_year{};
06:         int m_score{};
07:     };
08: 
09:     auto getMaxScore = [](const std::vector<Student>& students, int year)
10:     {
11:         auto byYear = [=](const auto& s) { return s.m_year == year; };
12: 
13:         // student list needs to be <b>copied</b> in order to be filtered on the year <b>!!!</b>
14:         auto result = std::vector<Student>{};
15: 
16:         std::copy_if(
17:             std::begin(students),
18:             std::end(students),
19:             std::back_inserter(result),
20:             byYear
21:         );
22: 
23:         auto it = std::max_element(
24:             std::begin(result),
25:             std::end(result),
26:             [](const Student& a, const Student& b) {
27:                 return a.m_score < b.m_score;
28:             }
29:         );
30: 
31:         return it != result.end() ? it->m_score : 0;
32:     };
33: 
34:     // Although it's easily achievable in this small example to find a fitting solution,
35:     // the goal is to to be able to implement this algorithm by composing small algorithmic building blocks,
36:     // rather than implementing it for every use case from scratch using a single for-loop
37:     auto getMaxScoreAlternate = [](const std::vector<Student>& students, int year) {
38:         auto max_score = 0;
39:         for (const auto& student : students) {
40:             if (student.m_year == year) {
41:                 max_score = std::max(max_score, student.m_score);
42:             }
43:         }
44:         return max_score;
45:     };
46: 
47:     auto students = std::vector<Student>{
48:         {"Georg", 2021, 120 },
49:         {"Hans",  2021, 140 },
50:         {"Susan", 2020, 180 },
51:         {"Mike",  2020, 110 },
52:         {"Hello", 2021, 190 },
53:         {"Franz", 2021, 110 },
54:     };
55: 
56:     auto score = getMaxScore(students, 2021);
57: 
58:     std::cout << score << std::endl;
59: }
</pre>

*Ausgabe*:

```
190
```

###### C++&ndash;20 &ndash; Variante:

<pre>
01: void v1_20()
02: {
03:     struct Student {
04:         std::string m_name{};
05:         int m_year{};
06:         int m_score{};
07:     };
08: 
09:     auto maxValue = [](auto&& range) {
10:         const auto it = std::ranges::max_element(range);
11:         return it != range.end() ? *it : 0;
12:     };
13: 
14:     auto getMaxScore = [&](const std::vector<Student>& students, int year) {
15:         const auto byYear = [=](auto&& s) { return s.m_year == year; };
16:         return maxValue(students
17:             | std::views::filter(byYear)
18:             | std::views::transform(&Student::m_score)
19:         );
20:     };
21: 
22:     auto students = std::vector<Student>{
23:         {"Georg", 2021, 120 },
24:         {"Hans",  2021, 140 },
25:         {"Susan", 2020, 180 },
26:         {"Mike",  2020, 110 },
27:         {"Hello", 2021, 190 },
28:         {"Franz", 2021, 110 },
29:     };
30: 
31:     auto score = getMaxScore(students, 2021);
32: 
33:     std::cout << score << std::endl;
34: }
</pre>


## Ein erster Blick auf *Views*

*Views* in der &ldquo;*Ranges*&rdquo;-Bibliothek sind *Lazy* ausgewertete Iterationen über einen Bereich.

Technisch betrachtet sind sie sehr ähnlich mit Iteratoren, nur mit integrierter Logik.

Syntaktisch gesehen bieten sie eine sehr angenehme Syntax für viele gängige Operationen:

*Beispiel*:
Quadrieren aller Elemente eines Containers

```cpp
01: void test()
02: {
03:     auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04: 
05:     auto square = [](auto v) { return v * v; };
06:     auto squared_view = std::views::transform(numbers, square);
07:     for (auto s : squared_view) { // The square lambda is invoked here
08:         std::cout << s << ", ";
09:     }
10:     std::cout << std::endl;
11: }
```

*Ausgabe*:

```
1, 4, 9, 16, 25, 36, 49, 64, 81, 100,
```

*Beispiel*:
Erstellen einer gefilterte Ansicht, in der nur ein Teil des Bereichs sichtbar ist.
In diesem Fall treten nur die Elemente in Erscheinung, die die Bedingung erfüllen,
wenn die Ansicht iteriert wird:

```cpp
01: void test()
02: {
03:     auto numbers = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
04: 
05:     auto oddView = std::views::filter(numbers, [](auto i) { return (i % 2) == 1; });
06:     for (auto odd : oddView) {
07:         std::cout << odd << ", ";
08:     }
09:     std::cout << std::endl;
10: }
```

*Ausgabe*:

```
1, 3, 5, 7, 9,
```

*Beispiel*:
Ein weiteres Beispiel für die Vielseitigkeit der Ranges-Bibliothek ist die Möglichkeit,
eine Ansicht zu erstellen, die über mehrere Container iterieren kann, als ob sie eine einzelne Liste wären:


```cpp
01: void test()
02: {
03:     auto list_of_lists = std::vector<std::vector<int>>{
04:         { 1, 2 },
05:         { 3, 4, 5 },
06:         { 6 },
07:         { 7, 8, 9, 10 }
08:     };
09: 
10:     auto flattenedView = std::views::join(list_of_lists);
11: 
12:     for (auto v : flattenedView)
13:         std::cout << v << ", ";
14:     std::cout << std::endl;
15: 
16:     auto maxValue = *std::ranges::max_element(flattenedView);
17:     std::cout << "Maximum value: " << maxValue << std::endl;
18: }
```

*Ausgabe*:

```
1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
Maximum value: 10
```

## Komposition von *Views*: *Views* können miteinander verknüpft werden

Die eigentliche Stärke von *Views* ,liegt darin, dass sie miteinander verknüpft werden können.
Die sie die zugrunden liegenden Daten *nicht* kopieren, lassen sich mehrere Transformationen
auf einem Bereich formulieren, was intern nur in Iterationen resultiert.

Das erste Beispiel verwendet die tatsächlichen *Views*-Klassen direkt.
Dies bedeutet, dass der visuell mächtige Pipe-Operator noch nicht zum Einsatz gelangt:

```cpp
01: void test()
02: {
03:     struct Student {
04:         std::string m_name{};
05:         int m_year{};
06:         int m_score{};
07:     };
08: 
09:     auto getMaxScore = [](const std::vector<Student>& s, int year) {
10: 
11:         auto byYear = [=](const auto& s) { return s.m_year == year; };
12: 
13:         auto v1 = std::ranges::ref_view{ s }; // wrap container in a view
14:         auto v2 = std::ranges::filter_view{ v1, byYear }; // apply 'filter' view
15:         auto v3 = std::ranges::transform_view{ v2, &Student::m_score };  // apply 'transform' view
16:         auto it = std::ranges::max_element(v3); // apply 'max_element' view
17:         return it != v3.end() ? *it : 0;
18:     };
19: 
20:     auto students = std::vector<Student>{
21:         {"Georg", 2021, 120 },
22:         {"Hans",  2021, 140 },
23:         {"Susan", 2020, 180 },
24:         {"Mike",  2020, 110 },
25:         {"Hello", 2021, 190 },
26:         {"Franz", 2021, 110 },
27:     };
28: 
29:     auto score = getMaxScore(students, 2021);
30:     std::cout << score << std::endl;
31: }
```

*Ausgabe*:

```
190
```

Wir beginnen mit der Erstellung eines `std::ranges::ref_view`-Objekts,
einer Hülle um einen Container.
In unserem Fall verwandelt es das `std::vector`-Objekt in eine Ansicht.
Dies erfüllt den Zweck, dass die nächste Ansicht, Klasse `std::ranges::filter_view`, zum Einsatz kommen kann,
da sie als ersten Parameter eine Ansicht erfordert.
Auf diese Weise kann man eine Kette von komponierbaren Ansichten bilden,
die beliebig lang sein kann.

Wir könnten das letzte Beispiel auch kompakter formulieren,
indem wir die temporären Variablen `v1`, `v2` und `v3` entfernen:

```
01: auto getMaxScoreAlternate = [](const std::vector<Student>& s, int year) {
02: 
03:     auto byYear = [=](const auto& s) { return s.m_year == year; };
04: 
05:     auto scores = std::ranges::transform_view{
06:         std::ranges::filter_view {
07:             std::ranges::ref_view{s}, 
08:             byYear
09:         },
10:         &Student::m_score
11:     };
12: 
13:     auto it = std::ranges::max_element(scores);
14:     return it != scores.end() ? *it : 0;
15: };
```

*Hinweis*: Die `_view`-Klassen wurden dem Namensraum  `std::ranges` zugeordnet.

## Bereichs *Views* besitzen Bereichsadaptoren

Wie Sie bereits gesehen haben, können wir mit der &ldquo;*Ranges*&rdquo;-Bibliothek auch Ansichten erstellen,
die mit Pipe-Operatoren &ndash; und eben dann mit Bereichsadaptoren &ndash; eine viel elegantere Syntax besitzen.

Die Fähigkeit, eine Anweisung von links nach rechts anstatt sie von innen nach außen zu lesen, macht den
Code viel leichter zu lesen:

*Hinweis*: Die Bereichsadaptoren wurden dem Namensraum  `std::views` zugeordnet.


```cpp
01: void test()
02: {
03:     struct Student {
04:         std::string m_name{};
05:         int m_year{};
06:         int m_score{};
07:     };
08: 
09:     auto students = std::vector<Student>{
10:         {"Georg", 2021, 120 },
11:         {"Hans",  2021, 140 },
12:         {"Susan", 2020, 180 },
13:         {"Mike",  2020, 110 },
14:         {"Hello", 2021, 190 },
15:         {"Franz", 2021, 110 },
16:     };
17: 
18:     int year = 2021;
19:     auto byYear = [=](const auto& s) { return s.m_year == year; };
20: 
21:     auto scores = students | std::views::filter(byYear) | std::views::transform(&Student::m_score);
22: 
23:     auto it = std::ranges::max_element(scores);
24:     auto score = it != scores.end() ? *it : 0;
25: 
26:     std::cout << score << std::endl;
27: }
```

*Ausgabe*:

```
190
```

Jede *View* in der &ldquo;*Ranges*&rdquo;-Bibliothek verfügt über ein entsprechendes Bereichsadapterobjekt, das
zusammen mit dem Pipe-Operator verwendet werden.

Bei Verwendung der Bereichsadapterobjekte können wir auch das
zusätzliche `std::ranges::ref_view`-Objekt weglassen!


## *Views* verändern den zugrunde liegenden Container nicht!

Auf den ersten Blick könnte eine Ansicht wie eine veränderte Version des Eingabecontainers aussehen.
Dies ist nicht der Fall: Die gesamte Verarbeitung wird in den Iterator-Objekten durchgeführt,
eine *View* ist nur ein *Proxy*-Objekt!

```cpp
01: void test()
02: {
03:     auto ints = std::list{ 1, 2, 3, 4, 5, 6 };
04: 
05:     auto strings = ints | std::views::transform([](auto i) {
06:         return std::string{"\""} + std::to_string(i) + std::string{ "\"" };
07:         }
08:     );
09: 
10:     for (const auto& s : strings) {
11:         std::cout << s << ", ";
12:     }
13:     std::cout << std::endl;
14: 
15:     for (const auto& i : ints) {
16:         std::cout << i << ", ";
17:     }
18: }
```

*Ausgabe*:

```
"1", "2", "3", "4", "5", "6",
1, 2, 3, 4, 5, 6,
```

Prinzipiell stellt sich damit natürlich die Frage, wie Resultate von Transformationen &ndash; also *Views* &ndash; in
einem Ergebnis-Container abgespeichert werden können.

## *Views* lassen sich in Containers &ldquo;materialisieren&rdquo;

Der Ergebnis der Transformation von *Views*  kann man in einem Container abspeichern.
Man spricht hier auch von &ldquo;die Ansicht materialisieren&rdquo;.

Alle *Views* können prinzipiell in Containern materialisiert werden, aber:
Eine Funktions-Template namens `std::ranges::to<T>()`, die diese Funktionalität besitzt,
wurde für C++&ndash;20 vorgeschlagen, hat es aber dann doch nicht zur Verabschiedung in den Standard geschafft.

Es gibt allerdings einen einfachen Workaround mit der Funktion `std::ranges::copy()`, den wir im Folgenden vorstellen:

```cpp
01: auto to_vector(auto&& r)
02: {
03:     std::vector<std::ranges::range_value_t<decltype(r)>> v;
04: 
05:     if constexpr (std::ranges::sized_range<decltype(r)>) {
06:         v.reserve(std::ranges::size(r));
07:     }
08: 
09:     std::ranges::copy(r, std::back_inserter(v));
10: 
11:     return v;
12: }
13: 
14: 
15: void test()
16: {
17:     auto ints = std::list{ 1, 2, 3, 4, 5, 6 };
18: 
19:     auto strings = ints | std::views::transform([](auto i) {
20:         return std::string{ "\"" } + std::to_string(i) + std::string{ "\"" };
21:         }
22:     );
23: 
24:     auto vec = std::vector<std::string>{};
25: 
26:     std::ranges::copy(strings, std::back_inserter(vec));
27: 
28:     for (const auto& s : vec) {
29:         std::cout << s << ", ";
30:     }
31:     std::cout << std::endl;
32: 
33:     vec.clear();
34: 
35:     vec = to_vector(strings);
36: 
37:     for (const auto& s : vec) {
38:         std::cout << s << ", ";
39:     }
40: }
```

*Ausgabe*:

```
"1", "2", "3", "4", "5", "6",
"1", "2", "3", "4", "5", "6",
```

*Hinweis*:
Wir einem Aufruf von `reserve()` lässt sich die Performance der Funktion `to_vector` optimieren.
Es wird vorab genug Platz für alle Elemente im Ergebnis-Container allokiert, um weitere, überflüssige Allokationen zu vermeiden.
Jedoch kann man `reserve()` nur aufrufen, wenn die Methode am Ausgangscontainer vorhanden ist.
Dies kann man mit `if constexpr`  und dem Test der Methode `sized_range` bewerkstelligen!

## *Views* werden *lazy* evaluiert

Die gesamte Arbeit, die eine *View* verrichtet, basiert auf der *Lazy*-Vorgehensweise.
Dies erfolgt somit im Gehensatz zu allen Funktionen aus der `<algorithm>`-Headerdatei,
die ihre Arbeit *immediately*, also sofort ausführen, wenn sie aufgerufen werden.

Wenn wir *Views* als Bausteine und in Ketten verwenden,
profitieren wir von der *Lazy*-Auswertungsvorgehensweise, da wir unnötige Kopien vermeiden.

Wie sieht es aber mit *Views* aus, die einen *Eager*-Algorithmus benötigen würden?
Zum Beispiel `std::sort`? Die Antwort lautet: Dies geht nicht!

Wir müssen in diesem Fall die Ansicht vorher materialisieren, wenn wir sie sortieren möchten:

```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 4, 2, 7, 1, 2, 6, 1, 5 };
04:     print(vec);
05: 
06:     // filter range
07:     auto isOdd = [](auto i) { return i % 2 == 1; };
08:     auto odd_numbers = vec | std::views::filter(isOdd);
09: 
10:     // std::ranges::sort(odd_numbers); // doesn't compile !!!
11: 
12:     // materialize the view before sorting
13:     auto result = to_vector(odd_numbers);
14:     print(result);
15: 
16:     // sort range
17:     std::ranges::sort(result);
18:     print(result);
19: }
```

*Ausgabe*:

```
4 2 7 1 2 6 1 5
7 1 1 5
1 1 5 7
```

*Hinweis*:
Eines der Probleme in diesem Zusammenhang ist,
dass sich der Übersetzer über die Iteratortypen beschwert, die von einer bestimmten *View* bereitgestellt werden.
Der Sortieralgorithmus erfordert *Random-Access*-Iteratoren!
Dies muss aber nicht der Typ der Iteratoren sein, die eine *View* besitzt,
obwohl der zugrundeliegende Eingabecontainer beispielsweise ein `std::vector`-Objekt ist!

Manchmal lassen sich auch Workarounds finden, die ein Materialisiern der *View* umgehen, 
siehe das folgende Beispiel:


```cpp
01: void test()
02: {
03:     auto vec = std::vector{ 8, 6, 10, 9, 2, 1, 3, 7, 4, 5 };
04:     print(vec);
05: 
06:     // filter range
07:     auto firstHalf = vec | std::views::take(vec.size());
08:     std::ranges::sort(firstHalf);
09:     print(firstHalf);
10: 
11:     std::ranges::sort(firstHalf, std::greater<>{});
12:     print(firstHalf);
13: }
```

*Ausgabe*:

```
8 6 10 9 2 1 3 7 4 5
1 2 3 4 5 6 7 8 9 10
10 9 8 7 6 5 4 3 2 1
```

---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen zum großen Teil aus dem Buch

&ldquo;[C++ High Performance](https://www.amazon.de/High-Performance-Master-optimizing-functioning/dp/1839216549/)&rdquo;
von Björn Andrist und Viktor Sehr.

---

[Zurück](ReadmeEx.md)

---
