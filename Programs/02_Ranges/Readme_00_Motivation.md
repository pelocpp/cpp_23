# Motivation

[Zur�ck](Readme.md)

---

[Quellcode](Ranges_00_Motivation.cpp)

---

## Qualit�t von Fehlermeldungen

Mit Einf�hrung der &ldquo;*Ranges*&rdquo;-Bibliothek hat sich die Qualit�t der Fehlermeldungen
verbessert, da *Ranges* auf *Concepts* beruhen.

Wir demonstrieren dies an einem &ldquo;Klassiker&rdquo; einer C++&ndash;Fehlermeldung,
n�mlich dem Versuch, eine Liste (`std::list`) zu sortieren (`std::sort`):

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
ist doch etwas zielf�hrender:

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

  * Der erste Vektor `vec1` muss bei den Parametern von `std::copy` *zweimal* aufgef�hrt werden.
  * F�r das Zielobjekt (`vec2`) darf man nicht vergessen, den `std::back_inserter`-Iteratoradapter zu verwenden.


###### Eine erheblich k�rzere Variante

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

  * Der erste Vektor `vec1` ist bei den Parametern von `std::copy` nur einmal aufgef�hrt.
  * Allerdings muss der gesamte Container kopiert werden, ein Teilbereich geht nicht.


## Komposition von Funktionen

In der funktionalen Programmierung gibt es die so genannte &ldquo;Komposition von Funktionen&rdquo;:

  * Unter der &ldquo;Komposition von Funktionen&rdquo; versteht man in der funktionalen Programmierung
    die M�glichkeit, einfache &ndash; oder wie es im Fachjargon hei�t: *pure* &ndash; Funktionen zu kombinieren,
    um kompliziertere Funktionen zu bilden.

  * Wie bei der �blichen Zusammensetzung von Funktionen in der Mathematik wird das Ergebnis einer Funktion als Argument der n�chsten �bergeben,
    und das Ergebnis der letzten ist das Ergebnis des Ganzen.

In C++ mit Ber�cksichtigung der STL ist es zwar m�glich, stilistisch
zu einem gewisssen Grad in die Richtung &rdquo;funktionaler Programmierung&rdquo; vorzudringen,
zum Beispiel mit den STL-Algorithmen `std::copy_if`, `std::accumulate` und `std::transform`,
sp�testens bei der Komposition von Funktionen sind diese Ans�tze dann aber zum scheitern verurteilt.

Dies sei an einem Beispiel verdeutlicht: 

```cpp
01: std::vector<int> numbers{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
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
22:     std::print("{} ", n);
23: }
24: std::println("");
```

Was immer diese Folge von Anweisungen auch tut
(Ermittlung einer Reihe von geraden Zahlen mit anschlie�ender Quadrierung),
mit der Bibliothek `<ranges>` l�sst sich das erheblich eleganter formulieren:

```cpp
std::vector<int> numbers{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

auto results = numbers
    | std::ranges::views::filter([](int n) -> bool { return n % 2 == 0; })
    | std::ranges::views::transform([](int n) { return n * n; });

for (int n : results) {
    std::print("{} ", n);
}
```

Diesen Ausdruck muss man von links nach rechts lesen.
Das Pipe-Symbol `|` steht f�r die Verkn�pfung von Funktionen:
Zuerst werden alle Elemente akzeptiert, die gerade sind (`std::ranges::views::filter([](int n) { return n % 2 == 0; })`).
Danach wird jedes verbleibende Element quadriert
(`std::ranges::views::transform([](int n) { return n * n; })`).
Dieses kleine Beispiel demonstriert zum einen die Funktionskomposition,
zum anderen agiert diese Funktionskomposition direkt auf dem Container.
Es sind also nicht, wie im klassischen Beispiel zuvor, tempor�re Zwischencontainer notwendig.


## Projektionen

C++ 20 Projektionen sind ein neues Sprachmittel, die dann ins Spiel kommen,
wenn wir zum Beispiel einen Container nach einer bestimmten Eigenschaft seiner Elemente sortieren wollen.
Zum Beispiel ein `std::vector<Book>`-Objekt von B�chern nach dem Titel oder dem Preis.

In einer klassischen Schreibweise kommt uns hier der `std::sort`-Algorithmus
mit einer Lambda-Funktion als dritter Parameter entgegen:

```cpp
01: std::vector<Book> books 
02: {
03:     { "C++", "Bjarne Stroustrup", 1985, 20.55 },
04:     { "C", "Dennis Ritchie", 1972, 11.99 } ,
05:     { "Java", "James Gosling", 1995, 19.99 },
06:     { "C#", "Anders Hejlsberg", 2000, 29.99 }
07: };
08: 
09: std::sort(
10:     std::begin(books),
11:     std::end(books),
12:     [](const Book& book1, const Book& book2) {
13:         return book1.m_title < book2.m_author;
14:     }
15: );
16: 
17: for (int index{ 1 }; const auto& [title, author, year, price] : books) {
18:     std::println("{}: [{}] ", price, title);
19:     index++;
20: }
21: std::println("");
```

In dieser Realisierung k�nnten folgende kleine Fehler unbeabsichtigt zum Problem werden,
wenn wir Zeile 13 n�her betrachten:

<pre>
return book1.m_title < book2.m_author;
</pre>

Wenn die beiden Elemente von Klasse `Book` (hier: `m_title` und `m_author`) vom selben Typ sind,
ist das syntaktisch absolut korrekter Quellcode, der Compiler winkt hier freundlich durch.

Mit dem neuen Sprachfeature *Projektionen* l�sst sich das Sortieren wie folgt l�sen:

```cpp
01: std::vector<Book> books
02: {
03:     { "C++", "Bjarne Stroustrup", 1985, 20.55 },
04:     { "C", "Dennis Ritchie", 1972, 11.99 } ,
05:     { "Java", "James Gosling", 1995, 19.99 },
06:     { "C#", "Anders Hejlsberg", 2000, 29.99 }
07: };
08: 
09: // sort books
10: std::ranges::sort(books, std::less{}, &Book::m_price);
11: 
12: for (int index{ 1 }; const auto & [title, author, year, price] : books) {
13:     std::println("{}: [{}] ", price, title);
14:     index++;
15: }
16: std::println("");
17: 
18: // list all titles
19: auto titlesView{ books | std::ranges::views::transform(&Book::m_title) };
20: 
21: for (const auto& title : titlesView) {
22:     std::println("{}", title);
23: }
```

Dieses Mal wird der Quellcode nicht unbedingt k�rzer, daf�r aber pr�gnanter.
Wir k�nnen mit *Projektionen* viel klarer zu verstehenden Code schreiben,
der weniger anf�llig f�r unbeabsichtigte Fehler ist!

Wir geben einfach `std::ranges::less` (oder ein anderes Funktionsobjekt) und einen Pointer auf ein Data Member als Argument mit.
Unter der Haube kommt hier `std::invoke` zum Einsatz, dies nur der Vollst�ndigkeit halber erw�hnt.

---

[Zur�ck](Readme.md)

---
