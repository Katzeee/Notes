#c-cpp
## 2023.08.21

> https://hackingcpp.com/cpp/lang/enumerations.html

## Underlying Type Of Enumerations

-   must be an integer type (`char`, `short`, `long`, …)
-   `int` is the default

```cpp
// 7 values ⇒ char should be enough
enum class day : char {
  mon, tue, wed, thu, fri, sat, sun
};
// less than 10,000 ⇒ short should be enough
enum class language_ISO639 : short {
  abk, aar, afr, aka, amh, ara, arg, …
};
```

## Custom Enumerator Mapping Enumerator Mapping Enumerators

-   enumerator values can be set explicitly
-   need not start with 0
-   some values can be left out
-   can be partial (only some enumerators with expl. value)

**if you set enumerator values explicitly, do it for _all_ enumerators**

```cpp
enum class month {
  jan = 1, feb = 2, mar = 3, apr = 4,  may = 5,  jun = 6,  
  jul = 7, aug = 8, sep = 9, oct = 10, nov = 11, dec = 12 
};
enum class flag {
  A = 2, B = 8, C = 5, D, E, F = 25
};
```

## Conversions To/From Underlying Type Enumerator ↔ Integer Conversion

**Enum → Integer**

```cpp
int i = static_cast<int>(month::mar);  
// i: 3
```

**Integer → Enum**

```cpp
int i = 0;
cin >> i;
// make sure i ≥ 1 and ≤ 12 …
month m1 = static_cast<month>(i);
```