# Mehr Details

[Zurück](Readme.md)

---

[Quellcode](XXX.cpp)

---

## Concept Definition

Ein Concept bezieht sich auf ein Tempalte für einen benannten Satz
von Constraints, wobei jedes Constraint durch eine oder mehrere Requirements
für die Template-Parameter definiert wird:


<pre>
<b>template</b> <b>&lt;</b> <i>template_parameter_list</i> <b>&gt;</b>
<b>concept</b> concept_name
<b>=</b> constraint_expression <b>;</b>
</pre>


###### Beispiel:

```cpp
template <typename T>
concept Small = sizeof(T) <= sizeof(int);
```

---

## Konpept-Ausdruck (*Concept Expression*)


Ein Concept Expression* wird verwendet, um zu überprüfen,
ob ein gegebener Typ ein Konzept modelliert.
Ein Konzept-Ausdruck besteht aus dem Namen eines zuvor definierten Konzepts,
gefolgt von einer Reihe von Template Argumenten zwischen spitzen Klammern.

Beispielsweise sind `Small<char>` und `Small<short>` Konzeptausdrücke,
die als `true` ausgewertet werden, aber `Small<double>` und `Small<long long>`
werden im Allgemeinen als `false` ausgewertet.

###### Beispiel:

```cpp
static_assert(Small<char> == true);
static_assert(Small<short> == true);
static_assert(Small<int> == true);

static_assert(not Small<double> == true);
static_assert(not Small<long double> == true);
static_assert(not Small<long long> == true);
```


Konzept-Ausdrucke kommen ebenfalls zum Zuge,
um Konzepte in Bezug auf zuvor definierte benannte Konzepte zu definieren:


###### Beispiel:

```cpp
template <class T>
concept Integral = std::is_integral_v<T>;

template <class T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;

template <class T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;
```


Obwohl Sie mit der Definition von Konzepten mithilfe von Konzept-Ausdrücken
ziemlich weit kommen können, stellen Sie möglicherweise irgendwann fest,
dass es keinen Konzeptausdruck gibt, der die Anforderungen des Typs testet,
den Sie benötigen.

In diesem Fall können Sie einen *Requires Expression* verwenden.


---

## Requires Ausdruck (*Requires Expression*)

Es gibt 4 unterschiedliche Requirements:

  * Einfaches Requirement (*Simple Requirement*)
  * Typ Requirement (*Type Requirement*)
  * Verbund-Requirement (*Compound Requirement*)
  * Geschachteltes Requirement (*Nested Requirement*)


##### Einfaches Requirement (*Simple Requirement*)

##### Typ Requirement (*Type Requirement*)

##### Verbund-Requirement (*Compound Requirement*)

##### Geschachteltes Requirement (*Nested Requirement*)

---

## Requires Anweisung (*Requires Clause*)



---

## Constrained Class Templates


---


###### Beispiel:

```cpp
auto values = std::vector{ 9, 2, 5, 3, 4 };
std::sort(std::begin(values), std::end(values));
```

---



## Literaturhinweise:

Die Anregungen zu den Beispielen stammen aus

[C++20: Concepts, an Introduction](https://oopscenities.net/2020/09/29/c20-concept/)

---

[Zurück](Readme.md)

---
