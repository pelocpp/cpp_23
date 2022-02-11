# Erste Schritte

[Zur�ck](Readme.md)

---

[Quellcode](Coroutines_02_FirstSteps.cpp)

---

## Motivation

Zur Motivation von Coroutinen betrachten wir eine Funktion `getNumbers`:

```cpp
std::vector<int> getNumbers(int begin, int end)
{
    std::vector<int> numbers;
    
    for (int i = begin; i <= end; ++i) {
        numbers.push_back(i);
    }

    return numbers;
}
```

Folgende Beobachtungen sind wichtig:

  * Ein Aufrufer von `getNumbers` bekommt immer alle Werte, die er durch die beiden Parameter 
    `begin` und `end` anfordert. Dies wirkt sich negativ bei vielen Zahlen auf den ben�tigten Speicherplatz aus.

  * Dar�berhinaus kann es sein, dass ein Aufrufer nach dem Auswerten der ersten 5 Zahlen
    m�glicherweise an den restlichen Zahlen �berhaupt nicht mehr interessiert ist.
    Bei einem Wert `end` gleich 1000 wurden also fast alle angeforderten Werte umsonst berechnet und transportiert.

Man spicht in der Informatik bei derartigen Berechnungen immer in der Vorgehensweise &ldquo;greedy&rdquo; oder &ldquo;lazy&rdquo;.
Die Funktion `getNumbers` f�llt offensichtlich in die erste Kategorie.
Mit Hilfe von Coroutinen k�nnten Sie eine Variation von  `getNumbers ` auf Basis der &ldquo;lazy&rdquo;-Strategie
umsetzen.

```cpp
Generator generatorForNumbers(int begin, int end)
{
    for (int i = begin; i <= end; ++i) {
        co_yield i;
    }
}
```

Eine Anwendung k�nnte nun so aussehen:

```cpp
Generator coroutine = generatorForNumbers(1, 10);

while (true) {

    int value = coroutine.next();
    if (value == -1) {
        break;
    }
}
```

Diese beiden Code-Fragmente sind nicht unmittelbar �bersetzungsf�hig.
Wir nehmen sie als Ausgangspunkt unserer Betrachtungen der ersten Schritte im
Umfeld von C++&ndash;Coroutinen.

## Coroutinen: *stackless* und *stackful*

Coroutinen existieren in den verschiedenene Programmierumgebungen prinzipiell
in zwei Auspr�gungen:

  * *stackless*
  * *stackful*

*Stackful* Coroutinen haben einen separaten Stack (�hnlich einem Thread), der den
so genannten *Coroutine-Frame* und die verschachtelten Aufruf-Frames enth�lt.
Dadurch ist es m�glich, an einer beliebigen Stelle im verschachtelten Aufruf-Frame
zu unterbrechen (*suspend*) und wieder fortzufahren (*resume*).

*Stackless* Coroutinen m�ssen den Coroutine-Frame an einer anderen Stelle speichern
(normalerweise auf dem Heap) und verwenden den Stapel des *aktuell* ausgef�hrten Threads,
um verschachtelte Aufrufe durchf�hren zu k�nnen.

<img src="C20_Coroutine_Stackless_Stackful.png" width="500">

*Abbildung* 1: Funktionen versus Coroutinen &ndash; *stackless* versus *stackful* Coroutinen.

In C++ 20 finden wir eine Unterst�tzung f�r *stackless* Coroutinen vor.


## Drei neue Schl�sselw�rter

Eine C++&ndash;Funktion erlangt den Status einer Coroutine, wenn sie eines der
drei neuen C++&ndash;Schl�sselw�rter verwendet:

  * `co_return`
  * `co_yield`
  * `co_await`

Diese recht einfache Aussage l�sst sich etwas pr�zisieren:

Eine Funktion ist eine Coroutine, wenn ihre Definition eine der folgenden Aktionen ausf�hrt:

  1. verwendet den `co_await` Operator, um die Ausf�hrung der Funktion zu unterbrechen (*suspend*) ...
  2. verwendet das Schl�sselwort `co_yield`, um die Ausf�hrung der Funktion zu unterbrechen (*suspend*) und um einen Wert zur�ckgeben ...
  3. verwendet das Schl�sselwort `co_return`, um die Ausf�hrung abzuschlie�en und um einen Wert zur�ckzugeben ...


## Ein erstes Beispiel

Das folgende Code-Fragment &ndash; noch nicht �bersetzungsf�hig &ndash; beschreibt eine Coroutine:

```cpp
#include <coroutine>

Generator coroutine(�nt n)
{
    co_yield "Hello";
    co_yield "World";
    co_return;
}
```

Man k�nnte zu der Formulierung neigen, dass `coroutine` die Definition einer Funktion ist.
Pr�ziser m�ssen wir sagen: Wir verwenden die Syntax einer Funktionsdefinition,
um einen Codeblock zu beschreiben, dem bei der Instanziierung Argumente �bergeben werden k�nnen
(hier exemplarisch der Parameter `n`)

Zum zweiten erkennen wir, dass ein R�ckgabetyp &ndash; hier der exemplarische Typ `Generator` &ndash;
in Erscheinung tritt, und das, obwohl `coroutine()` keine `return`-Anweisung enth�lt &ndash; 
und per Definition auch nicht enthalten kann.

<img src="C20_Coroutine_vs_Function.png" width="700">

*Abbildung* 2: Coroutinen k�nnen sich suspendieren.

Der Compiler ordnet diesen Codeblock neu an, um den Coroutinen-Mechanismus mit seinem
*Save*- und *Restore*-Anweisungen auszurollen. Diese werden unter anderem durch 
die beiden Schl�sselw�rter `co_yield` und `co_return` beeinflusst.

Ein Anwendungsprogramm (ebenfalls noch nicht �bersetzungsf�hig),
dass diese Coroutine verwenden will, k�nnte so aussehen:

```cpp
Generator client = coroutine();

std::cout << client.next();
std::cout << client.next();
```

Sobald wir alle zwei via `co_yield` verf�gbaren Werte verbraucht haben,
wird die Coroutine beendet und der gesamte Speicher freigegeben,
der zum Speichern des Coroutinen-Zustands verwendet wurde.

Das Zusammenspiel zwischen `co_yield` (Coroutine / Produzent) und `next` (Anwendung / Konsument)
kann exakter so beschrieben werden:

1. den aktuellen Client-Code suspendieren (Konsument)
2. den Zustand der Coroutine wiederherstellen (Produzent)
3. den Coroutinen-Code ab der letzten `co_yield`-Anweisung fortsetzen
4. speichern des Werts der n�chsten `co_yield`-Anweisung
5. den Coroutinen-Zustand abspeichern (Produzent)
6. den Zustand des Client-Codes wiederherstellen (Konsument)
7. den Client-Code fortsetzen, indem diesem der gespeicherte Wert aus der `co_yield`-Anweisung zugef�hrt wird

## Eine Coroutine ist keine C-Funktion

Wollten wir eines der Coroutinen-Schl�sselw�rter im Kontext einer &ldquo;normalen&rdquo; C/C++&ndash;Funktion
verwenden, erhalten wir ein nicht �bersetzungsf�higes C++-Programm:

<pre>
int foo() { co_return 2; }
</pre>

Wir erhalten je nach verwendetem Compiler folgende Fehlermeldung:

  * Visual C++: `'promise_type': is not a member of 'std::coroutine_traits<int>'`
  * gcc: `In function 'int foo()': error: unable to find the promise type for this coroutine`



## Der C++ *Coroutine Framework* 

### Ein *Generator*

Um im C++&ndash;Laufzeitsystem eine Coroutine zu verwalten, bedarf es eines so genannten
*Generator*-Objekts, das den Lebenszyklus einer Coroutine abbildet und verwaltet.

Informationen dieser Klasse sind in der `include`-Datei

```cpp
#include <coroutine>
```

vorhanden. Man k�nnte geneigt sein zu vermuten, dass in dieser Include-Datei eine Schnittstelle
oder eine abstrakte Klassendefinition f�r das *Generator*-Objekt abgelegt sind.
In C++ 20 gibt es hierf�r
`concepts`, die entsprechende Forderungen (`requires`) an eine Implementierung stellen.
Es m�ssen je nach Anwendungsfall eine Reihe von Methoden eines bestimmten Namens
und mit bestimmten Parametertypen definiert sein.

Jede C++&ndash;Anwenderklasse, die den Anspruch erhebt, eine *Generator*-Klasse zu sein,
muss einen Bezug zu einer vordefinerten Klasse eines bestimmten Namens aufweisen:

```cpp
struct promise_type
```

Genau betrachtet erzwingen C++20 Coroutinen,
dass wir zwei miteinander verbundene, unterst�tzende Klassen bereitstellen:

  * eine Klasse zum Speichern des Coroutinen-Zustands und zum Speichern der Daten, die via `co_yield` bereit gestellt werden.
    Diese Klasse wird in der Regel als *Promise* bezeichnet, daher auch der Typbezeichner `promise_type`.
  * eine Klasse zur Verwaltung des Coroutinen-(Promise-)Objekts &ndash; das ist die *Generator*-Klasse.

Im *Promise*-Objekt sind die Lebenszyklusmethoden einer Coroutine bereitstellen.


### Das *Promise*-Objekt

Wir f�hren zwei Methoden des *Promise*-Objekts exemplarisch auf:

Verwendet eine Coroutine eine `co_yield`-Anweisung mit einem `std::string`-Wert,
ben�tigen wir im *Promise*-Objekt eine Methode mit der folgenden Signatur:

```cpp
std::suspend_always yield_value(std::string value);

```

Der Parameter ist gewisserma�en das Objekt, welches das Argument von `co_yield` darstellt.
Der R�ckgabewert von `yield_value` teilt dem Laufzeitsystem mit, 
ob der Thread-Status gespeichert werden soll.

Die `yield_value`-Methode muss ihr Argument speichern,
damit es bei Anforderung den Wert an die aufrufende Routine (Consumer)
zur�ckgeben kann:

```cpp
std::suspend_always yield_value(std::string value) {
    m_value = std::move(value);
    return {};
}
```

Die `return`-Anweisung entspricht &ldquo;Modern C++&rdquo;:
`return {}` bedeutet einfach, ein Objekt des R�ckgabetyps (Standard-Konstruktor)
f�r diese Methode zu erstellen. Man k�nnte auch

```cpp
return std::suspend_always{}
```

schreiben.

---

### Beispielquellcode: Das *Promise*-Objekt

```cpp
01: struct Promise
02: {
03:     using value_type = std::string;
04: 
05:     std::string m_value{};
06: 
07:     Promise() = default;
08: 
09:     std::suspend_always initial_suspend() { return {}; }
10: 
11:     std::suspend_always final_suspend() noexcept { return {}; }
12: 
13:     void unhandled_exception() { std::rethrow_exception(std::current_exception()); }
14: 
15:     std::suspend_always yield_value(std::string value) {
16:         m_value = std::move(value);
17:         return {};
18:     }
19: 
20:     void return_void() {
21:         m_value.clear();
22:     }
23: 
24:     Generator get_return_object();
25: };
```

### Beispielquellcode: Der Generator

```cpp
01: class Generator
02: {
03: public:
04:     using promise_type = Promise;
05: 
06:     explicit Generator(std::coroutine_handle<Promise> handle)
07:         : m_handle(handle)
08:     {}
09: 
10:     ~Generator() {
11:         if (m_handle) { m_handle.destroy(); }
12:     }
13: 
14:     Promise::value_type next() {
15:         if (m_handle) {
16:             m_handle.resume();
17:             return m_handle.promise().m_value;
18:         }
19:         else {
20:             return {};
21:         }
22:     }
23: 
24: private:
25:     std::coroutine_handle<Promise> m_handle{};
26: };
27: 
28: Generator Promise::get_return_object()
29: {
30:     return Generator{ std::coroutine_handle<Promise>::from_promise(*this) };
31: }
```

### Beispielquellcode: Die Coroutine

```cpp
01: // coroutine
02: Generator generator()
03: {
04:     co_yield std::string{ "Hello " };
05:     co_yield std::string{ "World" };
06:     co_return;
07: }
```

### Beispielquellcode: Der Konsument

```cpp
01: void test()
02: {
03:     auto coroutine = generator();
04: 
05:     std::cout << coroutine.next() << std::endl;
06:     std::cout << coroutine.next() << std::endl;
07:     std::cout << std::endl;
08: }
```

## �bung:

Wie m�ssen Sie den Quellcode f�r den &ldquo;Hello World&rdquo;-Generator umschreiben,
um die in der Einleitung skizzierte Idee eines &ldquo;Lazy&rdquo;-Generators f�r
aufsteigende, ganze Zahlen zu implementieren?


## Literaturhinweise:

Die Anregungen zu den Beispielen stammen zum gro�en Teil aus dem Artikel

&ldquo;[C++20 Coroutines](https://blog.feabhas.com/2021/09/c20-coroutines/#Promise_data_holder)&rdquo;
von Martin Bond.

Die Motivation stammt von [Rainer Grimm](https://www.modernescpp.com/index.php/c-20-coroutines-the-first-overview).

---

[Zur�ck](Readme.md)

---
