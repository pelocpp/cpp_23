# Concepts

[Zurück](../../Readme.md)

---

[Quellcode](Concepts01.cpp)

---

## Überblick

*Kurz und bündig*:

C++-*Concepts* (*Konzepte*) definieren eine Reihe von Bedingungen (*Conditions*) oder Einschränkungen (*Constraints*),
die ein Datentyp erfüllen muss, um als Template Argument verwendet werden zu können.

Ein einfaches Beispiel: Ich möchte eine C++ -Funktion schreiben, die zwei Werte summiert und das Ergebnis auf der Konsole ausgibt.
In C++ 17 könnte man folgendes Template erstellen:

```cpp
template <typename A, typename B>
void sumAndPrint(const A& a, const B& b)
{
    std::cout << (a + b) << std::endl;
}
```

Die Funktionsschablone `sumAndPrint` funktioniert ordnungsgemäß für Typen `A` und `B`,
die den `+`-Operator besitzen. 
Wenn die verwendeten Typen den `+`-Operator nicht implementieren,
versucht der Compiler zunächst mit etwaigen Typumwandlungen, 
die für den Programmersteller nicht immer intuitiv sein müssen,
auf diese Weise sein Ziel zu erreichen. Lässt sich der `+`-Operator auch auf diese Weise nicht finden,
wird eine Fehlermeldung erzeugt.

Man kann festhalten: Die Funktionsweise des Compilers ist selbstverständlich korrekt,
aber ein Fehlschlagen während der eigentlichen Ersetzung der Template Parameter mit realen Datentypen
ohne vorherige Überprüfung entspricht eher einem *reagierenden* als einem *proaktiven* Verhalten.
Auf diese Weise sind die Fehlermeldungen aufgrund von Substitutionsfehlern ziemlich schwer zu lesen und zu verstehen.

C++-*Concepts* stellen ein Hilfsmittel dar, um Anforderungen an Datentypen zu stellen, die erfüllt sein müssen,
damit diese zúr Instanziierung von Templates herangezogen werden dürfen.
Sind derartige Anforderungen verfügbar, so überprüft der Compiler diese, 
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

Ein neues Schlüsselwort `concept` wird verwendet, um ein C++ -*Konzept* (*Concept*) zu definieren.
Es wird als Template definiert, da das *Konzept* anhand des Typs oder der Typen bewertet wird,
die hier als Templateargumente verwendet werden (hier: `T` und `U`).

Im vorliegenden Beispiel heißt das *Konzept* `Sumable`, nach dem &ldquo;`=`&rdquo; -Zeichen erwartet der Compiler
ein Prädikat, das zur Kompilierungszeit ausgewertet wird.
Mit diesem *Konzept* wird eine Funktion `sumAndPrint` nun auf die folgende Weise definiert:

```cpp
template <Sumable A, Sumable B>
void sumAndPrint (const A& a, const B& b)
{
    std::cout << (a + b) << "\n";
}
```

In der Template Klausel erkennen wir, dass an die Stelle des Schlüsselworts `typename`
ein Bezeichner eines Konzepts (hier: `Sumable`) tritt.
Wenn ich beispielsweise ein *Konzept* erstellen möchte,
um die Auswahl möglicher Datentypen auf `int` oder `double` zu beschränken,
kann ich dieses wie folgt definieren:

```cpp
template <typename T>
concept SumableOnlyForIntsAndDoubles =
    std::is_same<T, int>::value || std::is_same<T, double>::value;
```

&ldquo;Type Traits&rdquo; sind ein übliches Hilfsmittel (hier: `std::is_same<T, U>`),
um derartige *Constraints* zu erstellen.

Zurück zum Beispiel der Funktionsschablone `sumAndPrint`:
Möchte ich erreichen, dass die beiden Typen `A` und `B`  den Operator `+` implementieren,
muss ich ein oder mehrere *Anforderungen* (*Requirements*) für diese Einschränkung spezifizieren.
Zu diesem Zweck gibt es das neue Schlüsselwort `require`.
Jede Definition zwischen geschweiften Klammern im `require`-Block
(tatsächlich ist `require` immer ein Block, auch wenn nur eine Anforderung ohne geschweiften Klammern angegeben werden kann)
muss von den Datentypen erfüllt werden, die zur Instanziierung des Templates benutzt werden.
Im vorliegenden Beispiel müssen `a + b` und `b + a` gültige Operationen sein.

Wenn die Typen `T` oder `U` den Operator `+` nicht implementieren, werden die Anforderungen nicht erfüllt.
Das heißt, der Übersetzer stoppt, bevor er überhaupt versucht, `A` und `B` durch die tatsächlichen Typen zu ersetzen.

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

Zum Konzept `Sumable` wäre die Klasse `Adder` ebenfalls kompatibel, aber nicht zum Konzept `SumableOnlyForIntsAndDoubles`.
Klasse `Adder` weist aber ein anderes Manko auf, sie hat den Operator `<<` zum Zwecke der Ausgabe auf der Konsole nicht implementiert
(wir legen dabei zu Grunde, dass es den Operator `<<` auch nicht als globale Funktion gibt).
Um diese Randbedingung zu erfüllen, müssen wir das `Sumable`-Konzept wie folgt erweitern:

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

Wenn wir nun noch für die Klasse `Adder` den Operator `<<` implementieren,

```cpp
std::ostream& operator<<(std::ostream& os, const Adder& n)
{
    os << n();
    return os;
}
```

können wir folgende Anweisungen schreiben:

```cpp
sumAndPrint(Adder{ 1 }, Adder{ 2 });
```

Beachten Sie prinzipiell die neue Schreibweise in den Funktionsschablonen:
Es heißt nun `template <Sumable a, Sumable b>` anstelle der früheren Notation `template <typename A, typename B>`.
Auf diese Weise wird der Compiler angehalten, die Typargumente anhand des `Sumable`-Konzepts zu validieren.

---

Wir betrachten eine zweite Formulierung für *Anforderungen* (*Requirements*):
Wie wäre es, wenn wir mehrere &ldquo;Begrüßer&rdquo; in mehreren Sprachen und eine Funktion &ldquo;Begrüßung&rdquo; implementieren wollten,
die den Begrüßer verwendet, um &ldquo;Hallo&rdquo; zu sagen. Zum Beispiel so:

```cpp
template <Greeter G>
void greet(const G& greeter)
{
    greeter.sayHi();
}
```

Wie Sie sehen können, möchte ich erreichen, dass die *Begrüßer* eine Methode `sayHi` haben.
Daher könnte das Konzept wie folgt definiert werden, um den Typ `G` mit der Implementierung der Methode `sayHi()` zu beauftragen:

```cpp
template <typename G>
concept Greeter = requires(G g)
{
    { g.sayHi() } -> std::convertible_to<void>;
};
```

Mit einem solchen *Konzept* wären nun folgende Implementierungen möglich:

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

Ich schließe diese einführenden Erläuterungen mit folgenden ergänzenden Bemerkungen ab:

  * Wenn Sie das neue Feature C++ - *Konzepte* verwenden, müssen Sie keine Basisklassen, Vererbung, virtuelle bzw. rein virtuelle Methoden
    und ähnliche Hilfsmittel der OO-Programmierung verwenden, um einen Vertrag zu erfüllen bzw. zu formulieren.
    Es sind nur die Anforderungen zu erfüllen, die das *Konzept* definiert, und das war's!

   * *Konzepte* sind eine &ldquo;Null-Kosten-Abstraktion&rdquo;, da ihre Validierung vollständig zur Kompilierungszeit durchgeführt wird
      und der Compiler bei ordnungsgemäßer Überprüfung und Akzeptanz im Gegensatz zum Laufzeitaufwand
      für die Ausführung virtueller Objekte keinen Code generiert, der mit dieser Überprüfung in einem objektorientierten Ansatz zusammenhängt!

---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen aus

[C++20: Concepts, an introduction](https://oopscenities.net/2020/09/29/c20-concept/)

---

[Zurück](../../Readme.md)

---
