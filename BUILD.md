# Building the Library

CMake is used to build the library. Once inside the repository, it can be built using

`cmake .`

`make`

and then installed with

`make install`

An installation directory can be specified through `cmake` using

`cmake -DCMAKE_INSTALL_PREFIX=[destination_directory] .`

# Testing

The library can be tested using
```
ctest -L cmp
ctest -T memcheck -L memcheck
```
