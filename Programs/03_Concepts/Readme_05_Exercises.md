# Aufgaben

[Zur�ck](Readme.md)

---

[L�sungen](Concepts_05_Exercises.cpp)

---

## Aufgabe 1: Konkatenation von Zeichenketten

#### Voraussetzungen: `std::initializer_list`

Schreiben Sie eine Funktionen `concat`, die eine beliebige Anzahl von `std::string`-Parametern miteinander verkn�pft.
Mit dem `concept`-Konzept soll erreicht werden,
dass die `concat`-Funktion ausschlie�lich mit Parametern des Typ `std::string` aufgerufen werden kann.
Dies schlie�t im Detail nat�rlich mit ein, dass `const` oder Referenz-Parameter ebenfalls erlaubt sind.

M�gliche Testbeispiele k�nnen so aussehen:

```cpp
std::string s1{ "111/" };
std::string& s2{ s1 };
const std::string s3{ "333/" };
const std::string& s4{ s3 };

std::string result{ concat(
    s1, 
    s2, 
    s3, 
    s4,
    std::string { "555/" },
    "7" + std::string { "7" } + "7"
) };
std::cout << result << std::endl;
```

*Ausgabe*:

```cpp
111/111/333/333/555/777
```

*Hinweis*: In der Implementierung der `concat`-Funktion kann ein `std::initializer_list`-Objekt behilflich sein!

---

[An den Anfang](#Aufgaben)

---

[Zur�ck](Readme.md)

---
