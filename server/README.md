# TinDox Server

Author: Jakub Mazurkiewicz

License: MIT (see [`LICENSE`](https://github.com/JMazurkiewicz/TinDox/blob/main/LICENSE) for details)

## How to get

### Get some dependencies

This step is not required - CMake will take care of dependencies anyway.

```bash
sudo apt-get libfmt-dev libspdlog-dev
```

### Build and install

```bash
cmake -B build
cmake --build build
sudo cmake --install build
```

### Run tests

```bash
cmake -B build -DTDS_TESTS=ON [-DTDS_{TYPE}_TESTS=ON...]
cmake --build build
ctest --test-dir build
```

The `{TYPE}` must be at least one of:

* `UNIT`
* `INTEGRATION`
