# *Ranges* und *Views* in Standard C++&ndash;20

[Zurück](Readme.md)

---

## Übersicht

Nicht alle *Ranges* und *Views* der &ldquo;range-v3&rdquo;-Bibliothek
von Eric Niebler haben Einzug in den C++&ndash;20 Standard gefunden.

##  *Ranges* in Standard C++&ndash;20


Nachfolgende Tabelle beschreibt &ndash; Stand August 2021 &ndash; den Umfang der *Ranges*,
die der Visual C++ Compiler unterstützt:

| *Range* | Beschreibung | Unterstützende Container |
|:-- |:----|:----|
| `std::ranges::input_range` | Kann &ndash; mindestens einmal &ndash; von Anfang bis zum Ende iterieren | `std::forward_list`<br/>`std::unordered_map`<br/>`std::unordered_multimap`<br/>`std::unordered_set`<br/>`std::unordered_multiset`<br/>`basic_istream_view` |
| `std::ranges::forward_range` | Kann &ndash; mehr als einmal &ndash; von Anfang bis Ende iterieren | `std::forward_list`<br/>`std::unordered_map`<br/>`std::unordered_multimap`<br/>`std::unordered_set`<br/>`std::unordered_multiset` |
| `std::ranges::bidirectional_range` | Kann &ndash; mehr als einmal &ndash; vorwärts und rückwärts iterieren  | `std::list`<br/>`std::map`<br/>`std::multimap`<br/>`std::multiset`<br/>`std::set` |
| `std::ranges::random_access_range` | Zugriff mit dem `[]`-Operator auf ein beliebiges Element möglich  | `std::deque` |
| `std::ranges::contiguous_range` | Die Elemente sind im Speicher dicht aufeinanderfolgend abgelegt  | `std::array`<br/>`std::string`<br/>`std::vector` |

Tabelle 1. *Ranges* in Standard C++&ndash;20.

##  *Views* in Standard C++&ndash;20

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

Tabelle 2. *Views* in Standard C++&ndash;20.

---

[Zurück](Readme.md)

---
