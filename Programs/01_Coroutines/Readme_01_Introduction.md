# Einführung

[Zurück](Readme.md)

---

[Quellcode](Coroutines_01_Introduction.cpp)

---

Ganz kurz und prägant:

> Eine Coroutine ist eine Funktion, die sich selbst suspendieren kann.

---

## Motivation

Zur Motivation von Coroutinen betrachten wir eine Funktion `getNumbers`:

```cpp
01: std::vector<int> getNumbers(int begin, int end)
02: {
03:     std::vector<int> numbers;
04: 
05:     for (int i = begin; i <= end; ++i) {
06:         numbers.push_back(i);
07:     }
08: 
09:     return numbers;
10: }
```

Folgende Beobachtungen sind wichtig:

  * Ein Aufrufer von `getNumbers` bekommt immer alle Werte, die er durch die beiden Parameter 
    `begin` und `end` anfordert. Dies wirkt sich negativ bei vielen Zahlen auf den benötigten Speicherplatz aus.

  * Darüberhinaus kann es sein, dass ein Aufrufer nach dem Auswerten der ersten 5 Zahlen
    möglicherweise an den restlichen Zahlen überhaupt nicht mehr interessiert ist.
    Bei einem Wert `end` gleich 1000 wurden also fast alle angeforderten Werte umsonst berechnet und transportiert.

Man spicht in der Informatik bei derartigen Berechnungen immer in der Vorgehensweise &ldquo;greedy&rdquo; oder &ldquo;lazy&rdquo;.
Die Funktion `getNumbers` fällt offensichtlich in die erste Kategorie.
Mit Hilfe von Coroutinen könnten Sie eine Variation von  `getNumbers ` auf Basis der &ldquo;lazy&rdquo;-Strategie
umsetzen.

```cpp
01: Generator generatorForNumbers(int begin, int end)
02: {
03:     for (int i = begin; i <= end; ++i) {
04:         co_yield i;
05:     }
06: }
```

Eine Anwendung könnte nun so aussehen:

```cpp
01: Generator coroutine = generatorForNumbers(1, 10);
02: 
03: while (true) {
04: 
05:     int value = coroutine.next();
06:     if (value == -1) {
07:         break;
08:     }
09: }
```

Diese beiden Code-Fragmente sind nicht unmittelbar übersetzungsfähig.
Wir nehmen sie als Ausgangspunkt unserer Betrachtungen der ersten Schritte im
Umfeld von C++&ndash;Coroutinen.

---

## Etwas Theorie

*Erforderliche Begriffe*:

  * *Kooperatives Multitasking* (auch als *nicht-präemptives Multitasking* bezeichnet) &ndash;
    Man spricht von *kooperativem Multitasking* wenn die beteiligten Prozesse (Threads)
    freiwillig die Kontrolle regelmäßig abgeben.
    Diese Art des Multitaskings erfordert zwingend, dass sich alle Teilnehmer an diese Regel halten!

  * *Präemptives Multitasking* &ndash; Das Gegenteil von *kooperativem Multitasking* ist das so genannte *präemptive Multitasking*.
    *Präemptives Multitasking* wird durch das Betriebssystem verwaltet bzw. organisiert. Ein *Scheduler* (oder auch als *Dispatcher* bezeichnet)
    bestimmt, wie lange ein Prozess (Thread) laufen darf. Ist die Zeit abgelaufen (Zeitscheibe), wird ein Kontextwechsel
    durchgeführt, also ein anderer Prozess (Thread) ausgewählt.

  * Unterprogramm &ndash; Anderer Begriff für eine &ldquo;reguläre&rdquo; Funktion (auch als *Prozedur* bezeichnet).

*Kurz und bündig*:

Coroutinen sind Funktionen, die das kooperative Multitasking ermöglichen, indem wiederholt die Ausführung angehalten und fortgesetzt werden kann.
Coroutinen unterbrechen die Ausführung, indem sie immer wieder zum Aufrufer zurückkehren und dabei auch Daten zurückliefern.
Daten, die zur Wiederaufnahme der Ausführung erforderlich sind, werden getrennt von einem Stapel gespeichert.

Coroutinen eigen sich zur Implementierung von sequentiellem Code, der asynchron ausgeführt wird
(z. B. um nicht blockierende E / A ohne explizite Rückrufe zu verarbeiten) und unterstützt auch Algorithmen für verzögert berechnete unendliche Sequenzen und andere Verwendungen.

<img src="cpp_20_coroutine_vs_subroutine_02.svg" width="600">

*Abbildung* 1: Subroutines (links) und Coroutines (rechts) im Vergleich.

*Man beachte*: 
Nicht zwingend müssen Aufrufer der Coroutine und die Coroutine selbst in verschiedenen Threads ablaufen:
Coroutinen (und ihr Aufrufer) können sehr wohl auf einem einzigen Thread ausgeführt werden.
Siehe dazu *Abbildung* 2, die bewusst keine Aussagen über Threadaffinitäten trifft:

<img src="consumer_producer_coroutine.png" width="700">

*Abbildung* 2: Kontrollfluss zwischen einem Produzenten (Coroutine) und einem Konsumenten (Anwendung).

---

## Coroutinen: *stackless* und *stackful*

Coroutinen existieren in den verschiedenene Programmierumgebungen prinzipiell
in zwei Ausprägungen:

  * *stackless*
  * *stackful*

*Stackful* Coroutinen haben einen separaten Stack (ähnlich einem Thread), der den
so genannten *Coroutine-Frame* und die verschachtelten Aufruf-Frames enthält.
Dadurch ist es möglich, an einer beliebigen Stelle im verschachtelten Aufruf-Frame
zu unterbrechen (*suspend*) und wieder fortzufahren (*resume*).

*Stackless* Coroutinen müssen den Coroutine-Frame an einer anderen Stelle speichern
(normalerweise auf dem Heap) und verwenden den Stapel des *aktuell* ausgeführten Threads,
um verschachtelte Aufrufe durchführen zu können.

<img src="C20_Coroutine_Stackless_Stackful.png" width="500">

*Abbildung* 1: Funktionen versus Coroutinen &ndash; *stackless* versus *stackful* Coroutinen.

In C++ 20 finden wir eine Unterstützung für *stackless* Coroutinen vor.

---

## &ldquo;Don't implement coroutines yourself&rdquo;

Um es deutlich zu sagen: Der Standard C++ 20 enthält *keine* Coroutinen.
Er definiert statt dessen einen *Framework* für Coroutinen, mit dessen Hilfe sich Coroutinen implementieren lassen,
siehe hierzu das nachfolgende Beispiel einer Generatorklasse.

*Tipp*: In einer zukünftigen Version (C++ 20, C++ 23) wird es eine umfangreichere Unterstützung / exemplarische Realisierung
dieses Frameworks geben, so dass eine Entwicklung von Coroutinen für den Anwender einfacher ausfällt.

---

## Coroutinen in C++ 20

Per Definition wird in C++ 20 eine Funktion als *Coroutine* bezeichnet, wenn

  * sie das Schlüsselwort `co_yield` verwendet , um die Ausführung anzuhalten und einen Wert zurückzugeben.
  * sie das Schlüsselwort `co_return` verwendet , um die Ausführung abzuschließen.
  * sie den Operator `co_await` verwendet (*suspend*), um die Ausführung bis zur Wiederaufnahme (*resume*) zu unterbrechen.

---

## Ein Generator Template: `Generator<T>`

*Definition*:

Ein *Generator* stellt einen Datentyp für eine Coroutine dar, die eine Folge von Werten des Typs `T` erzeugt,
wobei die Werte *on-demand* (*lazy*) und synchron (im Kontext der Coroutine) erzeugt werden.

Bis zur Version 20 musste man diese Klasse selbst schreiben bzw. aus einer geeigneten Vorlage entnehmen und hinzufügen.

Eine mögliche Vorlage findet man [hier](https://en.cppreference.com/w/cpp/coroutine/coroutine_handle.html).

Ab C++ 23 findet man eine Vorlage in der C++ Standardklassenbibliothek vor.
Dazu muss man nur noch die Dateien `<generator>` und `<coroutine>` inkludieren.

Da es mich bei meinen ersten Schritten mit Coroutinen viel arbeitet gekostet hatte,
eine Generator-Klasse selbst zu schreiben, will ich an dieser Stelle zumindest ihren Quellcode aufheben.
Aber um es noch einmal zu verdeutlichen: Es dient nur noch illustrativen Zwecken!

```cpp
001: template<std::movable T>
002: class Generator {
003: public:
004:     struct promise_type {
005:         Generator<T> get_return_object() {
006:             return Generator{ Handle::from_promise(*this) };
007:         }
008:         static std::suspend_always initial_suspend() noexcept {
009:             return {};
010:         }
011:         static std::suspend_always final_suspend() noexcept {
012:             return {};
013:         }
014:         std::suspend_always yield_value(T value) noexcept {
015:             current_value = std::move(value);
016:             return {};
017:         }
018:         void return_void() {}
019: 
020:         // Disallow co_await in generator coroutines.
021:         void await_transform() = delete;
022:         [[noreturn]]
023:         static void unhandled_exception() {
024:             throw;
025:         }
026: 
027:         std::optional<T> current_value;
028:     };
029: 
030:     using Handle = std::coroutine_handle<promise_type>;
031: 
032:     explicit Generator(const Handle coroutine) :
033:         m_coroutine{ coroutine }
034:     {}
035: 
036:     Generator() = default;
037:     ~Generator() {
038:         if (m_coroutine) {
039:             m_coroutine.destroy();
040:         }
041:     }
042: 
043:     Generator(const Generator&) = delete;
044:     Generator& operator=(const Generator&) = delete;
045: 
046:     Generator(Generator&& other) noexcept :
047:         m_coroutine{ other.m_coroutine }
048:     {
049:         other.m_coroutine = {};
050:     }
051:     Generator& operator=(Generator&& other) noexcept {
052:         if (this != &other) {
053:             if (m_coroutine) {
054:                 m_coroutine.destroy();
055:             }
056:             m_coroutine = other.m_coroutine;
057:             other.m_coroutine = {};
058:         }
059:         return *this;
060:     }
061: 
062:     // Range-based for loop support.
063:     class Iter {
064:     public:
065:         void operator++() {
066:             m_coroutine.resume();
067:         }
068:         const T& operator*() const {
069:             return *m_coroutine.promise().current_value;
070:         }
071:         bool operator==(std::default_sentinel_t) const {
072:             return !m_coroutine || m_coroutine.done();
073:         }
074: 
075:         explicit Iter(const Handle coroutine) :
076:             m_coroutine{ coroutine }
077:         {}
078: 
079:     private:
080:         Handle m_coroutine;
081:     };
082: 
083:     Iter begin() {
084:         if (m_coroutine) {
085:             m_coroutine.resume();
086:         }
087:         return Iter{ m_coroutine };
088:     }
089:     std::default_sentinel_t end() {
090:         return {};
091:     }
092: 
093: private:
094:     Handle m_coroutine;
095: };
096: 
097: template<std::integral T>
098: Generator<T> range(T first, const T last) {
099:     while (first < last) {
100:         co_yield first++;
101:     }
102: }
```

---

## Ein &bdquo;Hello World&rdquo;-Beispiel

*Beispiel*:

```cpp
01: std::generator<char> hello() {
02:     co_yield 'h';
03:     co_yield 'e';
04:     co_yield 'l';
05:     co_yield 'l';
06:     co_yield 'o';
07:     co_return;
08: }
```

*Aufruf*:


```cpp
01: void simple_coroutine() {
02: 
03:     for (auto ch : hello()) {
04:         std::println("{}", ch);
05:     }
06: }
```

*Ausgabe*:

```
h
e
l
l
o
```


---

## Ein Beispiel für Fibonacci-Zahlen

*Beispiel*:

```cpp
01: std::generator<long long> fibonacci() {
02:     long long a = 0;
03:     long long b = 1;
04:     while (true) {
05:         co_yield b;
06:         auto tmp = a;
07:         a = b;
08:         b += tmp;
09:     }
10: }
```

Der Rückgabetyp der Generator-Funktion `fibonacci` muss den *Coroutine*-Anforderungen entsprechen.

Ferner muss die Coroutine mindestens ein Coroutine-Schlüsselwort verwenden, in unserem Beispiel ist es `co_yield`.


*Anwendung*:

Man beachte bei diesem Beispiel, dass es sich bei der Generator-Funktion `fibonacci` um einen so genannten *unendlichen* Generator
handelt. Es obliegt folglich der Anwendung, für einen kontrollierten Abbruch zu sorgen. 
Natürlich könnte man einen Generator auch so implementieren, dass es nur endlich viele Werte erzeugt.

```cpp
01: void simple_coroutine() {
02: 
03:     for (auto i : fibonacci()) {
04:         if (i > 1'000'000)
05:             break;
06: 
07:         std::cout << i << std::endl;
08:     }
09: 
10:     std::cout << std::endl;
11: }
```

*Ausgabe*:

```
1
1
2
3
5
8
13
21
34
55
89
144
233
377
610
987
1597
2584
4181
6765
10946
17711
28657
46368
75025
121393
196418
317811
514229
832040
```

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen teilweise bzw. in modifizierter Form aus
[C++20: Coroutinen mit cppcoro](https://www.heise.de/developer/artikel/C-20-Coroutinen-mit-cppcoro-4705161.html)
von Rainer Grimm.

---

[Zurück](Readme.md)

---
