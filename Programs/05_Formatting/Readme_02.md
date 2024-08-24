# The Return of &bdquo;`printf`&rdquo;: `std::print` in C++ 23

[Zur�ck](../../Readme.md)

---

[Quellcode](Formatting.cpp)

---

## �berblick

&bdquo;`printf`&rdquo;: Wer hat sie nicht vermisst? Die C-Bibliotheksfunktion war uns &ndash; trotz all ihrer Schw�chen &ndash; 
doch sehr ans Herz gewachsen.

<img src="StdPrintln_02.png" width="400">

*Abbildung* 1: The Return of &bdquo;`printf`&rdquo;: `std::print` ab C++ 23.

In C++ 23 ist sie zur�ckgekehrt, und wenn wir so wollen: St�rker denn je zuvor!
Typsicherheit, Erweiterbarkeit, Unterst�tzung von Unicode und Performanz sind 
einige der St�rken der neuen Funktionen `std::print` und  `std::println`.
Dar�ber hinaus weisen sie gro�e �hnlichkeiten zu `printf` auf!

---

## Dokumentation

Eine genaue Beschreibung der Formatzeichenkette findet man in der
&bdquo;[Standard Format Specification](https://en.cppreference.com/w/cpp/utility/format/spec)&rdquo; wieder.

Aus diesem Grund macht es wenig Sinn, hier die Spezifikation zu wiederholen.

Ich versuche, mit einer Reihe m�glichst ausdrucksstarker Beispiele die wesentlichen Features der `std::print(ln)`
Funktion vorzustellen.

---

## Beispiele


##### Verwendung einfacher Variablenersetzungen


*Beispiel*:

```cpp
XXX
```

*Ausgabe*:

```
AAA
```


---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen teilweise bzw. in modifizierter Form aus

[`std::format` in C++ 20](https://www.heise.de/developer/artikel/std-format-in-C-20-4919132.html)

von Rainer Grimm und Peter Gottschling.

---

[Zur�ck](../../Readme.md)

---
