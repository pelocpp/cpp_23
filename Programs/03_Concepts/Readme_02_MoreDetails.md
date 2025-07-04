# Mehr Details

[Zur�ck](Readme.md)

---

[Quellcode](Concepts_02_MoreDetails.cpp)

---

## Konzept Definition (*Concept Definition*)

Ein &ldquo;Konzept&rdquo; bezieht sich auf ein Template, das wiederum eine Menge
benannter *Constraints* definiert, wobei jedes *Constraint* durch eine oder mehrere Requirements
f�r die Template-Parameter definiert wird:

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


Ein *Concept Expression* wird verwendet, um zu �berpr�fen,
ob ein gegebener Typ ein Konzept modelliert.
Ein Konzept-Ausdruck besteht aus dem Namen eines zuvor definierten Konzepts,
gefolgt von einer Reihe von Template Argumenten zwischen spitzen Klammern.

Beispielsweise sind `Small<char>` und `Small<short>` Konzeptausdr�cke,
die als `true` ausgewertet werden, `Small<double>` und `Small<long long>` d�rften
im Regelfall als `false` ausgewertet werden.


<img src="cpp_20_concept_syntax.svg" width="700" />

*Bild* 1: Syntax-Diagramm f�r Konzept




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


Obwohl Sie mit der Definition von Konzepten mithilfe von Konzept-Ausdr�cken
ziemlich weit kommen k�nnen, stellen Sie m�glicherweise irgendwann fest,
dass es keinen Konzeptausdruck gibt, der die Anforderungen des Typs testet,
den Sie ben�tigen.

In diesem Fall k�nnen Sie *Requires Expressions* verwenden.


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


Es gibt 4 unterschiedliche Requires Ausdr�cke:

  * Einfacher Requires Ausdruck (*Simple Requirement*)
  * Typ Requirement (*Type Requirement*)
  * Verbund Requirement (*Compound Requirement*)
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

Mit Typ Requirements k�nnen wir fordern,
dass ein bestimmter Typ in einem bestimmten Kontext g�ltig ist.
Typ Requirements k�nnen verwendet werden, um zu �berpr�fen, dass

  * ein bestimmter verschachtelter Typ existiert
  * eine Klassen Template Spezialisierung einen Typ benennt
  * eine Alias-Template-Spezialisierung einen Typ benennt


###### *Syntax*:

<pre>
<b>requires</b> <b>{</b> <b>typename</b> <i>name</i> <b>;</b> <b>}</b>
</pre>

oder

<pre>
<b>requires</b> <b>(</b> <i>parameter-list</i> <b>)</b> <b>{</b> <b>typename</b> <i>name</i> <b>;</b> <b>}</b>
</pre>


Das folgende Beispiel pr�ft, ob das Template Argument `T` einen
verschachtelten member Type namens `value_type` hat:


###### Beispiel:

```cpp
template<class T>
concept HasValueType = requires {
    typename T::value_type;
};
```


##### Verbund-Requirement (*Compound Requirement*)

�hnlich wie einfache Requirements werden Verbund-Requirements verwendet,
um die G�ltigkeit eines C++-Ausdrucks zu �berpr�fen.
Zus�tzlich zu einfachen Requirements k�nnen Verbund-Requirements auch �berpr�fen,
ob das Ergebnis des Ausdrucks ein Typ Requirement erf�llt
oder ob der Ausdruck keine Ausnahme ausl�st.

In einfachen Worten:

Mit einem Verbund-Requirement kann man den R�ckgabetyp eines bestimmten Ausdrucks �berpr�fen.

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

  * Der Ausdruck, f�r den Sie ein Return Typ Requirement festlegen m�chten,
  muss von geschweiften Klammern (`{` und `}`) umgeben sein, dann kommt ein Pfeil (`->`),
  gefolgt vom Constraint des R�ckgabetyps.

  * Diese Einschr�nkung kann nicht einfach ein Typ sein, also zum Beispiel `int`.

Der letzte Punkt hat konzeptionelle Gr�nde. Also 
anstatt nur einen Typ zu benennen, muss man sich f�r ein Konzept entscheiden!
M�chte man einen R�ckgabetyp festlegen, wird eine der beiden folgenden Optionen dieser
Anforderung gerecht:

```cpp
  {t.square()} -> std::same_as<int>;
  {t.square()} -> std::convertible_to<int>;
```

Im Falle von `std::same_as` muss der R�ckgabewert derselbe sein wie im Template-Argument angegeben,
w�hrend bei `std::convertible_to` Konvertierungen erlaubt sind.


##### Geschachteltes Requirement (*Nested Requirement*)

Ein geschachteltes Requirement wird verwendet, um ein *Constraint Expression*
innerhalb eines �bergeordneten *Requires Expressions* zu testen.
Der *Concept Expression* einer verschachtelten Anforderung kann lokale Parameter verwenden,
die in einem der �bergeordneten �bergeordneten *Requires Expressions* eingef�hrt wurden.


###### *Syntax*:

<pre>
<b>requires</b> <b>{</b> <b>requires</b> <i>constraint_expression</i> <b>;</b> <b>}</b>
<b>requires</b> <b>(</b> <i>parameter_list</i> <b>)</b> <b>{</b> <b>requires</b> <i>constraint_expression</i> <b>;</b> <b>}</b>
</pre>

###### Beispiel:


```cpp
template<typename TCar>
concept Car = requires (TCar car) {
    car.startEngine();
};

template<typename TCar>
concept Convertible = Car<TCar> && requires (TCar car) {
    car.openRoof();
};

// nexted Concept
template<typename TCar>
concept Coupe = Car<TCar> && requires (TCar car) {
    requires !Convertible<TCar>;
};
```

---

## Requires Klausel (*Requires Clause*)


Die *Requires-Klausel* kann syntaktisch an drei Stellen erscheinen:

A. Als so genannte *Requires-Klausel* im Kopf einer Template Definition:

<pre>
<b>template</b> <b>&lt;</b> <b>typename</b> <i>T</i> <b>&gt;</b>
<b>requires</b> <i>Your_Requirement_Or_Concept</i> <b>&lt;</b> <i>T</i> <b>&gt;</b>
<b>void</b> <i>func();</i>
</pre>


B. Als so genannte *trailing Requires-Klausel*:

<pre>
<b>template</b> <b>&lt;</b> <b>typename</b> <i>T</i> <b>&gt;</b>
<b>void</b> <i>func()</i> <b>requires</b> <i>Your_Requirement_Or_Concept</i> <b>&lt;</b> <i>T</i> <b>&gt;</b> <b>;</b>
</pre>

C. Als so genannter *Constrained Template Parameter*:

<pre>
<b>template</b> <b>&lt;</b> <i>Your_Requirement_Or_Concept</i> <i>T</i> <b>&gt;</b>
<b>void</b> <i>func();</i>
</pre>


---


## Literaturhinweise

Die Anregungen zu den Beispielen stammen aus einem Blog von *Sandor Dargo*:

[How to write your own C++ concepts? Part I](https://www.sandordargo.com/blog/2021/03/10/write-your-own-cpp-concepts-part-i)

[How to write your own C++ concepts? Part II](https://www.sandordargo.com/blog/2021/03/17/write-your-own-cpp-concepts-part-ii)

Weitere Beispiele sind aus 

[C++ Template Programming](https://www.3dgep.com/beginning-cpp-template-programming)

entnommen.


---

[Zur�ck](Readme.md)

---
