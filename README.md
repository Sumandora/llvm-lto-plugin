# LLVM LTO plugin

This is an LLVM pass plugin, that acts upon entire projects (as opposed to translation units).

## How it works

LLVM, just like GCC, supports a feature called `Fat LTO objects`. The idea is to bypass the compilation to object files, by adding a secret section into the object and dumping the entire LLVM bitcode into it.
The linker can then reconstruct a single module, that contains the LLVM bitcode of all the objects, which can then be optimized as a whole.

## Building

Ensure you have the LLVM and its headers installed, then build like any other CMake project

```sh
mkdir build && cd build
cmake ..
make
```

Watch out, that the LLVM version matches the one, that's running the plugin.

## Example

One must explicitly enable this feature, by enabling `lto` and `fat-lto-objects`:

```sh
clang++ a.cpp -c -flto -ffat-lto-objects
clang++ b.cpp -c -flto -ffat-lto-objects

clang++ -fuse-ld=lld -flto -ffat-lto-objects -Wl,--load-pass-plugin=/path/to/libDumpPass.so a.o b.o
```

The plugin provided in this repository will dump the entire module into stderr, so that you can inspect the result and verify that both translation units have been fused together.

### CMake

CMake can be configured like this:

```sh
export CC=clang
export CXX=clang++
cmake .. \
  -DCMAKE_CXX_FLAGS="-flto -ffat-lto-objects" \
  -DCMAKE_C_FLAGS="-flto -ffat-lto-objects" \
  -DCMAKE_LINKER="clang" \
  -DCMAKE_[SHARED/EXE/MODULE/STATIC]_LINKER_FLAGS="-fuse-ld=lld -flto -ffat-lto-objects -Wl,--load-pass-plugin=/path/to/libDumpPass.so"
```
