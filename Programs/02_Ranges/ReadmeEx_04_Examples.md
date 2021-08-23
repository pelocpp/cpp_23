# Beispiele

[Zurück](Readme.md)

---

[Quellcode](Ranges_Ex_04_Examples.cpp)

---

## Das Filter-Map-Reduce Pattern

&ldquo;Filter-Map-Reduce&ldquo; beschreibt ein Pattern, bei dem eine Menge von Daten in einer Abfolge
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

## Konvertierung Binär nach Dezimal

Die Konvertierung einer binären Zahl in ihre dezimale Darstellung 
kann man auch mit der &ldquo;*Ranges*&rdquo;-Bibliothek in C++ 20 lösen &ndash;
wenngleich der Lerneffekt hier im Vordergrund steht.

Wir legen zu Grunde, dass die Ziffern der Binärdarstellung in einem Bereich vorliegen.
Um die nachfolgenden Erläuterungen an einem Beispiel festzumachen, wollen wir die binäre Zahl `1110` betrachten:

  * Kehren Sie die anfängliche Binärdarstellung mithilfe von `std::views::reverse` um (im Beispiel erhalten Sie `[0, 1, 1, 1]`).
    Dadurch können wir mit der Potenz 2<sup>0</sup> beginnen.
  * Generieren Sie den Bereich `[0, 1, 2, 3]` mithilfe von `std::views::iota`.
  * Verwenden Sie `std::views::transform` und den Shift-Operator, um `[0, 1, 2, 3]` in
    <code>[2<sup>0</sup>, 2<sup>1</sup>, 2<sup>2</sup>, 2<sup>3</sup>]</code> umzuwandeln.
  * Verwenden Sie nun `std::views::inner_product`, um den umgekehrten Binärbereich `[0, 1, 1, 1]` mit
    den Zweierpotenzen `[1, 2, 4, 8]` zu multiplizieren und zusammenzufassen.

*Hinweis*: Auch in diesem Beispiel sind nicht alle *Views* im C++&ndash;Standard vorhanden.
Für die Ansicht `std::views::inner_product` gibt es alternativ den STL-Algorithmus `std::inner_product`!

---

## Literaturhinweise:

---

[Zurück](Readme.md)

---
