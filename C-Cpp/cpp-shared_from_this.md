#c-cpp 
## 2022.6.8

## Use `shard_from_this` in template classes

> https://stackoverflow.com/questions/17853212/using-shared-from-this-in-templated-classes

`enable_shared_from_this<manager<T>>` is a "dependent base" (it's a base class whose type depends on a template parameter, in this case T) so the rules of C++ say that unqualified name lookup doesn't look in there, you need to say `this->shared_from_this()` or `std::enable_shared_from_this<manage<T>>::shared_from_this()` to find the member from the dependent base.

See http://gcc.gnu.org/wiki/VerboseDiagnostics#dependent_base for more detail and links to other references.
