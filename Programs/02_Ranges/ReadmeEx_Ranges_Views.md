# Ranges und Views / Bereiche und Sichten

[Zurück](../../Readme.md)

---

[Quellcode: Ranges01.cpp](Ranges01.cpp)<br/>
[Quellcode: Ranges02.cpp](Ranges02.cpp)

---

## Einleitung


###### C++&ndash;17  Variante

```cpp
auto values = std::vector{ 9, 2, 5, 3, 4 };
std::sort(std::begin(values), std::end(values));
print(values);
```

###### C++&ndash;20 &ndash; Variante

```cpp
auto values = std::vector{ 9, 2, 5, 3, 4 };
std::ranges::sort(values);
print(values);
```

Wir geben nun einen Überblick für die häufigsten Anwendungsfälle für STL-Algorithmen &ndash;
formuliert in der klassischen und in der bereichs-basierten Notation:




## Literaturhinweise:

Die Anregungen zu den Beispielen stammen aus

[A beginner's guide to C++ Ranges and Views](https://hannes.hauswedell.net/post/2019/11/30/range_intro/)

von Hannes Hauswedell.

Viele Beispiele zum Vergleichen von Code-Fragmenten ohne und mit *Ranges*-Features finden Sie in 

[C++ code samples before and after Ranges](https://mariusbancila.ro/blog/2019/01/20/cpp-code-samples-before-and-after-ranges/)

vor.

---

[Zurück](../../Readme.md)

---
