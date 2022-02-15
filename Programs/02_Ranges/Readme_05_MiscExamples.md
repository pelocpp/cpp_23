# Einige allgemeine Beispiele

[Zurück](Readme.md)

---

[Quellcode](Ranges_05_MiscExamples.cpp)

---

### Numerische Beispiele

##### Beispiel 1: Alle Elemente eines Bereichs iterieren / ausgeben

Die folgenden Beispiele verwenden folgende Hilfsfunktionen:

```cpp
auto printElem = [](auto n) { std::cout << n << ' '; };
auto printElemNL = [](auto n) { std::cout << n << std::endl; };
```

Damit zum ersten Beispiel:

```cpp
01: auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: std::ranges::for_each(vec, printElem);
```

*Ausgabe*:

```
1 2 3 4 5 6 7 8 9 10
```

##### Beispiel 2: Alle Elemente eines Bereichs in umgekehrter Reihenfolge ausgeben

```cpp
01: auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: auto range = vec | std::views::reverse;
03: std::ranges::for_each(range, printElem);
```

*Ausgabe*:

```
10 9 8 7 6 5 4 3 2 1
```

##### Beispiel 3: Nur die geraden Elemente eines Bereichs ausgeben

```cpp
01: auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: auto range = vec | std::views::filter([](auto i) { return i % 2 == 0; });
03: std::ranges::for_each(range, printElem);
```

*Ausgabe*:

```
2 4 6 8 10
```

##### Beispiel 4: Nur die geraden Elemente eines Bereichs ausgeben, jedoch in umgekehrter Reihenfolge

```cpp
01: auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: 
03: auto range = vec
04:     | std::views::filter([](auto i) { return i % 2 == 0; })
05:     | std::views::reverse;
06: 
07: std::ranges::for_each(range, printElem);
```

*Ausgabe*:

```
10 8 6 4 2
```

##### Beispiel 5: Die ersten beiden Elemente eines Bereichs überspringen, die nächsten drei geraden Zahlen des Bereichs ausgeben

```cpp
01: auto vec = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
02: 
03: auto range = vec
04:     | std::views::drop(2)
05:     | std::views::take(4)
06:     | std::views::filter([](auto i) { return i % 2 == 0; });
07: 
08: std::ranges::for_each(range, printElem);
```

*Ausgabe*:

```
4 6
```

##### Beispiel 6: Alle Zahlen von 1 bis 20 ausgeben

```cpp
01: auto range = std::views::iota(1, 21);
02: std::ranges::for_each(range, printElem);
```

*Ausgabe*:

```
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
```

##### Beispiel 7: Alle römischen Ziffern von 1 bis 50 ausgeben

```cpp
01: std::string toRoman(int value)
02: {
03:     std::vector<std::pair<int, std::string_view>> romans
04:     {
05:         { 1000, "M" },
06:         { 900, "CM" },
07:         { 500, "D" },
08:         { 400, "CD" },
09:         { 100, "C" },
10:         { 90, "XC" },
11:         { 50, "L" },
12:         { 40, "XL" },
13:         { 10, "X" },
14:         { 9, "IX" },
15:         { 5, "V" },
16:         { 4, "IV" },
17:         { 1, "I" }
18:     };
19: 
20:     std::string result{};
21: 
22:     for (auto const& [decimal, roman] : romans)
23:     {
24:         while (value >= decimal)
25:         {
26:             result += roman;
27:             value -= decimal;
28:         }
29:     }
30: 
31:     return result;
32: }
33: 
34: void example()
35: {
36:     auto range = std::views::iota(1, 51)
37:         | std::views::transform([](auto i) { return toRoman(i); });
38: 
39:     std::ranges::for_each(range, printElemNL);
40: }
```

*Ausgabe*:

```
I
II
III
IV
V
VI
VII
VIII
IX
X
XI
XII
XIII
XIV
XV
XVI
XVII
XVIII
XIX
XX
XXI
XXII
XXIII
XXIV
XXV
XXVI
XXVII
XXVIII
XXIX
XXX
XXXI
XXXII
XXXIII
XXXIV
XXXV
XXXVI
XXXVII
XXXVIII
XXXIX
XL
XLI
XLII
XLIII
XLIV
XLV
XLVI
XLVII
XLVIII
XLIX
L
```

##### Beispiel 8: Ausgabe der letzten drei durch 7 teilbaren Zahlen im Bereich [1, 50] als römische Ziffern in umgekehrter Reihenfolge ausgeben

```cpp
01: auto range = std::views::iota(1, 22)
02:     | std::views::filter([](auto i) { return i % 7 == 0; })
03:     | std::views::transform([](auto i) { return toRoman(i); })
04:     | std::views::reverse
05:     | std::views::take(3);
06: 
07: std::ranges::for_each(range, printElemNL);
```

*Ausgabe*:

```
XXI
XIV
VII
```

### Beispiele mit Zeichenketten

##### Beispiel 1: Anzahl der Teilzeichenketten (Wörter) in einer Zeichenkette bestimmen

```cpp
01: std::string text { "The quick brown fox jumps over the lazy dog" };
02: auto words{ text | std::views::split(' ') };
03: size_t num{ sizeOfRange(words) };
04: std::cout << num << " words.";

```

*Ausgabe*:

```
9 words.
```

##### Beispiel 2: Eine Zeichenkette in Teilzeichenketten zerlegen

Bei diesem Beispiel sollten die Teilzeichenketten durch ein beliebiges Trennzeichen voneinander getrennt sein.
Das Resultat im Form eines Bereichs kann man entweder traversieren oder aber die Menge der Teilzeichenketten 
wird in einem `std::vector<std::string>-Objekt` zusammengefasst.

Es werden gleich drei Lösungsansätze vorgestellt.
Allen Vorschlägen ist die Beobachtung gemeinsam, dass die View `std::views::split` einen *Subrange*-Typ zurückliefert,
also keine Teilzeichenkette und damit kein Objekt vom Typ `std::string`.

```cpp
01: auto toVector(auto&& r)
02: {
03:     std::vector<std::ranges::range_value_t<decltype(r)>> vec;
04: 
05:     if constexpr (std::ranges::sized_range<decltype(r)>) {
06:         vec.reserve(std::ranges::size(r));
07:     }
08:     else {
09:         vec.reserve(std::distance(r.begin(), r.end()));
10:     }
11: 
12:     std::ranges::copy(r, std::back_inserter(vec));
13:     return vec;
14: }
15: 
16: void example_strings_02()
17: {
18:     std::string text{ "The-quick-brown-fox-jumps-over-the-lazy-dog" };
19: 
20:     auto range = text | std::views::split('-') | std::views::transform([](auto&& s) {
21:         auto subrange{ s | std::views::common };
22:         std::string word{ subrange.begin(), subrange.end() };
23:         return word;
24:     });
25: 
26:     auto words{ toVector(range) };
27: 
28:     for (auto&& word : words) {
29:         std::cout << word << "--";
30:     }
31: }
32: 
33: void example_strings_02a()
34: {
35:     std::string text{ "The-quick-brown-fox-jumps-over-the-lazy-dog" };
36: 
37:     auto range = text | std::views::split('-') | std::views::transform([](auto&& s) {
38:         auto subrange{ s | std::views::common };
39:         std::string word{ subrange.begin(), subrange.end() };
40:         return word;
41:     });
42: 
43:     for (auto&& word : range) {
44:         std::cout << word << "!!";
45:     }
46: }
47: 
48: auto toString(auto&& r)
49: {
50:     std::string result{};
51:     if constexpr (std::ranges::sized_range<decltype(r)>) {
52:         result.reserve(std::ranges::size(r));
53:     }
54:     else {
55:         result.reserve(std::distance(r.begin(), r.end()));
56:     }
57: 
58:     std::ranges::copy(r, std::back_inserter(result));
59:     return result;
60: }
61: 
62: void example_strings_02b()
63: {
64:     std::string text { "The-quick-brown-fox-jumps-over-the-lazy-dog" };
65: 
66:     auto range = text | std::views::split('-') | std::views::transform([](auto&& s) {
67:         auto subrange{ s | std::views::common };
68:         std::string word{ toString (subrange)};
69:         return word;
70:     });
71: 
72:     for (auto&& word : range) {
73:         std::cout << word << "??";
74:     }
75: }
```

*Ausgabe*:

```
The--quick--brown--fox--jumps--over--the--lazy--dog--
The!!quick!!brown!!fox!!jumps!!over!!the!!lazy!!dog!!
The??quick??brown??fox??jumps??over??the??lazy??dog??
```

##### Beispiel 3: Teilzeichenketten zu einer ganzen Zeichenkette zusammenfügen

Natürlich gibt es hierfür mehrere Lösungsmöglichkeiten. Wir stellen eine Lösung
ohne die &ldquo;*Ranges*&rdquo;-Bibliothek vor (sie basiert auf dem STL-Algorithmus `std::accumulate`)
und eine zweite mit der *View* `std::views::join`:

```cpp
01: void example_strings_03()
02: {
03:     std::vector<std::string> words{
04:         "Lorem", ".", "ipsum", ".",
05:         "dolor", ".", "sit", ".",
06:         "amet"
07:     };
08: 
09:     // okay, solution without ranges :(
10:     std::string text{ std::accumulate(std::begin(words), std::end(words), std::string{}) };
11:     std::cout << text;
12: }
13: 
14: void example_strings_03a()
15: {
16:     std::vector<std::string> words{
17:         "Lorem", "-", "ipsum", "-",
18:         "dolor", "-", "sit", "-",
19:         "amet"
20:     };
21: 
22:     // this solution is with ranges :)
23:     auto range{ words | std::views::join };
24:     std::string text{ toString(range) };
25:     std::cout << text;
26: }
```

*Ausgabe*:

```
Lorem.ipsum.dolor.sit.amet
Lorem-ipsum-dolor-sit-amet
```

##### Beispiel 4: Eine Zeichenkettenzerlegung mit Trennzeichenkette

```cpp
01: std::string words{ "Modern-_-C++-_-is-_-really-_-awesome-_-!" };
02: std::string delim{ "-_-" };
03: 
04: auto range = words | std::views::split(delim) | std::views::transform([](auto&& s) {
05:     auto subrange{ s | std::views::common };
06:     std::string word{ subrange.begin(), subrange.end() };
07:     return word;
08: });
09: 
10: for (std::string word : range) {
11:     std::cout << std::quoted(word) << std::endl;
12: }
```

*Ausgabe*:

```
"Modern"
"C++"
"is"
"really"
"awesome"
"!"
```


---

## Literaturhinweise:

Die numerischen Beispiele sind zum großen Teil aus dem Blog von *Marius Bancila* entnommen &ndash; in leicht modifizierter Form:

&ldquo;[C++ code samples before and after Ranges](https://mariusbancila.ro/blog/2019/01/20/cpp-code-samples-before-and-after-ranges/)&rdquo;.

---

[Zurück](Readme.md)

---
