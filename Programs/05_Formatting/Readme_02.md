# The Return of &bdquo;`printf`&rdquo;: `std::print` in C++ 23

[Zur�ck](../../Readme.md)

---

[Quellcode](Formatting.cpp)

---

## �berblick

&bdquo;`printf`&rdquo;: Wer hat sie nicht vermisst? Die C-Bibliotheksfunktion war uns &ndash; trotz all ihrer Schw�chen &ndash; 
doch sehr ans Herz gewachsen.

<img src="StdPrintln_02.png" width="400">

*Abbildung* 1: The Return of &bdquo;`printf`&rdquo;: `std::print` in C++ 23.

In C++ 23 ist sie zur�ckgekehrt, und wenn wir so wollen: St�rker denn je zuvor!
Typsicherheit, Erweiterbarkeit, Unterst�tzung von Unicode und Performanz sind 
einige der St�rken der neuen Funktionen `std::print` und  `std::println`,
die gro�e �hnlichkeiten zu `printf` aufweisen.

---

## Dokumentation

Eine genaue Beschreibung der Formatzeichenkette findet man in der
&bdquo;[Standard Format Specification](https://en.cppreference.com/w/cpp/utility/format/spec)&rdquo; wieder.

Aus diesem Grund macht es wenig Sinn, hier die Spezifikation zu wiederholen.

Ich versuche, mit einer Reihe m�glichst ausdrucksstarker Beispiele die wesentlichen Features der `std::print(ln)`
Funktion vorzustellen.

---

## Beispiele

---

## Weiter



##### Verwendung einfacher Variablenersetzungen

`std::format` ersetzt das `{}` durch entsprechende Variablenwerte.

*Beispiel*:

```cpp
std::string result{ std::format("Format arguments: {}, {}, {}.", 1, 2, 3) };
std::cout << result << std::endl;
```

*Ausgabe*:

```
Format arguments: 1, 2, 3.
```

##### Reihenfolge der Argumente definieren

`std::format` ersetzt das `{}` durch die entsprechenden Variablenwerte

`{}` kann eine Zahl enthalten, die `std::format` die Nummer des Arguments mitteilt,
das an dieser Position beim Formatieren der Ausgabe verwendet werden soll.

*Beispiel*:

```cpp
std::string_view one{ "One" };
std::string_view two{ "Two" };
std::string_view three{ "Three" };

std::string result = std::format("{2}, {1}, {0}", one, two, three);
std::cout << result << std::endl;
```

*Ausgabe*:

```
Three, Two, One
```


##### Ganze Zahlen in unterschiedlichen Formaten

*Beispiel*:

```cpp
std::cout << std::format("Decimal:     {} {:d} {:06} {:06d} {:0} {:+} {:+}", 1, 2, 3, 4, 5, 6, -7);
std::cout << std::format("Hexadecimal: {:x} {:x} {:X} {:#x}", 5, 10, 10, 5);
std::cout << std::format("Octal:       {:o} {:#o} {:#o}", 12, 12, 4);
std::cout << std::format("Binary:      {:b} {:#b} {:#B}", 15, 15, 5);
```

*Ausgabe*:

```
Decimal:     1 2 000003 000004 5 +6 -7
Hexadecimal: 5 a A 0x5
Octal:       14 014 04
Binary:      1111 0b1111 0B101
```

Die erste Zahl wird ohne irgendwelche Formatangaben ausgegeben.
Die gleiche Ausgabe wird erzeugt, wenn wir eine Dezimalzahl mit der Formatangabe `:d` spezifizieren.
Die dritte Zahl wird (mindestens) 6 Zeichen breit ausgegeben und mit f�hrenden Nullen aufgef�llt.
Der Spezifizierer `+` erm�glicht es, die Ausgabe des Vorzeichens f�r eine Zahl zu erzwingen.

Die zweite Zeile zeigt, dass wir Werte hexadezimal darstellen k�nnen &ndash; sowohl mit Klein- als auch mit Gro�buchstaben
f�r die Ziffern gr��er als 9. Der Spezifizierer `#` erzeugt das in hexadezimalen Literalen verwendete Pr�fix `0x`.

Ebenso k�nnen wir die Werte oktal und bin�r ausgeben &ndash; optional mit entsprechenden Literalpr�fixen.


##### Flie�kommazahlen

*Beispiel*:

```cpp
std::cout << std::format("Default:     {} {:} {:g} {:g}", 1.5, 1.5, 1.5, 1e20);
std::cout << std::format("Rounding:    {:f} {:.0f} {:.20f}", 1.5, 1.5, 1.5);
std::cout << std::format("Padding:     {:05.2f} {:.2f} {:5.2f}", 1.5, 1.5, 1.5);
std::cout << std::format("Scientific:  {:E} {:e}", 1.5, 1.5);
```

*Ausgabe*:

```
Default:     1.5 1.5 1.5 1e+20
Rounding:    1.500000 2 1.50000000000000000000
Padding:     01.50 1.50  1.50
Scientific:  1.500000E+00 1.500000e+00
```

Mit leeren Klammern oder nur mit einem Doppelpunkt erhalten wir die Standardausgabe.
Dies entspricht der Formatangabe `:g`.
Die Anzahl der Nachkommastellen kann zwischen einem Punkt und dem Formatbezeichner `f` angegeben werden.
Dann wird der Wert auf diese Genauigkeit gerundet.
Wenn die angeforderte Zahl gr��er ist als das, was durch den Typ des Wertes darstellbar ist,
sind die letzten Ziffern nicht sonderlich sinnvoll.

Eine Ziffer vor dem Punkt
gibt die (minimale) Breite der Ausgabe an. Wie bei ganzen Zahlen k�nnen wir f�hrende Nullen verlangen.
Gleitkommazahlen k�nnen mit den Bezeichnern `e` und `E` in der wissenschaftlichen Notation ausgegeben werden,
wobei der Exponentialteil entsprechend mit einem Gro�- oder Kleinbuchstaben beginnt.

---

## Literaturhinweise:

Die Anregungen zu den Beispielen stammen teilweise bzw. in modifizierter Form aus

[`std::format` in C++ 20](https://www.heise.de/developer/artikel/std-format-in-C-20-4919132.html)

von Rainer Grimm und Peter Gottschling.

---

[Zur�ck](../../Readme.md)

---
