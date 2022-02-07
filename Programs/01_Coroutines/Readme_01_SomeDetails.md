# Einige technische Details

[Zur�ck](../../Readme.md)

---

[Quellcode](XXX.cpp)

---

## Coroutinen: *stackless* und *stackful*

Coroutinen existieren in den verschiedenene Programmierumgebungen prinzipiell
in zwei Auspr�gungen:

  * *stackless*
  * *stackful*

In C++ 20 haben wir eine Unterst�tzung f�r *stackless* Coroutinen.
Dies bedeutet in einfachen Worten formuliert, dass der Zustanf einer Coroutine &ndash;
auch als *Coroutine-Frame* bezeichnet  &ndash; auf dem Heap abgelegt wird.

## 3 neue Schl�sselw�rter

Eine C++&ndash;Funktion erlangt den Status einer Coroutine, wenn sie eines der
drei neuen C++&ndash;Schl�sselw�rter verwendet:

  * `co_return`
  * `co_yield`
  * `co_await`

## Der C++ *Coroutine Framework* 

### Ein *Generator*

Um im C++&ndash;Laufzeitsystem eine Coroutine zuverwalten, bedarf es eines so genannten
Generator-Objekts, das den Lebenszyklus einer Coroutine abbildet und verwaltet.

Die Definition dieser Klasse ist in der include-Datei vorhanden:

```cpp
#include <coroutine>
```

Jede C++&ndash;Anwenderklasse, die den Anspruch erhebt, eine *Generator*-Klasse zu sein,
muss einen Bezug zu einer vordefinerten Klasse eines bestimmten Namens aufweisen:

```cpp
promise_type
```

Dabei spielt es keine Rolle, ob diese Klasse intern in der Anwenderklasse definiert ist,
oder nur mittels einer using-Direktive ein Hinweis vorhanden ist,
wo der Compiler die Definition dieser Klasse findet.

Der Name `promise_type` weist die Qualit�t einer Schnittstelle auf:
Es m�ssen je nach Anwendungsfall eine Reihe von vordefinierten Methoden definiert sein.



---

## Literaturhinweise:

Anregungen zu den Beispielen stammen zum gro�en Teil aus dem Buch

&ldquo;[C++ High Performance](https://www.amazon.de/High-Performance-Master-optimizing-functioning/dp/1839216549/)&rdquo;
von Bj�rn Andrist und Viktor Sehr.

Das Beispiel zu Projektionen wurde

[C++20 Ranges, Projections, std::invoke and if constexpr](https://www.cppstories.com/2020/10/understanding-invoke.html/)

entnommen.

---

[Zur�ck](../../Readme.md)

---
