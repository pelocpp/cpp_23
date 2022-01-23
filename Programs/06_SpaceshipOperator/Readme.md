# Spaceship Operator / 3-Way Comparison Operator / 3-Weg-Vergleichsoperator 

[Zur�ck](../../Readme.md)

---

[Quellcode](SpaceshipOperator.cpp)

---

## �berblick

Der 3-Weg-Vergleichsoperator `<=>`, auch als *Spaceship*-Operator bezeichnet, bestimmt f�r zwei Objekte `A` und `B`,
ob `A < B`, `A == B` oder `A > B` gilt. Der Code f�r den  *Spaceship*-Operator kann vom �bersetzer automatisch generiert werden.
Historisch gesehen ist der *Spaceship*-Operator keine neue Erfindung, die `strcmp()`-Funktion aus C ist �hnlich definiert.
Bei zwei C-Zeichenketten wird eine ganz Zahl als Ergebnis zur�ckgegeben, wobei

  * &lt; 0 bedeutet, dass die erste Zeichenfolge kleiner ist
  * == 0 bedeutet, dass beide Zeichenfolgen gleich sind
  * &gt; 0 bedeutet, dass die erste Zeichenfolge gr��er ist

gilt. Es k�nnen also drei Ergebnisse vorhanden sein, daher die Begrifflichkeit des Drei-Wege-Vergleichs.


## Verschiedene M�glichkeiten einer Beziehung

> &ldquo;Nicht alle Gleichheitsbeziehungen sind gleich&rdquo;!

Mit dem 3-Weg-Vergleichsoperator k�nnen nicht nur Ordnungen (Beziegungen) und Gleichheit zwischen Objekten ausgedr�ckt werden,
sondern auch die Eigenschaften dieser Beziehungen. Dazu gibt es in C++ f�nf so genannte *Beziehungsst�rken*,
die wir an Hand einer Reihe von Beispielen betrachten:

#### `std::strong_ordering`

Hier gilt: Gleiche Objekte k�nnen nicht voneinander unterschieden werden.

  * Ein typisches Beispiel sind `int`-Variablen. Zwei Variablen `a` und `b` mit demselben Wert lassen sich nicht voneinander unterscheiden.

  * Ein zweites Beispiel: Betrachten wir eine Klasse `Rectangle` mit zwei Instanzvariablen `m_width` und `m_height`.
    Gilt f�r zwei `Rectangle` Objekte, dass deren `m_width`- und `m_height`-Instanzvariablen gleich sind,
    dann lassen sich auch die beiden `Rectangle` Objekte nicht voneinander unterscheiden.

#### `std::weak_ordering`

Objekte, die an Hand der Beziehungsst�rke `std::weak_ordering` gleich sind, m�ssen nicht als soche gleich sein.

  * Beispiel: F�r Zeichenketten &ndash; egal, ob hier die Klasse `std::string` oder eine andere benutzerdefinierte Klasse betrachtet wird &ndash;,
    kann man einen Vergleich definieren, der Gro�- und Kleinschreibung au�er Acht l�sst.
    Zwei Zeichenketten `"ABC"` und `"abc"` w�ren an Hand dieser Definition dann gleich,
    die Objekte sind es nicht.

  * Ein zweites Beispiel: F�r eine benutzerdefinierte Klasse `Rectangle` &ndash; m�glicherweise wieder mit den Instanzvariablen `m_width` und `m_height` &ndash;
    legt man fest, dass zwei `Rectangle`-Objekte mit derselben Fl�che gleich sind.

#### `std::partial_ordering`

Die Beziehungsst�rke `std::partial_ordering` ist �hnlich zu `std::weak_ordering` nur mit dem Unterschied,
dass es manche Werte gibt, die sich �berhaupt nicht miteinander vergleichen lassen.

  * Beispiel: Zum Datentyp `float` oder `double` gibt es den Wert NaN,
    der mit einem regul�ren Gleitpunktwert nicht verglichen werden kann.
    Man kann weder den Vergleich `5 < NaN` oder `5 > NaN` formulieren.
 
#### `std::strong_equality`

Es gibt &ndash; nicht wenige &ndash; Datentypen, f�r die eine Aussage &ldquo;Wert_1 ist kleiner als Wert_2&rdquo; keinen Sinn ergibt.

  * Beispiel: Um auf eine Datenbank zuzugreifen, gibt es zum Zwecke des Verbindungsaufbaus in den meisten Bibliotheken
    eine Klasse `SqlConnection`. F�r zwei `SqlConnection`-Objekte `conn1` und `conn2` ergebene die Operatoren
    `<`, `<=`, `>` und `>=` keinen Sinn.

#### `std::weak_equality`

�hnlich zu `std::weak_ordering`: F�r an sich gleiche Objekte kann es eben doch Unterschiede geben.

---

## Die f�nf Vergleichskategorien im �berblick

<img src="cpp_20_relations_orderings.svg" width="500">

*Abbildung* 1: C++ Beziehungsst�rken.

Nicht alle Vergleichsoperatoren sind f�r alle Beziehungsst�rken vorhanden. Es gilt hier:

| Beziehungsst�rken | Verf�gbare Operatoren |
|:-------------- |-----------------------------------------|
| `std::strong_ordering`  | `==`  `!=`  `<`  `>`  `<=`  `>=` | 
| `std::weak_ordering`    | `==`  `!=`  `<`  `>`  `<=`  `>=` | 
| `std::partial_ordering` | `==`  `!=`  `<`  `>`  `<=`  `>=` | 
| `std::strong_equality`  | `==`  `!=` | 
| `std::weak_equality`    | `==`  `!=` | 

Tabelle 1.  C++&ndash;Beziehungsst�rken und Operatoren.

---

## Der Quellcoode im �berblick

  * Namensraum `Spaceship_01_Operator_Equality`:<br/>Hier wird gezeigt, dass ab C++ 20 bei vorhandener
    `==`-Implementierung auch der entgegengesetzte Operator `!=` definiert ist.  

  * Namensraum `Spaceship_02_BuiltIn`:<br/>Der *Spaceship*-Operator `<=>`-Operator f�r Standard-Datentypen.  

  * Namensraum `Spaceship_03_Operator_Point`:<br/>Der *Spaceship*-Operator `<=>`-Operator automatisch erzeugt.

  * Namensraum `Spaceship_04_Operator_Fraction`:<br/>Der *Spaceship*-Operator `<=>`-Operator benutzer-definiert.

---


## Literaturhinweise:

Interessante Informationen und Beschreibungen des *Spaceship*-Operators finden sich zum Beispiel hier vor:

[Spaceship Operator](https://blog.tartanllama.xyz/spaceship-operator/)

[StackOverflow.com](https://stackoverflow.com/questions/47466358/what-is-the-spaceship-three-way-comparison-operator-in-c)

[Using C++20�s Three-way Comparison <=>](https://www.jonathanmueller.dev/)

[Simplify Your Code With Rocket Science](https://devblogs.microsoft.com/cppblog/simplify-your-code-with-rocket-science-c20s-spaceship-operator/)

---

[Zur�ck](../../Readme.md)

---
