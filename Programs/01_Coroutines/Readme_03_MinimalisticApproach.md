# Erste Schritte

[Zur�ck](Readme.md)

---

[Quellcode](Coroutines03_MinimalisticApproach.cpp)

---

## Eine minimalistische Betrachtung

Im Prinzip werden C++ Coroutinen durch einen so genannten *Coroutinen Framework* gebildet,
der erg�nzend zum Quellcode des Anwenders hinzugef�gt wird.

Die konkrete Gestaltung des generierten Quellcodes h�ngt von
benutzerdefinierten R�ckgabetypen und von einem *Promise*-Datentyp ab.

Bis auf Weiteres &ndash; das k�nnten Spracherweiterungen von C++&ndash;23 sein  &ndash; 
m�ssen wir diesen oftmals als &ldquo;Boilerplatecode&rdquo; bezeichneten Quellcode selber schreiben.

Ohne ein Verst�ndnis dieses &ldquo;Boilerplatecode&rdquo; Quellcodes geht es also nicht,
wir n�hern uns dem *Coroutinen Framework* in langsamen Schritten.

## Eine Coroutine, die nichts tut

Werfen wir einen Blick auf die k�rzestm�gliche Coroutine,
die wir zum jetzigen Zeitpunkt (C++ Sprachstandard Version C++&ndash;20 &ndash; Dezember 2020)
schreiben k�nnen:

<pre>
01: struct Task {
02: 
03:     struct promise_type {
04:         Task get_return_object() { return {}; }
05:         std::suspend_never initial_suspend() { return {}; }
06:         std::suspend_never final_suspend() noexcept { return {}; }
07:         void return_void() { }
08:         void unhandled_exception() { }
09:     };
10: };
11: 
12: Task myCoroutine() {
13:     co_return; // make it a coroutine
14: }
15: 
16: void test_01()
17: {
18:     Task x = myCoroutine();
19: }
</pre>

So sieht eine minimalistische Coroutine aus,
die sofort zur�ckkehrt und nichts tut.

Sie ist aber ein ausgezeichneter Ausgangspunkt,
um sich den vom Compiler generierten Framework-Code anzusehen:

BILD 

Neben den Erl�uterungen von Abbildung 1 sehen wir uns das minimalistische Coroutinenbeispiel
mit instrumentierten Testausgaben an.
Die Ausgabe sieht so aus:

<pre>
before coroutine call
  c'tor promise
  get_return_object
c'tor Task
  initial_suspend
inside coroutine
  return_void
  final_suspend
  ~promise
after coroutine call
~Task
</pre>

Wir erkennen:

> Bevor der Konstruktor von `Task` zur Ausf�hrung gelangt, wurde bereits eine Instanz
von der inneren Klasse `struct promise_type` angelegt!


## `std::suspend_never` versus `std::suspend_always`

WEITER: 

`std::suspend_never` versus `std::suspend_always` ufzeigen

Memory Leaks aufzeigen

L�sung aufzeigen

Der Generator

Awaitable



---


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
