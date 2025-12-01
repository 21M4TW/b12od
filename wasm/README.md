# Bolt12 Offer Decoding Library

WASM binding for the [Bolt12 Offer Decoding Library](https://github.com/21M4TW/b12od).

[Emscripten](https://emscripten.org) needs to be installed to compile the
module. The module can then be compiled and tested using the following commands:
```
mkdir build
cd build/
emcmake cmake ..
emmake make
cd ../
cp build/b12od.{wasm, js} examples/
cd examples/
python3 -m http.server 8080
```
