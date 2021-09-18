# Aufgaben

[Zur�ck](Readme.md)

---

[L�sungen](ConceptsExercises.cpp)

---

## Aufgabe 1: Konkatenation von Zeichenketten

#### Voraussetzungen: `std::initializer_list`

Schreiben Sie eine Funktionen `concat`, die eine beliebige Anzahl von `std::string`-Parametern
miteinander verkn�pft. Mit dem `concept`-Konzept soll erreicht werden,
dass die `concat`-Funktion ausschlie�lich mit Parametern des Typ `std::string` aufgerufen werden d�rfen.
Dies schlie�t nat�rlich das Detail mit ein, dass `const` oder Referenz-Parameter ebenfalls g�ltig sind.

M�gliche Testbeispiele k�nnen so aussehen:

```cpp
```

*Ausgabe*:

```cpp
false
true
```

*Hinweis*: In der Implementierung der `concat`-Funktion kann ein `std::initializer_list`-Objekt behilflich sein!

---

[An den Anfang](#Aufgaben)

---

[Zur�ck](Readme.md)

---
