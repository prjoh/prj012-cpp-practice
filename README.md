# [PRJ012] C++ Showcase

A C++ showcase project.

## Build

### Prerequisites

1. Ensure that [CMake](https://cmake.org/download/) is installed on your system. You can check this by running `cmake --version` in your terminal or command prompt.
2. Make sure [Ninja](https://github.com/ninja-build/ninja/releases) is installed. You can check this by running `ninja --version` in your terminal or command prompt.
3. Make sure that either [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/) or [Build Tools for Visual Studio](https://visualstudio.microsoft.com/visual-cpp-build-tools/) are installed on your system.

Further build prerequisites are required for building dependencies:

1. A [Python interpreter](https://www.python.org/downloads/) (3.0 or later)
2. Windows SDK 10.0.17763.0 or later (installed with Visual Studio 2022)
3. C++ build tools (installed with Visual Studio 2022)
4. Visual C++ ATL Support (installed with Visual Studio 2022)

### Building

Open the `Developer Powershell for VS 2022` and navigate to the project root directory.

The `Make.bat` script creates a debug build of the project:
```
.\Make.bat
```

You can specify the build type like so:

```
.\Make.bat Release
```

Supported build types are:

- Debug
- Release
- MinSizeRel
- RelWithDebInfo

You can cleanup the Build direcotry by executing:

```
.\Clean.bat
```

## Run

You can run the project by executing:

```
.\Run.bat
```

You can also specify the build type to run:

```
.\Run.bat Release
```

Alternatively you can navigate to the build direcotry to find the executable:

```
.\Build\<BUILD_TYPE>\Bin\Game.exe
```
