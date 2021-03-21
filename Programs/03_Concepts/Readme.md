# Concepts

[Zur�ck](../../Readme.md)

---

[Quellcode](Concepts01.cpp)

---

## �berblick

*Kurz und b�ndig*:

C++-*Concepts* (*Konzepte*) definieren eine Reihe von Bedingungen (*Conditions*) oder Einschr�nkungen (*Constraints*),
die ein Datentyp erf�llen muss, um als Template Argument verwendet werden zu k�nnen.

Ein einfaches Beispiel: Ich m�chte eine C++ -Funktion schreiben, die zwei Werte summiert und das Ergebnis auf der Konsole ausgibt.
In C++ 17 k�nnte man folgendes Template erstellen:

```cpp
template <typename A, typename B>
void sumAndPrint(const A& a, const B& b)
{
    std::cout << (a + b) << std::endl;
}
```

Die Funktionsschablone `sumAndPrint` funktioniert ordnungsgem�� f�r Typen `A` und `B`,
die den `+`-Operator besitzen. 
Wenn die verwendeten Typen den `+`-Operator nicht implementieren,
versucht der Compiler zun�chst mit etwaigen Typumwandlungen, 
die f�r den Programmersteller nicht immer intuitiv sein m�ssen,
auf diese Weise sein Ziel zu erreichen. L�sst sich der `+`-Operator auch auf diese Weise nicht finden,
wird eine Fehlermeldung erzeugt.

Man kann festhalten: Die Funktionsweise des Compilers ist selbstverst�ndlich korrekt,
aber ein Fehlschlagen w�hrend der eigentlichen Ersetzung der Template Parameter mit realen Datentypen
ohne vorherige �berpr�fung entspricht eher einem *reagierenden* als einem *proaktiven* Verhalten.
Auf diese Weise sind die Fehlermeldungen aufgrund von Substitutionsfehlern ziemlich schwer zu lesen und zu verstehen.

C++-*Concepts* stellen ein Hilfsmittel dar, um Anforderungen an Datentypen zu stellen, die erf�llt sein m�ssen,
damit diese z�r Instanziierung von Templates herangezogen werden d�rfen.
Sind derartige Anforderungen verf�gbar, so �berpr�ft der Compiler diese, 
bevor die eigentliche Ersetzung gestartet wird (Generierung der Funktionsschabone / des Klassentemplates).

Um beim Beispiel von oben zu bleiben:

```cpp
template <typename T, typename U = T>
concept Sumable =
requires(T a, U b)
{
   { a + b };
   { b + a };
};
```

Ein neues Schl�sselwort `concept` wird verwendet, um ein C++ -*Konzept* (*Concept*) zu definieren.
Es wird als Template definiert, da das *Konzept* anhand des Typs oder der Typen bewertet wird,
die hier als Templateargumente verwendet werden (hier: `T` und `U`).

Im vorliegenden Beispiel hei�t das *Konzept* `Sumable`, nach dem &ldquo;`=`&rdquo; -Zeichen erwartet der Compiler
ein Pr�dikat, das zur Kompilierungszeit ausgewertet wird.
Mit diesem *Konzept* wird eine Funktion `sumAndPrint` nun auf die folgende Weise definiert:

```cpp
template <Sumable A, Sumable B>
void sumAndPrint (const A& a, const B& b)
{
    std::cout << (a + b) << "\n";
}
```

In der Template Klausel erkennen wir, dass an die Stelle des Schl�sselworts `typename`
ein Bezeichner eines Konzepts (hier: `Sumable`) tritt.
Wenn ich beispielsweise ein *Konzept* erstellen m�chte,
um die Auswahl m�glicher Datentypen auf `int` oder `double` zu beschr�nken,
kann ich dieses wie folgt definieren:

```cpp
template <typename T>
concept SumableOnlyForIntsAndDoubles =
    std::is_same<T, int>::value || std::is_same<T, double>::value;
```

&ldquo;Type Traits&rdquo; sind ein �bliches Hilfsmittel (hier: `std::is_same<T, U>`),
um derartige *Constraints* zu erstellen.

Zur�ck zum Beispiel der Funktionsschablone `sumAndPrint`:
M�chte ich erreichen, dass die beiden Typen `A` und `B`  den Operator `+` implementieren,
muss ich ein oder mehrere *Anforderungen* (*Requirements*) f�r diese Einschr�nkung spezifizieren.
Zu diesem Zweck gibt es das neue Schl�sselwort `require`.
Jede Definition zwischen geschweiften Klammern im `require`-Block
(tats�chlich ist `require` immer ein Block, auch wenn nur eine Anforderung ohne geschweiften Klammern angegeben werden kann)
muss von den Datentypen erf�llt werden, die zur Instanziierung des Templates benutzt werden.
Im vorliegenden Beispiel m�ssen `a + b` und `b + a` g�ltige Operationen sein.

Wenn die Typen `T` oder `U` den Operator `+` nicht implementieren, werden die Anforderungen nicht erf�llt.
Das hei�t, der �bersetzer stoppt, bevor er �berhaupt versucht, `A` und `B` durch die tats�chlichen Typen zu ersetzen.

Wie geht es weiter, wenn wir als realen Datentyp zum Beispiel die folgende Klasse `Adder` heranziehen wollen,
die den `+` Operator ebenfalls bereitstellt:

```cpp
class Adder
{
private:
    int m_value{};

public:
    Adder (int value) : m_value { value } {}

    Adder operator+(const Adder& other) const
    {
        return { m_value + other.m_value };
    }

    int operator()() const { return m_value; }
};
```

Zum Konzept `Sumable` w�re die Klasse `Adder` ebenfalls kompatibel, aber nicht zum Konzept `SumableOnlyForIntsAndDoubles`.
Klasse `Adder` weist aber ein anderes Manko auf, sie hat den Operator `<<` zum Zwecke der Ausgabe auf der Konsole nicht implementiert
(wir legen dabei zu Grunde, dass es den Operator `<<` auch nicht als globale Funktion gibt).
Um diese Randbedingung zu erf�llen, m�ssen wir das `Sumable`-Konzept wie folgt erweitern:

```cpp
template <typename T, typename U = T>
concept Sumable =
requires(T a, U b)
{
    { a + b };
    { b + a };
}
&& requires(std::ostream& os, const T& a)
{
    { os << a };
};
```

Wenn wir nun noch f�r die Klasse `Adder` den Operator `<<` implementieren,

```cpp
std::ostream& operator<<(std::ostream& os, const Adder& n)
{
    os << n();
    return os;
}
```

k�nnen wir folgende Anweisungen schreiben:

```cpp
sumAndPrint(Adder{ 1 }, Adder{ 2 });
```

Beachten Sie prinzipiell die neue Schreibweise in den Funktionsschablonen:
Es hei�t nun `template <Sumable a, Sumable b>` anstelle der fr�heren Notation `template <typename A, typename B>`.
Auf diese Weise wird der Compiler angehalten, die Typargumente anhand des `Sumable`-Konzepts zu validieren.

---

Wir betrachten eine zweite Formulierung f�r *Anforderungen* (*Requirements*):
Wie w�re es, wenn wir mehrere &ldquo;Begr��er&rdquo; in mehreren Sprachen und eine Funktion &ldquo;Begr��ung&rdquo; implementieren wollten,
die den Begr��er verwendet, um &ldquo;Hallo&rdquo; zu sagen. Zum Beispiel so:

```cpp
template <Greeter G>
void greet(const G& greeter)
{
    greeter.sayHi();
}
```

Wie Sie sehen k�nnen, m�chte ich erreichen, dass die *Begr��er* eine Methode `sayHi` haben.
Daher k�nnte das Konzept wie folgt definiert werden, um den Typ `G` mit der Implementierung der Methode `sayHi()` zu beauftragen:

```cpp
template <typename G>
concept Greeter = requires(G g)
{
    { g.sayHi() } -> std::convertible_to<void>;
};
```

Mit einem solchen *Konzept* w�ren nun folgende Implementierungen m�glich:

```cpp
struct SpanishGreeter
{
    void sayHi() const { 
        std::cout << "Hola amigos" << std::endl;
    }
};

struct EnglishGreeter
{
    void sayHi() const { 
        std::cout << "Hello my friends" << std::endl;
    }
};

struct ItalianGreeter
{
    void sayHi() const {
        std::cout << "Ciao Ragazzi" << std::endl;
    }
};
```

Und damit folgendes Beispielprogramm:

```cpp
greet(SpanishGreeter{});
greet(EnglishGreeter{});
greet(ItalianGreeter{});
```

Ich schlie�e diese einf�hrenden Erl�uterungen mit folgenden erg�nzenden Bemerkungen ab:

  * Wenn Sie das neue Feature C++ - *Konzepte* verwenden, m�ssen Sie keine Basisklassen, Vererbung, virtuelle bzw. rein virtuelle Methoden
    und �hnliche Hilfsmittel der OO-Programmierung verwenden, um einen Vertrag zu erf�llen bzw. zu formulieren.
    Es sind nur die Anforderungen zu erf�llen, die das *Konzept* definiert, und das war's!

   * *Konzepte* sind eine &ldquo;Null-Kosten-Abstraktion&rdquo;, da ihre Validierung vollst�ndig zur Kompilierungszeit durchgef�hrt wird
      und der Compiler bei ordnungsgem��er �berpr�fung und Akzeptanz im Gegensatz zum Laufzeitaufwand
      f�r die Ausf�hrung virtueller Objekte keinen Code generiert, der mit dieser �berpr�fung in einem objektorientierten Ansatz zusammenh�ngt!

---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen aus

[C++20: Concepts, an introduction](https://oopscenities.net/2020/09/29/c20-concept/)

---

[Zur�ck](../../Readme.md)

---
