# TinDox Server

## How to build

```bash
cmake -B build
cmake --build build
```

## How to test

```bash
cmake -B build -DTDS_TESTING=ON
cmake --build build
cd build
ctest
```
