# 2022.6.7

When I learn to write the `sylar`, I want to let config variable to inform the config manager once the value is set.

## Circular dependency

First I declared the config manager class before config variable, it doesn't work.

I got a circular dependency between these two classes.

> https://stackoverflow.com/questions/29215723/incomplete-type-used-in-nested-name-specifier

## Undefined reference

Then I defined the function in the source file and declared it in the header. Then I got an undefined reference error when linking.

> https://blog.csdn.net/Cold_Sun_/article//details/100584418

So the solution is to define the function in header file but after the definition of the classes.


