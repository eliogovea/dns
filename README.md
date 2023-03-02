# dns
Library to handle DNS messages

## Build

```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release --toolchain cmake/toolchain/gcc.cmake -S . -B ./build/
cmake --build ./build/ --parallel $(($(nproc)*2)) --target all
```

## Test

```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Debug --toolchain cmake/toolchain/gcc.cmake -S . -B ./build/
cmake --build ./build/ --parallel $(($(nproc)*2)) --target unit_tests
ctest --test-dir ./build/ --extra-verbose
```

### Code coverage

```bash
mkdir build
cmake --toolchain cmake/toolchain/gcc-coverage.cmake -S . -B ./build/
cmake --build ./build/ --parallel $(($(nproc)*2))
ctest --test-dir ./build/ --extra-verbose
gcovr --verbose --root . --filter src/ --html --html-details --output ./build/coverage.html
```

### Fuzz



## Examples

```bash
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release --toolchain cmake/toolchain/gcc.cmake -S . -B ./build/
cmake --build ./build/ --parallel $(($(nproc)*2)) --target resolver
```