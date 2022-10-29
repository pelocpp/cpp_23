# Mehr Details

[Zurück](Readme.md)

---

[Quellcode](ConceptsMoreDetails.cpp)

---

## Konzept Definition (*Concept Definition*)

Ein &ldquo;Konzept&rdquo; bezieht sich auf ein Template, das wiederum eine Menge
benannter *Constraints* definiert, wobei jedes *Constraint* durch eine oder mehrere Requirements
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

## Konzept-Ausdruck (*Concept Expression*)


Ein *Concept Expression* wird verwendet, um zu überprüfen,
ob ein gegebener Typ ein Konzept modelliert.
Ein Konzept-Ausdruck besteht aus dem Namen eines zuvor definierten Konzepts,
gefolgt von einer Reihe von Template Argumenten zwischen spitzen Klammern.

Beispielsweise sind `Small<char>` und `Small<short>` Konzeptausdrücke,
die als `true` ausgewertet werden, `Small<double>` und `Small<long long>` dürften
im Regelfall als `false` ausgewertet werden.

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

###### *Syntax*:

<pre>
<b>requires</b> <b>{</b> <i>requirement-sequence</i> <b>}</b>
</pre>

oder

<pre>
<b>requires</b> <b>{</b> <i>parameter-list</i> <b>}</b> <b>{</b> <i>requirement-sequence</i> <b>}</b>
</pre>


Es gibt 4 unterschiedliche Requirements:

  * Einfaches Requirement (*Simple Requirement*)
  * Typ Requirement (*Type Requirement*)
  * Verbund-Requirement (*Compound Requirement*)
  * Geschachteltes Requirement (*Nested Requirement*)


##### Einfaches Requirement (*Simple Requirement*)

###### Beispiel:

```cpp
template<class T, class U>
concept Addable = requires (const T & t, const U & u) 
{
    t + u;
    u + t;
};
```

Eine einfaches Requirement ist nur ein C++-Ausdruck,
der durch ein Semikolon (;) abgeschlossen wird.


##### Typ Requirement (*Type Requirement*)


###### *Syntax*:

<pre>
<b>requires</b> <b>{</b> <b>typename</b> <i>name</i> <b>;</b> <b>}</b>
</pre>

oder

<pre>
<b>requires</b> <b>{</b> <i>parameter-list</i> <b>}</b> <b>{</b> <b>typename</b> <i>name</i> <b>;</b> <b>}</b>
</pre>


Das folgende Beispiel prüft, ob das Template Argument `T` einen
verschachtelten member Type namens `value_type` hat:


###### Beispiel:

```cpp
template<class T>
concept HasValueType = requires {
    typename T::value_type;
};
```


##### Verbund-Requirement (*Compound Requirement*)

Ähnlich wie einfache Requirements werden Verbund-Requirements verwendet,
um die Gültigkeit eines C++-Ausdrucks zu überprüfen.
Zusätzlich zu einfachen Requirements können Verbund-Requirements auch überprüfen,
ob das Ergebnis des Ausdrucks ein Typ Requirement erfüllt
oder ob der Ausdruck keine Ausnahme auslöst.

In einfachen Worten:

Mit einem Verbund-Requirement kann man den Rückgabetyp eines bestimmten Ausdrucks überprüfen.

###### *Syntax*:


<pre>
<b>requires</b> <b>{ {</b> <i>compound_requirement</i> <b>}; }</b>                             // Same as a simple requirement
<b>requires</b> <b>{ {</b> <i>compound_requirement</i> <b>} noexcept; }</b>                    // Compound-Requirement does not throw an exception
<b>requires</b> <b>{ {</b> <i>compound_requirement</i> <b>}</b> <b>-></b> <i>type-constraint</i><b>; }</b>          // Result of Compound-requirement satisfies type-constraint
<b>requires</b> <b>{ {</b> <i>compound_requirement</i> <b>}</b> <b>noexcept -></b> <i>type_constraint</i>; <b>}</b> // Result of Compound-requirement satisfies type-constraint and does not throw an exception
</pre>

und die Varianten mit einer Parameterliste:

<pre>
<b>requires</b> <b>(</b> <i>parameter_list</i> <b>)</b> <b>{ {</b> <i>compound_requirement</i> <b>}; }</b>                             // Same as a simple requirement
<b>requires</b> <b>(</b> <i>parameter_list</i> <b>)</b> <b>{ {</b> <i>compound_requirement</i> <b>} noexcept; }</b>                    // Compound-Requirement does not throw an exception
<b>requires</b> <b>(</b> <i>parameter_list</i> <b>)</b> <b>{ {</b> <i>compound_requirement</i> <b>}</b> <b>-></b> <i>type-constraint</i><b>; }</b>          // Result of Compound-requirement satisfies type-constraint
<b>requires</b> <b>(</b> <i>parameter_list</i> <b>)</b> <b>{ {</b> <i>compound_requirement</i> <b>}</b> <b>noexcept -></b> <i>type_constraint</i>; <b>}</b> // Result of Compound-requirement satisfies type-constraint and does not throw an exception
</pre>

###### Beispiel:

```cpp
template<class T, class U>
concept Same = std::is_same<T, U>::value;

template<class T>
concept EqualityComparable = requires (const T& a, const T& b) {
    { a == b } -> Same<bool>;
    { a != b } -> Same<bool>;
};
```

###### Beispiel:

```cpp
template <typename T>
concept HasSquare = requires (T t) {
    { t.square() } -> std::convertible_to<int>;
};
```

Man beachte:

  * Der Ausdruck, für den Sie ein Return Typ Requirement festlegen möchten,
  muss von geschweiften Klammern (`{` und `}`) umgeben sein, dann kommt ein Pfeil (`->`),
  gefolgt vom Constraint des Rückgabetyps.

  * Diese Einschränkung kann nicht einfach ein Typ sein, also zum Beispiel `int`.

Der letzte Punkt hat konzeptionelle Gründe. Also 
anstatt nur einen Typ zu benennen, muss man sich für ein Konzept entscheiden!
Möchte man einen Rückgabetyp festlegen, wird eine der beiden folgenden Optionen dieser
Anforderung gerecht:

```cpp
  {t.square()} -> std::same_as<int>;
  {t.square()} -> std::convertible_to<int>;
```

Im Falle von `std::same_as` muss der Rückgabewert derselbe sein wie im Template-Argument angegeben,
während bei `std::convertible_to` Konvertierungen erlaubt sind.

WEITER:  

https://www.sandordargo.com/blog/2021/03/17/write-your-own-cpp-concepts-part-ii

In order to demonstrate this, let’s have a look at the following example:

##### Geschachteltes Requirement (*Nested Requirement*)


###### *Syntax*:


<pre>
<b>requires</b> <b>{</b> <b>requires</b> <i>constraint_expression</i> <b>;</b> <b>}</b>
<b>requires</b> <b>(</b> <i>parameter-list</i> <b>)</b> <b>{</b> <b>requires</b> <i>constraint_expression</i> <b>;</b> <b>}</b>
</pre>


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
