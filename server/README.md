# TinDox Server

Author: Jakub Mazurkiewicz

License: MIT (see [`LICENSE`](https://github.com/JMazurkiewicz/TinDox/blob/main/LICENSE) for details)

## How to install

### Easy way: use `dpkg`

Go to [release page](https://github.com/JMazurkiewicz/TinDox/releases) and download latest `TinDoxServer.deb`. Then go to your `Downloads` directory and do these steps:

```sh
sudo dpkg -i TinDoxServer.deb
dpkg -s TinDoxServer
```

### Hard way: build it yourself

#### Get some dependencies

This step is not required - CMake will take care of dependencies anyway.

```sh
sudo apt-get libfmt-dev libspdlog-dev
```

#### Build and install

```sh
git clone https://github.com/JMazurkiewicz/TinDox.git
cd server
cmake -B build
cmake --build build
sudo cmake --install build
```

## How to set up TDS

```sh
mkdir my_tds # Create directory for your TDS
cd my_tds
tds init     # Create TDS server instance
tds run      # Run
```

## How to run tests

```sh
cmake -B build -DTDS_TESTS=ON -R "TYPE::*"
cmake --build build
ctest --test-dir build
```

The `TYPE` must be one of:

* `UNIT`
* `INTEGRATION`
