# Minomaly engine

## Requirements

- [C++17 compatible compiler](https://en.cppreference.com/w/cpp/compiler_support) (e.g. MSVC 19+)
- [CMake 3.0+](https://cmake.org/)

## Initializing CMake

```
cmake -H. -Bbuild
```

## Building

```
cmake --build build
```

## Running the example

```
build\Debug\app.exe # on Windows
```

## Basic concepts

- `Entity`: An entity in the game, does not implement anything special, it's just an identifier to find _components_ that are linked together
- `Component`: Holds the data of an _entity_ required by some functionality
- `System`: Implements functionality. Generally used to iterate on _entities_ or just _components_
- `Manager`: Manages memory and lifetimes of _entities_

Example: __TBA__
