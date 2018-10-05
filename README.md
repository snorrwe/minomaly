# Minomaly engine

[![Build status](https://ci.appveyor.com/api/projects/status/76liyvvyl3x3i4dg/branch/master?svg=true)](https://ci.appveyor.com/project/snorrwe/minomaly/branch/master)

## Requirements

- [C++17 compatible compiler](https://en.cppreference.com/w/cpp/compiler_support) (e.g. MSVC 19+)
- [CMake 3.0+](https://cmake.org/)

## Pulling submodules

This repository makes use of [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules) be sure to initialize your submodules

```
git submodule init
git submodule update
```

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
- `Component`: Holds the data of an _entity_ required by some functionality. Components should not hold resources, let _systems_ handle that
- `System`: Implements functionality. Generally used to iterate on _entities_ or just _components_
- `Manager`: Manages memory and lifetimes of _entities_

Example: __TBA__
