# linebot

_Is it a robot? Is it an app? It's an over-engineered `+` in the terminal!_


## Purpose

This project arises from the personal interests of its creator, and has only
a vague direction and purpose — _to be developed_; to be a stage for learning
various concepts and patterns in programming (and beyond).


### License

To that end, `linebot` is available to others who may find it a suitable stage
for their own learning: although a conscious decision has been made **not** to
attach a license to it at least as of yet, feel free to drop a line to
ivanovkirilg@gmail.com if you want a formal permission to use it, or if you
wish to contribute.

Currently, this project depends on:
- [cmake_helpers](https://github.com/zhivkopetrov/cmake_helpers) (MIT License)
- [zpp::bits](https://github.com/eyalz800/zpp_bits) (MIT License)
- [gtest](https://github.com/google/googletest) (BSD-3-Clause license)



## Development


### Platform Support

`linebot` is built using CMake on Linux, in C++20.

To save on maintenance effort, it does not (yet) try to be multi-platform or
to support a wide range of compilers, language standards, build systems, etc.

It aims to be runnable out-of-the-box on GitHub Codespaces but is more actively
developed in a WSL2 Ubuntu environment.


### Environment

#### A) Codespace

1. On GitHub, click "Code" → "Create codespace on main",
1. Wait for all blue progress bars in VS Code to go away,
1. Patiently wait for "Running postCreateCommand..." to go away.

#### B) Devcontainer

GitHub codespaces are cloud Docker containers that integrate with VS Code.
You can use a local Docker engine and VS Code to achieve the same setup as A).

#### C) Linux/WSL2

The creator of this project primarily uses an Ubuntu on WSL2. This is likely
to be less straight-forward than A) or B) for you, depending on what packages
you have installed or not.


### Build

#### Initial

```bash
mkdir build
cd build
cmake ..
make -j
```

#### Incremental

After the initial build above, it is recommended to stay in `build/` and run
**`make -j`** for subsequent builds.

#### Test

After a build, run the automated tests with:
- `ctest` for all tests,
- `ctest -LE known-fail` to exclude known broken tests, if any.

Or run the application:
- `DRVR/DRVR_main <port> &` to start the 'Driver' process in the background,
- `MAIN/MAIN <port>` to start the main process with terminal user interface,
- ... where `<port>` is the same number (e.g. `9999`).
