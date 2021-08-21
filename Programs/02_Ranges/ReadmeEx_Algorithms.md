# Redesign der Standard Library Algorithmen

[Zurück](../../Readme.md)

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

  * alle klassischen iterator-basierte Algorithmen (`std::begin()`, `std::end()`) und
  * alle neuen,  `concept`-basierten Algorithmen (`std::ranges::range`)

verfügbar. Das `range`-Konzept ist so definiert:

```cpp
template< class T >
concept range = requires(T& t) {
  ranges::begin(t);
  ranges::end  (t);
};
```

Alle C++ &ndash; Programme, die STL-Algorithmen verwenden,
können nun in zwei Varianten programmiert werden, wie das folgende Beispiel zeigt:

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


### Iteration über einen Bereich

WEITER : Hier alle Überschriften auffürehn .......................


---

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
