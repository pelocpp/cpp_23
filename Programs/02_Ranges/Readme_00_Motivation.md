# Motivation

[Zurück](Readme.md)

---

[Quellcode](Ranges_00_Motivation.cpp)

---

## Qualität von Fehlermeldungen

Mit Einführung der &ldquo;*Ranges*&rdquo;-Bibliothek hat sich die Qualität der Fehlermeldungen
verbessert, da *Ranges* auf *Concepts* beruhen.

Wir demonstrieren dies an einem &ldquo;Klassiker&rdquo; einer C++&ndash;Fehlermeldung,
nämlich dem Versuch, eine Liste (`std::list`) zu sortieren (`std::sort`):

###### Erstes Beispiel &ndash; C++ 17:

```cpp
std::list<int> numbers = { 1, 4, 2, 3 };

std::sort(
    std::begin(numbers),
    std::end(numbers)
);
```

Hier nun einige Kostproben der Fehlermeldungen (Visual C++):

  * `Error binary '-': 'const std::_List_unchecked_iterator<std::_List_val<std::_List_simple_types<_Ty>>>' does not define this operator or a conversion to a type acceptable to the predefined operator	Ranges`
  * `'_Sort_unchecked': no matching overloaded function found`

Das ist alles sehr schwer zu verstehen. Nun zu C++ 20:

###### Zweites Beispiel &ndash; C++ 20:

```cpp
std::list<int> numbers = { 1, 4, 2, 3 };
std::ranges::sort(numbers);
```

Logisch, das Programm ist nach wie vor falsch, aber die folgende Fehlermeldung
ist doch etwas zielführender:

  * `Error 'std::ranges::_Sort_fn::operator ()': the associated constraints are not satisfied` (Visual C++)
  * `note: constraints not satisfied` (gcc)
  * `note: 'std::random_access_iterator_tag' is not a base of 'std::bidirectional_iterator_tag'` (gcc)


## Lesbarkeit des Quellcodes

###### Betrachten Sie folgendes Beispiel:

```cpp
std::vector<int> vec1 { 1, 2, 3, 4, 5};
std::vector<int> vec2;

std::copy(
    std::begin(vec1),
    std::end(vec1), 
    std::back_inserter(vec2)
);
```

  * Der erste Vektor `vec1` muss bei den Parametern von `std::copy` *zweimal* aufgeführt werden.
  * Für das Zielobjekt (`vec2`) darf man nicht vergessen, den `std::back_inserter`-Iterator zu verwenden.


###### Eine erheblich kürzere Variante

```cpp
std::vector<int> vec1 { 1, 2, 3, 4, 5};
std::vector<int> vec2;

vec2 = vec1;
```

*Nachteile*:

  * Man kann nicht Teilbereiche des ersten Vektors auf diese Weise kopieren.
  * Man kann beispielsweise nicht von einem `std::list`- in ein `std::vector`-Objekt umkopieren.


###### Variante mit *Ranges*

```cpp
std::ranges::copy(vec1, std::back_inserter(vec2));
```

  * Der erste Vektor `vec1` ist bei den Parametern von `std::copy` nur einmal aufgeführt.
  * Allerdings muss der gesamte Container kopiert werden, ein Teilbereich geht nicht.


## Komposition von Funktionen

In der funktionalen Programmierung gibt es die so genannte &ldquo;Komposition von Funktionen&rdquo;:

  * Unter der &ldquo;Komposition von Funktionen&rdquo; versteht man in der funktionalen Programmierung
    die Möglichkeit, einfache &ndash; oder wie es im Fachjargon heißt: *pure* &ndash; Funktionen zu kombinieren,
    um kompliziertere Funktionen zu bilden.

  * Wie bei der üblichen Zusammensetzung von Funktionen in der Mathematik wird das Ergebnis einer Funktion als Argument der nächsten übergeben,
    und das Ergebnis der letzten ist das Ergebnis des Ganzen.

In C++ mit Berücksichtigung der STL ist es zwar möglich, stilistisch
zu einem gewisssen Grad in die Richtung &rdquo;funktionaler Programmierung&rdquo; vorzudringen,
zum Beispiel mit den STL-Algorithmen `std::copy_if`, `std::accumulate` und `std::transform`,
spätestens bei der Komposition von Funktionen sind diese Ansätze dann aber zum scheitern verurteilt.

Dies sei an einem Beispiel verdeutlicht: 

```cpp
01: std::vector<int> numbers{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
02: 
03: std::vector<int> evenNumbers{};
04: 
05: std::copy_if(
06:     std::begin(numbers),
07:     std::end(numbers),
08:     std::back_inserter(evenNumbers),
09:     [](int n) -> bool { return n % 2 == 0; }
10: );
11: 
12: std::vector<int> squares{};
13: 
14: std::transform(
15:     std::begin(evenNumbers),
16:     std::end(evenNumbers),
17:     std::back_inserter(squares),
18:     [](int n) -> int { return n * n; }
19: );
20: 
21: for (int n : squares) { 
22:     std::cout << n << ' ';
23: }
```

Was immer diese Folge von Anweisungen auch tut
(Ermittlung einer Reihe von geraden Zahlen mit anschließender Quadrierung),
mit der Bibliothek `<ranges>` lässt sich das erheblich eleganter formulieren:

```cpp
01: std::vector<int> numbers{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };
02: 
03: auto results = numbers
04:     | std::views::filter([](int n) -> bool { return n % 2 == 0; })
05:     | std::views::transform([](int n) { return n * n; });
06: 
07: for (int n : results) {
08:     std::cout << n << ' ';
09: }
```

Diesen Ausdruck muss man von links nach rechts lesen.
Das Pipe-Symbol `|` steht für die Verknüpfung von Funktionen:
Zuerst werden alle Elemente akzeptiert, die gerade sind (`std::views::filter([](int n) { return n % 2 == 0; })`).
Danach wird jedes verbleibende Element quadriert
(`std::views::transform([](int n) { return n * n; })`).
Dieses kleine Beispiel demonstriert zum einen die Funktionskomposition,
zum anderen agiert diese Funktionskomposition direkt auf dem Container.
Es sind also nicht, wie im klassischen Beispiel zuvor, temporäre Zwischencontainer notwendig.


## Projektionen

C++ 20 Projektionen sind ein neues Sprachmittel, die dann ins Spiel kommen,
wenn wir zum Beispiel einen Container nach einer bestimmten Eigenschaft seiner Elemente sortieren wollen.
Zum Beispiel ein `std::vector`-Objekt von Büchern nach dem Titel oder dem Preis.

In einer klassischen Schreibweise kommt uns hier der `std::sort`-Algorithmus
mit einer Lambda-Funktion als dritter Parameter entgegen:

```cpp
01: struct Book {
02:     std::string m_title;
03:     std::string m_author;
04:     int m_year;
05:     double m_price;
06: };
07: 
08: std::vector<Book> books {
09:     { "C++", "Bjarne Stroustrup", 1985, 20.55 },
10:     { "C", "Dennis Ritchie", 1972, 11.99 } ,
11:     { "Java", "James Gosling", 1995, 19.99 },
12:     { "C#", "Anders Hejlsberg", 2000, 29.99 }
13: };
14: 
15: std::sort(
16:     std::begin(books),
17:     std::end(books),
18:     [](const Book& book1, const Book& book2) {
19:         return book1.m_price < book2.m_price;
20:     }
21: );
22: 
23: for (int index{ 1 }; const auto & [title, author, year, price] : books) {
24:     std::cout << index << ": [" << price << "] " << title << std::endl;
25:     index++;
26: }
```

In dieser Realisierung könnten folgende kleine Fehler unbeabsichtigt zum Problem werden,
wenn wir Zeile 19 näher betrachten:

<pre>
return book1.m_title < book2.m_author;
</pre>

Wenn die beiden Elemente von Klasse `Book` (hier: `m_title` und `m_author`) vom selben Typ sind,
ist das syntaktisch absolut korrekter Quellcode, der Compiler winkt hier freundlich durch.

Mit dem neuen Sprachfeature *Projektionen* lässt sich das Sortieren wie folgt lösen:

```cpp
01: struct Book {
02:     std::string m_title;
03:     std::string m_author;
04:     int m_year;
05:     double m_price;
06: };
07: 
08: std::vector<Book> books{
09:     { "C++", "Bjarne Stroustrup", 1985, 20.55 },
10:     { "C", "Dennis Ritchie", 1972, 11.99 } ,
11:     { "Java", "James Gosling", 1995, 19.99 },
12:     { "C#", "Anders Hejlsberg", 2000, 29.99 }
13: };
14: 
15: // sort books
16: std::ranges::sort(books, std::less{}, &Book::m_price);
17: 
18: for (int index{ 1 }; const auto & [title, author, year, price] : books) {
19:     std::cout << index << ": [" << price << "] " << title << std::endl;
20:     index++;
21: }
22: std::cout << std::endl;
23: 
24: // list all titles
25: for (const auto& title : books | std::views::transform(&Book::m_title))
26:     std::cout << title << std::endl;
```

Dieses Mal wird der Quellcode nicht unbedingt kürzer, dafür aber prägnanter.
Wir können mit *Projektionen* viel klarer zu verstehenden Code schreiben,
der weniger anfällig für unbeabsichtigte Fehler ist!

Wir geben einfach `std::ranges::less` (oder ein anderes Funktionsobjekt) und einen Pointer auf ein Data Member als Argument mit.
Unter der Haube kommt hier `std::invoke` zum Einsatz, dies nur der Vollständigkeit halber erwähnt.

---

[Zurück](Readme.md)

---
