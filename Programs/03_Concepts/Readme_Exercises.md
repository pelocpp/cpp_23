# Aufgaben

[Zurück](Readme.md)

---

[Lösungen](ConceptsExercises.cpp)

---

## Aufgabe 1: Konkatenation von Zeichenketten

#### Voraussetzungen: `std::initializer_list`

Schreiben Sie eine Funktionen `concat`, die eine beliebige Anzahl von `std::string`-Parametern
miteinander verknüpft. Mit dem `concept`-Konzept soll erreicht werden,
dass die `concat`-Funktion ausschließlich mit Parametern des Typ `std::string` aufgerufen werden dürfen.
Dies schließt natürlich das Detail mit ein, dass `const` oder Referenz-Parameter ebenfalls gültig sind.

Mögliche Testbeispiele können so aussehen:

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

[Zurück](Readme.md)

---
