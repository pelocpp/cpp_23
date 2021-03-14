# Ranges

[Zur�ck](../../Readme.md)

---

[Quellcode: Ranges01.cpp](Ranges01.cpp)<br/>
[Quellcode: Ranges02.cpp](Ranges02.cpp)

---

## Einleitung

Typischerweise verwenden die meisten generischen Algorithmen in der C ++ -Standardbibliothek (STL)
wie `std::sort` zwei Iteratoren (die von `begin()` und `end()` zur�ckgegebenen Objekte).

Wenn Sie einen `std::vector<int> vec` sortieren m�chten, m�ssen Sie

```cpp
std::sort(vec.begin (), vec.end());
```

und nicht

```cpp
std::sort(vec);
```

aufrufen. Warum wurde dieses Design mit Iteratoren gew�hlt? Es ist flexibler, weil es zum Beispiel folgende Variationen erm�glicht:

  * Sortieren aller Elemente beginnend nach dem f�nften Element des Vektors:
```cpp
std::sort(vec.begin() + 5, vec.end());
```
  * Einsatz eines nicht standardm��igen Iterators, zum Beispiel eines *Reverse Iterators* (Sortierung in umgekehrter Reihenfolge):
```cpp
std::sort(vec.rbegin(), vec.rend());
```
  * Kombination beider Beispiele (sortiere alle Elemente au�er den letzten 5 in umgekehrter Reihenfolge):
```cpp
std::sort(vec.rbegin() + 5, vec.rend());
```

Diese Schnittstelle ist jedoch weniger intuitiv als das Aufrufen von `std::sort` f�r den Container,
den man sortieren m�chte und es erm�glicht mehr Fehlerquellen,
z.B. das Mischen von zwei inkompatiblen Iteratoren.

C++ 20 f�hrt das Konzept der *Ranges* ein und stellt Algorithmen bereit,
die diese im Namesraum `std::range` akzeptieren, z.B. 

```cpp
std::range::sort(vec);
```

funktioniert jetzt, wenn `vec` ein *Range* ist - und Vektoren sind Bereiche!
Wie lassen sich die drei betrachteten Beispiele in C++ 20 mit *Ranges* formulieren:

```cpp
std::ranges::sort(std::views::drop(vec, 5));
std::ranges::sort(std::views::reverse(vec));
std::ranges::sort(std::views::drop(std::views::reverse(vec), 5));
```

## Nomenklatur

**Container** stellen die bekannteste Auspr�gung von *Bereichen* dar,
sie besitzen ihre Elemente.

**Ansichten** (**Views**) sind Bereiche, deren Daten normalerweise in einem anderen Bereich definiert sind
und diesen zugrunde liegenden Bereich �ber einen Algorithmus (Operation) transformieren.
**Views** besitzen keine Daten &ndash; abgesehen von Daten, die zur Ausf�hrung des Algorithmus ben�tigt werden.
Die Zeit, die zum Erstellen, Zerst�ren oder Kopieren ben�tigt wird, sollte nicht von der Anzahl der Elemente
des zugrunde liegenden Bereich abh�ngen.

Der Algorithmus als solcher wird verz�gert ausgef�hrt (so genannte **Lazy-Evaluation**),
damit mehrere Ansichten kombiniert werden k�nnen.

## Views und *Lazy-Evaluation*

Ein wichtiges Merkmal von Ansichten ist, dass unabh�ngig von der Transformation,
die sie anwenden, dies zum Zeitpunkt der Anforderung eines Elements geschieht
und nicht zum Zeitpunkt der Erstellung der Ansicht.

```cpp
std::vector<int> vec { 1, 2, 3, 4, 5, 6 };
auto view = std::views::reverse (vec);
```

Hier ist `view` eine Ansicht (*View*) &ndash; durch das Erstellen der Ansicht wird weder `vec` ge�ndert,
noch sind in `view` Elemente gespeichert. Die Zeit, die zum Erstellen von `view` ben�tigt wird,
und seine Gr��e im Speicher sind *unabh�ngig* von der Gr��e von `vec`!

```cpp
std::vector<int> vec { 1, 2, 3, 4, 5, 6 };
auto view = std::views::reverse(vec);
std::cout << *view.begin () << std::endl;
```

Die Ausgabe lautet `6` aus, aber das Wichtigste ist,
dass das Aufl�sen des ersten Elements von `view` bzgl. des letzten Elements von `vec` **on demand** erfolgt.
Dies garantiert, dass *Ansichten* genauso flexibel wie Iteratoren verwendet werden k�nnen,
bedeutet jedoch auch, dass die *Ansicht*, wenn sie eine &ldquo;teure&ldquo; Transformation durchf�hrt,
wiederholt ausgef�hrt werden muss, wenn dasselbe Element mehrmals angefordert wird.

Nat�rlich interessiert man sich auch f�r den Datentyp der *View* &ndash; es soll hier nichts verborgen bleiben.
Das `view`-Objekt w�re ohne `auto` so zu definieren:

```cpp
std::ranges::reverse_view<std::ranges::ref_view<std::vector<int>>> view = 
    std::views::reverse(vec);
```  

Wir wollen den Aspekt der *Lazy-Evaluation* nochmals an einem weiteren Beispiel vertieft betrachten:

```cpp
std::vector<int> vec{ 1, 5, 6, 8, 5 };

auto square = [](int const i) {
    std::cout << "calculate square of " << i << std::endl;
    return i * i; 
};

auto view = vec | std::views::transform(square) | std::views::drop(2);
std::cout << *std::begin(view) << std::endl;
```

In dem Beispiel wird auf ein `std::vector<int>``-Objekt eine *View* angewendet, 
die das Quadrat jedes Elements berechnet, und danach eine weitere *View*, die die ersten beiden Elemente des Resultat-Vektors l�scht.
Jetzt stellt sich die Frage, wenn wir nur am ersten Element des Resultat-Vektors interessiert sind,
wieviele &ldquo;Quadrierungsoperationen&rdquo; tats�chlich ausgef�hrt werden? 

*Ausgabe*:

```cpp
calculate square of 6
36
```

Die Ausgabe verifiziert das Feature der *Lazy-Evaluation*,
es wird nur eine einzige &ldquo;Quadrierungsoperationen&rdquo; aufgerufen!

## Kombinierbarkeit

Sie haben sich vielleicht gefragt, warum ich geschrieben habe

```cpp
auto view = std::views::reverse(vec);
```

und nicht

```cpp
std::views::reverse view {vec};
```

Dies liegt daran, dass `std::views::reverse` nicht die Ansicht selbst ist,
sondern ein *Adapter*, der den zugrunde liegenden Bereich (in unserem Fall `std::vector`) verwendet
und ein Ansichtsobjekt zum Vektorobjekt zur�ckgibt.
Der genaue Typ dieser Ansicht wird durch den Einsatz von `auto` verborgen!
Dies hat den Vorteil, dass wir uns nicht um die Template Argumente des *View*-Typs k�mmern m�ssen,
aber was noch wichtiger ist, der Adapter verf�gt �ber eine zus�tzliche Funktionalit�t:
Er kann mit anderen Adaptern verkettet werden!

```cpp
std::vector<int> vec {1, 2, 3, 4, 5, 6};
auto view = vec | std::views::reverse | std::views::drop(2);
std::cout << *view.begin () << std::endl;
```

*Ausgabe*:

```cpp
4
```

Es wird `4` ausgegeben, da `4` das 0-te Element des umgekehrten Vektors ist, nachdem die ersten beiden Elemente gel�scht wurden.

Im obigen Beispiel wird der Vektor in den *Reverse*-Adapter und dann in den *Drop*-Adapter "geleitet".
Ein &ldquo;kombiniertes&rdquo; *View*-Objekt wird zur�ckgegeben.
Die &ldquo;Pipe&rdquo; ist nur eine andere Notation, die die Lesbarkeit verbessert, das hei�t

```cpp
vec | foo | bar(3) | baz(7);
```

entspricht 

```cpp
baz(bar(foo(vec), 3), 7);
```

Beachten Sie, dass der Zugriff auf das 0. Element der *View* immer noch verz�gert ist,
sprich um welches Element es geht, wird zum Zeitpunkt des Zugriffs bestimmt.

Wenn wir die Aufl�sung der &ldquo;Pipe&rdquo;-Notation an dem Beispiel mit dem `` vollziehen,
lautet das Code-Fragment:

```cpp
std::vector<int> vec{ 1, 2, 3, 4, 5, 6 };
auto view = std::views::drop(std::views::reverse(vec), 2);
std::cout << *view.begin() << std::endl;
```
---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen aus

[A beginner's guide to C++ Ranges and Views](https://hannes.hauswedell.net/post/2019/11/30/range_intro/)

von Hannes Hauswedell.

Viele Beispiele zum Vergleichen von Code-Fragmenten ohne und mit *Ranges*-Features finden Sie in 

[C++ code samples before and after Ranges](https://mariusbancila.ro/blog/2019/01/20/cpp-code-samples-before-and-after-ranges/)

vor.

---

[Zur�ck](../../Readme.md)

---
