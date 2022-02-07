# Erste Schritte

[Zurück](../../Readme.md)

---

[Quellcode](Coroutines02_FirstSteps.cpp)

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

Ein Aufrufer von `getNumbers` bekommt immer alle Werte, die er durch die beiden Parameter 
begin und end anfordert. Dies wirkt sich bei vielen Zahlen auf den benötigten Speicherplatz aus.
Darüberhinaus kann es sein, dass ein Aufrufer nach dem Auswerten der ersten 5 Zahlen
möglicherweise an den restlichen überhaupt nicht mehr interessiert ist.
Bei einem Wert end gleich 1000 wurden also fast alle angeforderten Werte umsonst berechnet und transportiert.

Man spicht in der Informatik von &ldquo;greedy&rdquo; und &ldquo;lazy&rdquo; Berechnungen.
Die Funktion `getNumbers` fällt offensichtlich in die erste Kategorie.
Mit Hilfe von Coroutinen können Sie eine Variation von getNumbers auf Basis der &ldquo;lazy&rdquo;-Strategie
umsetzen.

```cpp
generator<int> generatorForNumbers(int begin, int end)
{
    for (int i = begin; i <= end; ++i) {
        co_yield i;
    }
}
```

Eine Anwendung könnte nun so aussehen:

```cpp
generator<int> generatorForNumbers(int begin, int end)
{
    for (int i = begin; i <= end; ++i) {
        co_yield i;
    }
}
```


## Coroutinen: *stackless* und *stackful*

Coroutinen existieren in den verschiedenene Programmierumgebungen prinzipiell
in zwei Ausprägungen:

  * *stackless*
  * *stackful*

*Stackful* Coroutinen haben einen separaten Stack (ähnlich einem Thread), der den
so genannten *Coroutine-Frame* und die verschachtelten Aufruf-Frames enthält.
Dadurch ist es möglich, an einer beliebigen Stelle im verschachtelten Aufruf-Frame
zu *suspendieren*.

*Stackless* Coroutinen müssen den Coroutine-Frame an einer anderen Stelle speichern
(normalerweise auf dem Heap) und verwenden den Stapel des aktuell ausgeführten Threads,
um verschachtelte Aufrufe durchführen zu können.

In C++ 20 haben wir eine Unterstützung für *stackless* Coroutinen.


## 3 neue Schlüsselwörter

Eine C++&ndash;Funktion erlangt den Status einer Coroutine, wenn sie eine der
drei neuen C++&ndash;Schlüsselwörter verwendet:

  * `co_return`
  * `co_yield`
  * `co_await`


## Ein erstes Beispiel

Das folgende Code-Fragment (noch nicht übersetzungsfähig) enthält eine Coroutine:

```cpp
#include <coroutine>

Generator coroutine()
{
    co_yield "Hello";
    co_yield "World";
    co_return;
}
```

Man könnte zu der Formulierung neigen, dass `coroutine` die Definition einer Funktion ist.
Präziser müssen wir sagen: Wir verwenden die Syntax einer Funktionsdefinition,
um einen Codeblock zu beschreiben, dem bei der Instanziierung Argumente übergeben werden können.

Zum zweiten erkennen wir, dass ein Rückgabetyp &ndash; hier der exemplarische Typ `Generator` &ndash;
in Erscheinung tritt, und das, obwohl `coroutine()` keine `return`-Anweisung enthält &ndash; 
und per Definition auch nicht enthalten kann.

Der Compiler ordnet diesen Codeblock neu an, um den Coroutinen-Mechanismus mit seinem
*Save*- und *Restore*-Anweisungen auszurollen. Diese werden unter anderem durch 
die beiden Schlüsselwörter `co_yield` und `co_return` beeinflusst.

Ein Anwendungsprogramm (ebenfalls noch nicht übersetzungsfähig),
dass diese Coroutine verwenden will, könnte so aussehen:

```cpp
Generator client = coroutine();

std::cout << client.next();
std::cout << client.next();
std::cout << client.next();
```

Sobald wir alle drei via `co_yield` verfügbaren Werte verbraucht haben,
wird die Coroutine beendet und der gesamte Speicher freigegeben,
der zum Speichern des Coroutinen-Zustands verwendet wurde.

Das Zusammenspiel zwischen `co_yield` (Coroutine) und `next` (Anwendung)
kann exakter so beschrieben werden:

1. den aktuellen Client-Code suspendieren
2. den Zustand der Coroutine wiederherstellen (Produzent)
3. den Coroutine-Code ab der letzten `co_yield`-Anweisung fortsetzen
4. speichern des Werts der nächsten `co_yield`-Anweisung
5. den Coroutinen-Zustand abspeichern
6. den Zustand des Client-Codes wiederherstellen
7. den Client-Code fortsetzen, indem diesem der gespeicherte Wert aus der `co_yield`-Anweisung zugeführt wird


## Der C++ *Coroutine Framework* 


### Ein *Generator*

Um im C++&ndash;Laufzeitsystem eine Coroutine zuverwalten, bedarf es eines so genannten
*Generator*-Objekts, das den Lebenszyklus einer Coroutine abbildet und verwaltet.

Informationen dieser Klasse sind in der `include`-Datei vorhanden:

```cpp
#include <coroutine>
```

Man könnte geneigt sein zu vermuten, dass in dieser Include-Datei eine Schnittstelle
oder eine abstrakte Klassendefinition abgelegt sind. In C++ 20 gibt es hierfür
`concepts`, die entsprechende Forderungen (`requires`) an eine Implementierung stellen.
Es müssen je nach Anwendungsfall eine Reihe von Methoden eines bestimmten Namens
und mit bestimmten Parametertypen definiert sein.

Jede C++&ndash;Anwenderklasse, die den Anspruch erhebt, eine *Generator*-Klasse zu sein,
muss einen Bezug zu einer vordefinerten Klasse eines bestimmten Namens aufweisen:

```cpp
promise_type
```

Genau betrachtet erzwingen C++20 Coroutinen,
dass wir zwei miteinander verbundene unterstützende Klassen bereitstellen:

  * eine Klasse zum Speichern des Coroutine-Zustands und zum Speichern der Daten, die via co_yield bereit gestellt werden.
    Diese Klasse wird in der Regel als *Promise* bezeichnet, daher auch der Typbezeichner `promise_type`.
  * eine Klasse zur Verwaltung des Coroutine-(Promise-)Objekts &ndash; das ist die *Generator*-Klasse.

Im *Promise*-Objekt sind die Lebenszyklusmethoden bereitstellen.


### Das *Promise*-Objekt

Wir führen zwei Methoden des *Promise*-Objekts exemplarisch auf:

Verwendet eine Coroutine eine `co_yield`-Anweisung mit einem `std::string`-Wert verwendet,
benötigen wir im *Promise*-Objekt eine Methode mit der folgenden Signatur:

```cpp
std::suspend_always yield_value(std::string value);

```

Der Parameter ist gewissermaßen das Objekt, das das Argument von `co_yield` darstellt.
Der Rückgabewert von `yield_value` teilt dem Laufzeitsystem mit, 
ob der Thread-Status gespeichert werden soll.

Die `yield_value`-Methode muss ihr Argument speichern,
damit es bei Anforderung den Wert an die aufrufende Routine (Consumer)
zurückgeben kann:

```cpp
std::suspend_always yield_value(std::string value) {
    m_value = std::move(value);
    return {};
}
```

Die `return`-Anweisung entspricht &ldquo;Modern C++&rdquo;:

`return {}` bedeutet einfach, ein Objekt des Rückgabetyps (Standard-Konstruktor)
für diese Methode zu erstellen. Man könnte auch

```cpp
return std::suspend_always{}
```

schreiben.

---

### Beispielquellcode: Das *Promise*-Objekt

```cpp
```

```cpp
```

### Beispielquellcode: Der Generator

```cpp
```

### Beispielquellcode: Die Coroutine

```cpp
```

### Beispielquellcode: Der Konsument


## Übung:

Wie müssen Sie den Quellcode für den &ldquo;Hello World&rdquo;-Generator umschreiben,
um die in der Einleitung skizzierte Idee eines &ldquo;Lazy&rdquo;-Generators für
aufsteigende, ganze Zahlen zu implementieren?


## Literaturhinweise:

Anregungen zu den Beispielen stammen zum großen Teil aus dem Buch

&ldquo;[C++20 Coroutines](https://blog.feabhas.com/2021/09/c20-coroutines/#Promise_data_holder)&rdquo;
von Martin Bond.


---

[Zurück](../../Readme.md)

---
