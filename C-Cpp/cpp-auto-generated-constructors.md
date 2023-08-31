#c-cpp
## 2023.08.31

> https://stackoverflow.com/questions/4943958/conditions-for-automatic-generation-of-default-copy-move-ctor-and-copy-move-assi

## At which time compiler provides default copy move constructor

-   The default constructor is auto-generated if there is **no user-declared constructor** (§12.1/5).
-   The copy constructor is auto-generated if there is **no user-declared move constructor or move assignment operator**   (because there are no move constructors or move assignment operators in C++03, this simplifies to "always" in C++03) (§12.8/8).
-   The copy assignment operator is auto-generated if there is **no user-declared move constructor or move assignment operator** (§12.8/19).
-   The destructor is auto-generated if there is **no user-declared destructor** (§12.4/4).

### C++11 and later only:

-   The move constructor is auto-generated if there is **no user-declared copy constructor, copy assignment operator or destructor, and if the generated move constructor is valid** (§12.8/10).
-   The move assignment operator is auto-generated if there is no user-declared copy constructor, copy assignment operator or destructor, and if the generated move assignment operator is valid (e.g. if it wouldn't need to assign constant members) (§12.8/21).

![Alt text](../.images/CVtPu.jpg)