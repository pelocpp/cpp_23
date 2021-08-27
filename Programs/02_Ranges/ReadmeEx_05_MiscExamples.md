# Einige allgemeine Beispiele

[Zurück](Readme.md)

---

[Quellcode](Ranges_Ex_05_MiscExamples.cpp)

---

### Numerische Beispiele

##### Beispiel 1: Alle Elemente eines Bereichs iterieren / ausgeben

Alle folgenden Beispiele verwenden folgende Hilfsfunktionen:

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


---

## Literaturhinweise:

Die numerischen Beispiele sind zum großen Teil aus dem Blog von *Marius Bancila* entnommen &ndash; in leicht modifizierter Form:

&ldquo;[C++ code samples before and after Ranges](https://mariusbancila.ro/blog/2019/01/20/cpp-code-samples-before-and-after-ranges/)&rdquo;.

---

[Zurück](Readme.md)

---
