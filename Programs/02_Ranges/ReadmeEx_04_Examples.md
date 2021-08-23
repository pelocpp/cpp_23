# Beispiele

[Zurück](Readme.md)

---

[Quellcode](Ranges_Ex_03_Examples.cpp)

---

## Das Filter-Map-Reduce Pattern

Das &ldquo;Filter-Map-Reduce&ldquo; beschreibt ein Pattern, bei dem eine Menge von Daten in einer Abfolge
von bestimmten Schritten verarbeitet wird. Dabei ist `reduce` eine andere Bezeichnung für `fold`.

Offensichtlich ist die Reihenfolge dabei wichtig:

  * *Filter*: Zunächst findet eine Filterung der Elemente aus einer bestimmten Menge von Elementen statt.
  * *Map*: Danach erfolgt eine Transformation der noch vorhandenen Elemente in einen neuen Datentyp.
  * *Reduce*: Schließlich wird das Zwischenergebnis, eine modifizierte Teilmenge der Ausgangsmenge, auf ein Endergebnis reduziert.

Wir betrachten das Muster am Beispiel einer Buchhandlung.
Dabei soll ein Buch aus folgenden Informationen bestehen:

```cpp
struct Book {
    std::string m_title;
    std::string m_author;
    int m_year;
    double m_price;
};
```

Legen Sie eine Reihe von Büchern in einem STL-Container Ihrer Wahl ab.
Es wäre von Vorteil, wenn einige Bücher vor 1990 und einige andere danach erschienen sind.
Setzen Sie das Pattern an Hand folgender Fragestellung um:

Wie lauten die Titel aller Bücher, die nach 1990 erschienen sind?

*Bemerkung*: Wenn Sie alle im C++&ndash;20 Standard verfügbaren *Views* studieren, werden Sie die Beobachtung machen,
dass eine *fold*-Ansicht nicht vorhanden ist. Sie haben hier keine andere Wahl, als zum STL-Algorithmus
`std::accumulate` zu wechseln und damit den Themenkreis der &ldquo;*Ranges*&rdquo;-Bibliothek zu verlassen!  

*Beispiel*:

```cpp
// testing 'Filter-Map-Reduce' Pattern
struct Book {
    std::string m_title;
    std::string m_author;
    int m_year;
    double m_price;
};

std::list<Book> books {
        {"C", "Dennis Ritchie", 1972, 11.99 } ,
        {"Java", "James Gosling", 1995, 19.99 },
        {"C++", "Bjarne Stroustrup", 1985, 20.00 },
        {"C#", "Anders Hejlsberg", 2000, 29.99 }
};

// your implementation ...

std::cout << result << std::endl;
```

*Ausgabe*:

```
Java, C#
```

---

## Literaturhinweise:

---

[Zurück](Readme.md)

---
