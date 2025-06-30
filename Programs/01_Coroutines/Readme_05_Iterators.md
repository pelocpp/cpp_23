# Coroutine Iteratoren

[Zurück](Readme.md)

---

[Quellcode](Coroutines_05_Iterators.cpp)

---

## Unterstützung für &ldquo;Range-based&rdquo; `for`-Wiederholungsschleife

Coroutinen lassen sich auch im Kontext einer &ldquo;Range-based&rdquo; `for`-Wiederholungsschleife
anwenden. Dazu müssen wir die Coroutinen-Generatorklasse um einen
Iterator ergänzen.

Der Quellcode zu diesem Beispiel nimmt sich die Generator-Klasse
aus der offiziellen C++ Referenz zu Hilfe:

[`std::coroutine_handle`, `std::noop_coroutine_handle`](https://en.cppreference.com/w/cpp/coroutine/coroutine_handle)

Die wesentlichen Zeilen auf dieser exemplarischen Generator-Klasse
sehen so aus:

<pre>
01: // Range-based for loop support
02: class Iter {
03: public:
04:     void operator++() { 
05:         m_coroutine.resume(); 
06:     }
07:     const T& operator*() const { 
08:         return *m_coroutine.promise().current_value; 
09:     }        
10:     bool operator==(std::default_sentinel_t) const { 
11:         return !m_coroutine || m_coroutine.done(); 
12:     }
13: 
14:     explicit Iter(const Handle coroutine) : 
15:         m_coroutine{coroutine}
16:     {}
17: 
18: private:
19:     Handle m_coroutine;
20: };
21: 
22: Iter begin() {
23:     if (m_coroutine) {
24:         m_coroutine.resume();
25:     } 
26:     return Iter{m_coroutine};
27: }
28: std::default_sentinel_t end() { 
29:     return {}; 
30: }
</pre>

Damit lassen sich nun folgende &ldquo;Range-based&rdquo; `for`-Wiederholungsschleifen
programmieren:


<pre>
01: Generator<std::string> generator()
02: {
03:     co_yield std::string{ "The" };
04:     co_yield std::string{ "quick" };
05:     co_yield std::string{ "brown " };
06:     co_yield std::string{ "fox" };
07:     co_yield std::string{ "jumps " };
08:     co_yield std::string{ "over" };
09:     co_yield std::string{ "the" };
10:     co_yield std::string{ "lazy" };
11:     co_yield std::string{ "dog" };
12:     co_return;
13: }
14: 
15: void test()
16: {
17:     for (const auto& next : generator()) {
18:         std::cout << next << std::endl;
19:     }
20: }
</pre>

*Ausgabe*:

<pre>
The
quick
brown
fox
jumps
over
the
lazy
dog
</pre>

Der Abbruch der &ldquo;Range-based&rdquo; `for`-Wiederholungsschleife
im letzten Beispiel erklärt sich durch den Einsatz von `co_return`
in der Coroutine.

Dies muss nicht immer der Fall sein, eine Coroutine kann prinzipiell
auch &ldquo;unendlich&ldquo; arbeiten. In diesem Fall obliegt es der
Anwendung, eine Wiederholungsschleife kontrolliert zu verlassen.

Auch hierzu ein kleines Beispiel in Form des &ldquo;*Fizz-Buzz*&ldquo;-Gruppen-Wortspiels,
bei dem man etwas über die mathematische Division lernen kann:

<pre>
01: Generator<std::string> fizzbuzz()
02: {
03:     for (int i = 1; ; i++) {
04:         std::string result{};
05:         if (i % 3 == 0) 
06:             result += "Fizz";
07:         if (i % 5 == 0)
08:             result += "Buzz";
09:         if (result.empty())
10:             co_yield std::to_string(i);
11:         else
12:             co_yield result;
13:     }
14: }
15: 
16: void test()
17: {
18:     auto coroutine = fizzbuzz();
19: 
20:     // endless loop
21:     for (int n{}; const auto & next : coroutine) {
22:         std::cout << next << std::endl;
23:         if (++n >= 20) break;
24:     }
25: }
</pre>

*Ausgabe*:

<pre>
1
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
16
17
Fizz
19
Buzz
</pre>

---


## Literaturhinweise

Die Anregungen zu dem Beispiel stammen zum großen Teil aus dem Artikel

&ldquo;[C++20 Ranges &ndash; Complete Guide](https://itnext.io/c-20-ranges-complete-guide-4d26e3511db0)&rdquo;
von Simon Tóth.

---

[Zurück](Readme.md)

---
