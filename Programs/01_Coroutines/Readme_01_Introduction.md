# Coroutines

[Zur�ck](Readme.md)

---

[Quellcode](Coroutines_01_Introduction.cpp)

---

## �berblick

*Erforderliche Begriffe*:

  * *Kooperatives Multitasking* (auch als *nicht-pr�emptives Multitasking* bezeichnet) &ndash;
    Man spricht von *kooperativem Multitasking* wenn die beteiligten Prozesse (Threads)
    freiwillig die Kontrolle regelm��ig abgeben.
    Diese Art des Multitaskings erfordert zwingend, dass sich alle Teilnehmer an diese Regel halten!

  * *Pr�emptives Multitasking* &ndash; Das Gegenteil von *kooperativem Multitasking* ist das so genannte *pr�emptive Multitasking*.
    *Pr�emptives Multitasking* wird durch das Betriebssystem verwaltet bzw. organisiert. Ein *Scheduler* (oder auch als *Dispatcher* bezeichnet)
    bestimmt, wie lange ein Prozess (Thread) laufen darf. Ist die Zeit abgelaufen (Zeitscheibe), wird ein Kontextwechsel
    durchgef�hrt, also ein anderer Prozess (Thread) ausgew�hlt.

  * Unterprogramm &ndash; Anderer Begriff f�r eine &ldquo;regul�re&rdquo; Funktion (auch als *Prozedur* bezeichnet).

*Kurz und b�ndig*:

Coroutinen sind Funktionen, die das kooperative Multitasking erm�glichen, indem wiederholt die Ausf�hrung angehalten und fortgesetzt werden kann.
Coroutinen unterbrechen die Ausf�hrung, indem sie immer wieder zum Aufrufer zur�ckkehren und dabei auch Daten zur�ckliefern.
Daten, die zur Wiederaufnahme der Ausf�hrung erforderlich sind, werden getrennt von einem Stapel gespeichert.

Coroutinen eigen sich zur Implementierung von sequentiellem Code, der asynchron ausgef�hrt wird
(z. B. um nicht blockierende E / A ohne explizite R�ckrufe zu verarbeiten) und unterst�tzt auch Algorithmen f�r verz�gert berechnete unendliche Sequenzen und andere Verwendungen.

<img src="cpp_20_coroutine_vs_subroutine_02.svg" width="700">

*Abbildung* 1: Subroutines (links) und Coroutines (rechts) im Vergleich.

## Coroutinen in C++ 20

Per Definition wird in C++ 20 eine Funktion als *Coroutine* bezeichnet, wenn

  * sie das Schl�sselwort `co_yield` verwendet , um die Ausf�hrung anzuhalten und einen Wert zur�ckzugeben.
  * sie das Schl�sselwort `co_return` verwendet , um die Ausf�hrung abzuschlie�en.
  * sie den Operator `co_await` verwendet (*suspend*), um die Ausf�hrung bis zur Wiederaufnahme (*resume*) zu unterbrechen.

Ohne auf die drei Schl�sselw�rter explizit einzugehen, kann man auch sagen:

> Eine Coroutine ist eine Funktion, die sich selbst suspendieren kann.


## &ldquo;Don't implement coroutines yourself&rdquo;

Um es deutlich zu sagen: Der Standard C++ 20 enth�lt *keine* Coroutinen.
Er definiert statt dessen einen *Framework* f�r Coroutinen, mit dessen Hilfe sich Coroutinen implementieren lassen,
siehe hierzu das nachfolgende Beispiel einer Generatorklasse.

*Tipp*: In einer zuk�nftigen Version (C++ 20, C++ 23) wird es eine umfangreichere Unterst�tzung / exemplarische Realisierung
dieses Frameworks geben, so dass eine Entwicklung von Coroutinen f�r den Anwender einfacher ausf�llt.

## Ein Generator Template: `Generator<T>`

*Definition*:

Ein *Generator* stellt einen Datentyp f�r eine Coroutine dar, die eine Folge von Werten des Typs `T` erzeugt,
wobei die Werte *on-demand* (*lazy*) und synchron (im Kontext der Coroutine) erzeugt werden.

## Ein Beispiel f�r Fibonacci-Zahlen

*Beispiel*:

```cpp
Generator<long long> fibonacci() {
    long long a = 0;
    long long b = 1;
    while (true) {
        co_yield b;
        auto tmp = a;
        a = b;
        b += tmp;
    }
}
```

*Anwendung*:

Man beachte bei diesem Beispiel, dass es sich bei der Generator-Funktion um einen so genannten *unendlichen* Generator
handelt. Es obliegt folglich der Anwendung, f�r einen kontrollierten Abbruch zu sorgen. 
Nat�rlich k�nnte man einen Generator auch so implementieren, dass es nur endlich viele Werte erzeugt.

```cpp
for (auto i : fibonacci()) {
    if (i > 1'000'000)
        break;

    std::cout << i << std::endl;
}
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

https://www.heise.de/developer/artikel/C-20-Coroutinen-mit-cppcoro-4705161.html

von Rainer Grimm.

---

[Zur�ck](Readme.md)

---
