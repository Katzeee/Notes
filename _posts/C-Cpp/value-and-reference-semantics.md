#c-cpp
## 2023.08.21

> https://hackingcpp.com/cs/value_vs_reference_semantics.html

## Value Semantics(V) vs Reference Semantics(R)

!!! Value semantics focuses on values, while reference semantics focuses on object identities

**Value Variables(V)**

-   refer to objects itself, i.e., _one_ specific memory location
-   exactly one value variable per object
-   lifetimes of variable and associated object are identical

**Reference Variables(R)**

-   may refer to any memory location that holds a type-compatible object
-   many reference variables per object possible
-   lifetimes of variable and referenced object are independent

**Value Members(V)** 

-   are usually stored as part of the containing object
-   refer to objects with the same lifetime as the containing object

**Reference Members(R)**

-   refer to separately stored objects
-   have independent lifetimes from referenced objects

**Deep Copying(V)** 

-   produces a new, independent object; object (member) values are copied

**Shallow Copying(R)**

-   produces a new reference to the same object

**Deep Assignment(V)** 

-   makes _value_ of target object equal to that of the source object
-   is either done by (recursively) copy-assigning members or alternatively by transferring ownership, i.e., (recursively) moving the source's value into the target

**Shallow Assignment(R)**

-   target variable is redirected to refer to the same object as the source variable

**Equality-Based Comparison(V)** 

-   objects are equal, if their (member) values are equal

**Identity-Based Comparison(R)**

-   objects are identical, if their memory addresses are equal


