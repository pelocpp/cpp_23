# *Views* in Standard C++&ndash;20

[Zurück](ReadmeEx.md)

---

## Übersicht

Nicht alle *Ranges* und *Views* der &ldquo;range-v3&rdquo;-Bibliothek
von Eric Niebler haben Einzug in den C++&ndash;20 Standard gefunden.

Nachfolgende Tabelle beschreibt &ndash; Stand August 2021 &ndash; den Umfang der *Views*,
die der Visual C++ Compiler unterstützt:

| *View* | Beschreibung |
|:-- |:----|
| `std::views::all` | Ansicht, die alle Elemente eines Bereichs enthält. |
| `std::views::common` | Umwandlung einer Ansicht in einen `common_range` Objekt. |
| `std::views::counted` | Erstellt einen Unterbereich aus einem Iterator und einer vorgegeben Anzahl von Elementen. |
| `std::views::drop` | Ansicht, die aus Elementen einer anderen Ansicht besteht, wobei die ersten *N* Elemente übersprungen werden. |
| `std::views::drop_while` | Ansicht, die aus den Elementen einer anderen Ansicht besteht, wobei die anfängliche Unterfolge von Elementen bis zum ersten Element übersprungen wird, bei dem das Prädikat zurückkehrt. |
| `std::views::elements` | Nimmt eine Ansicht, die aus tupelartigen Werten und einer Zahl *N* besteht, und erzeugt eine Ansicht des *N*.-ten Elements jedes Tupels. |
| `std::views::empty` | Leere Ansicht ohne Elemente. |
| `std::views::filter` | Eine Ansicht, die aus den Elementen eines Bereichs besteht, die ein bestimmtes Prädikat erfüllen. |
| `std::views::iota` | Erzeugt eine Ansicht, die aus einer Sequenz besteht, die durch wiederholtes Inkrementieren eines Anfangswerts erzeugt wird. |
| `std::views::join` | Ansicht, die aus der Sequenz besteht, die durch das Verknüpfen der Elemente mehrerer Ansichten von Bereichen entsteht. |
| `std::views::keys` | Nimmt eine Ansicht, die aus paarartigen Werten besteht, und erzeugt eine Ansicht der ersten Elemente jedes Paares. |
| `std::views::reverse` | Ansicht, die in umgekehrter Reihenfolge über die Elemente einer anderen Ansicht iteriert. |
| `std::views::single` | Ansicht, die ein einzelnes Element eines bestimmten Werts enthält. |
| `std::views::split` | Ansicht aus Teilbereichen, die durch das Aufteilen einer anderen Ansicht mit einem Trenner entsteht. |
| `std::views::take` | Ansicht bestehend aus den ersten *N* Elementen einer anderen Ansicht. |
| `std::views::take_while` | Ansicht, die aus den Anfangselementen einer anderen Ansicht besteht, bis zum ersten Element, für das ein Prädikat `false` zurückgibt. |
| `std::views::transform` | Ansicht, die auf jedes Element des Bereichs eine Transformationsfunktion anwendet. |
| `std::views::values` | Nimmt eine Ansicht, die aus paarartigen Werten besteht, und erzeugt eine Ansicht der zweiten Elemente jedes Paares. |

Tabelle 1. *Views* in Standard C++&ndash;20.

---

[Zurück](ReadmeEx.md)

---
