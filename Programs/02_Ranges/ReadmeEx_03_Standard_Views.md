# *Ranges* und *Views* in Standard C++&ndash;20

[Zur�ck](Readme.md)

---

## �bersicht

Nicht alle *Ranges* und *Views* der &ldquo;range-v3&rdquo;-Bibliothek
von Eric Niebler haben Einzug in den C++&ndash;20 Standard gefunden.

##  *Ranges* in Standard C++&ndash;20


Nachfolgende Tabelle beschreibt &ndash; Stand August 2021 &ndash; den Umfang der *Ranges*,
die der Visual C++ Compiler unterst�tzt:

| *Range* | Beschreibung | Unterst�tzende Container |
|:-- |:----|:----|
| `std::ranges::input_range` | Kann &ndash; mindestens einmal &ndash; von Anfang bis zum Ende iterieren | `std::forward_list`<br/>`std::unordered_map`<br/>`std::unordered_multimap`<br/>`std::unordered_set`<br/>`std::unordered_multiset`<br/>`basic_istream_view` |
| `std::ranges::forward_range` | Kann &ndash; mehr als einmal &ndash; von Anfang bis Ende iterieren | `std::forward_list`<br/>`std::unordered_map`<br/>`std::unordered_multimap`<br/>`std::unordered_set`<br/>`std::unordered_multiset` |
| `std::ranges::bidirectional_range` | Kann &ndash; mehr als einmal &ndash; vorw�rts und r�ckw�rts iterieren  | `std::list`<br/>`std::map`<br/>`std::multimap`<br/>`std::multiset`<br/>`std::set` |
| `std::ranges::random_access_range` | Zugriff mit dem `[]`-Operator auf ein beliebiges Element m�glich  | `std::deque` |
| `std::ranges::contiguous_range` | Die Elemente sind im Speicher dicht aufeinanderfolgend abgelegt  | `std::array`<br/>`std::string`<br/>`std::vector` |

Tabelle 1. *Ranges* in Standard C++&ndash;20.

##  *Views* in Standard C++&ndash;20

Nachfolgende Tabelle beschreibt &ndash; Stand August 2021 &ndash; den Umfang der *Views*,
die der Visual C++ Compiler unterst�tzt:

| *View* | Beschreibung |
|:-- |:----|
| `std::views::all` | Ansicht, die alle Elemente eines Bereichs enth�lt. |
| `std::views::common` | Umwandlung einer Ansicht in einen `common_range` Objekt. |
| `std::views::counted` | Erstellt einen Unterbereich aus einem Iterator und einer vorgegeben Anzahl von Elementen. |
| `std::views::drop` | Ansicht, die aus Elementen einer anderen Ansicht besteht, wobei die ersten *N* Elemente �bersprungen werden. |
| `std::views::drop_while` | Ansicht, die aus den Elementen einer anderen Ansicht besteht, wobei die anf�ngliche Unterfolge von Elementen bis zum ersten Element �bersprungen wird, bei dem das Pr�dikat zur�ckkehrt. |
| `std::views::elements` | Nimmt eine Ansicht, die aus tupelartigen Werten und einer Zahl *N* besteht, und erzeugt eine Ansicht des *N*.-ten Elements jedes Tupels. |
| `std::views::empty` | Leere Ansicht ohne Elemente. |
| `std::views::filter` | Eine Ansicht, die aus den Elementen eines Bereichs besteht, die ein bestimmtes Pr�dikat erf�llen. |
| `std::views::iota` | Erzeugt eine Ansicht, die aus einer Sequenz besteht, die durch wiederholtes Inkrementieren eines Anfangswerts erzeugt wird. |
| `std::views::join` | Ansicht, die aus der Sequenz besteht, die durch das Verkn�pfen der Elemente mehrerer Ansichten von Bereichen entsteht. |
| `std::views::keys` | Nimmt eine Ansicht, die aus paarartigen Werten besteht, und erzeugt eine Ansicht der ersten Elemente jedes Paares. |
| `std::views::reverse` | Ansicht, die in umgekehrter Reihenfolge �ber die Elemente einer anderen Ansicht iteriert. |
| `std::views::single` | Ansicht, die ein einzelnes Element eines bestimmten Werts enth�lt. |
| `std::views::split` | Ansicht aus Teilbereichen, die durch das Aufteilen einer anderen Ansicht mit einem Trenner entsteht. |
| `std::views::take` | Ansicht bestehend aus den ersten *N* Elementen einer anderen Ansicht. |
| `std::views::take_while` | Ansicht, die aus den Anfangselementen einer anderen Ansicht besteht, bis zum ersten Element, f�r das ein Pr�dikat `false` zur�ckgibt. |
| `std::views::transform` | Ansicht, die auf jedes Element des Bereichs eine Transformationsfunktion anwendet. |
| `std::views::values` | Nimmt eine Ansicht, die aus paarartigen Werten besteht, und erzeugt eine Ansicht der zweiten Elemente jedes Paares. |

Tabelle 2. *Views* in Standard C++&ndash;20.

---

[Zur�ck](Readme.md)

---
