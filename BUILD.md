# Building the Library

CMake is used to build the library. Once inside the repository, it can be built using

```
cmake -S . -B build
cmake --build build
```

and then installed with

`cmake --install build --prefix [destination_directory]`.

Individual python_binding, rust_binding and wasm_binding targets are defined,
and an all_bindings target also exists, e.g.:
```
cd build
make all_bindings
```

# Testing

The library can be tested using
```
cd build
ctest -L cmp
ctest -T memcheck -L memcheck
```
