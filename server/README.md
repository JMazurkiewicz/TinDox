# TinDox Server

## Build

```bash
cmake -B build
cmake --build build
```

## Test

```bash
cmake -B build -DTDS_TESTING=ON
cmake --build build
cd build
ctest
```

## Install

```bash
sudo cmake --install build
```
