# *Ranges* und *Views* in Standard C++&ndash;20

[Zurück](Readme.md)

---

## Übersicht

Nicht alle *Ranges* und *Views* der &ldquo;range-v3&rdquo;-Bibliothek
von Eric Niebler haben Einzug in den C++&ndash;20 oder auch C++&ndash;23 Standard gefunden.

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

*Tabelle* 1. *Ranges* in Standard C++&ndash;20.

##  *Views* in Standard C++&ndash;20

Nachfolgende Tabelle beschreibt &ndash; Stand August 2021 &ndash; den Umfang der *Views*,
die der Visual C++ Compiler unterstützt:

| *View* | Beschreibung |
|:-- |:----|
| `std::ranges::views::all` | Ansicht, die alle Elemente eines Bereichs enthält. |
| `std::ranges::views::common`<br />`std::ranges::common_view` | Umwandlung einer Ansicht in ein `common_range`&ndash;Objekt. |
| `std::ranges::views::counted` | Erstellt einen Unterbereich aus einem Iterator und einer vorgegeben Anzahl von Elementen. |
| `std::ranges::views::drop`<br />`std::ranges::drop_view` | Ansicht, die aus Elementen einer anderen Ansicht besteht, wobei die ersten *N* Elemente übersprungen werden. |
| `std::ranges::views::drop_while`<br />`std::ranges::drop_while_view` | Ansicht, die aus den Elementen einer anderen Ansicht besteht, wobei die anfängliche Unterfolge von Elementen bis zum ersten Element übersprungen wird, bei dem das Prädikat zurückkehrt. |
| `std::ranges::views::elements`<br />`std::ranges::elements_view` | Nimmt eine Ansicht, die aus tupelartigen Werten und einer Zahl *N* besteht, und erzeugt eine Ansicht des *N*.-ten Elements jedes Tupels. |
| `std::ranges::views::empty` | Leere Ansicht ohne Elemente. |
| `std::ranges::views::filter`<br />`std::ranges::filter_view` | Ansicht, die aus den Elementen eines Bereichs besteht, die ein bestimmtes Prädikat erfüllen. |
| `std::ranges::views::iota` | Erzeugt eine Ansicht, die aus einer Sequenz besteht, die durch wiederholtes Inkrementieren eines Anfangswerts erzeugt wird. |
| `std::ranges::views::join`<br />`std::ranges::join_view` | Ansicht, die aus der Sequenz besteht, die durch das Verknüpfen der Elemente mehrerer Ansichten von Bereichen entsteht. |
| `std::ranges::views::keys`<br />`std::ranges::keys_view` | Nimmt eine Ansicht, die aus paarartigen Werten besteht, und erzeugt eine Ansicht der ersten Elemente jedes Paares. |
| `std::ranges::views::reverse`<br />`std::ranges::reverse_view` | Ansicht, die in umgekehrter Reihenfolge über die Elemente einer anderen Ansicht iteriert. |
| `std::ranges::views::single` | Ansicht, die ein einzelnes Element eines bestimmten Werts enthält. |
| `std::ranges::views::split` | Ansicht aus Teilbereichen, die durch das Aufteilen einer anderen Ansicht mit einem Trenner entsteht. |
| `std::ranges::views::take`<br />`std::ranges::take_view` | Ansicht bestehend aus den ersten *N* Elementen einer anderen Ansicht. |
| `std::ranges::views::take_while`<br />`std::ranges::take_while_view` | Ansicht, die aus den Anfangselementen einer anderen Ansicht besteht, bis zum ersten Element, für das ein Prädikat `false` zurückgibt. |
| `std::ranges::views::transform`<br />`std::ranges::transform_view` | Ansicht, die auf jedes Element des Bereichs eine Transformationsfunktion anwendet. |
| `std::ranges::views::values`<br />`std::ranges::values_view` | Nimmt eine Ansicht, die aus paarartigen Werten besteht, und erzeugt eine Ansicht der zweiten Elemente jedes Paares. |

*Tabelle* 2. *Views* in Standard C++&ndash;20.


In C++&ndash;23 sind weitere *Views* hinzugekommen:

| *View* | Beschreibung |
|:-- |:----|
| `std::ranges::views::as_const`<br />`std::ranges::as_const_view` | Erstellt eine Ansicht, über die die Elemente einer zugrunde liegenden Sequenz nicht geändert werden können. |
| `std::ranges::views::as_rvalue`<br />`std::ranges::as_rvalue_view` | Erstellt eine Ansicht von RValues aller Elemente einer zugrunde liegende Sequenz. |
| `std::ranges::views::enumerate`<br />`std::ranges::enumerate_view` | Erstellt eine Ansicht, in der jedes Element die Position und den Wert aller Elemente einer zugrunde liegenden Sequenz darstellt. |
| `std::ranges::views::zip`<br />`std::ranges::zip_view` | Erstellt eine Ansicht, die aus Tupeln mit Verweisen auf entsprechende Elemente aller angegebenen Ansichten besteht. |
| `std::ranges::views::zip_transform`<br />`std::ranges::zip_transform_view` | Erstellt eine Ansicht, deren *i*-tes Element das Ergebnis der Anwendung eines bestimmten Callables auf die *i*-ten Elemente aller angegebenen Ansichten ist. |
| `std::ranges::views::adjacent`<br />`std::ranges::adjacent_view` | Erstellt für ein gegebenes *n* eine Ansicht, deren *i*-tes Element ein Tupel von Verweisen auf das *i*-te bis (*i* + *n* -  1)-te Element einer bestimmten Ansicht ist. |

*Tabelle* 3. Zusätzliche *Views* in Standard C++&ndash;23.

**Bemerkung*:<br />
Die vorgestellten Tabellen sollen nur einen Eindruck von den aktuell in Standard C++ verfügbaren *Views* zeigen.
Wollen Sie hierzu genaue Informationen haben, so verweise ich auf die aktuellen Dokumentationen im
[Netz](https://en.cppreference.com/w/cpp/ranges/adjacent_view)!

---

[Zurück](Readme.md)

---
