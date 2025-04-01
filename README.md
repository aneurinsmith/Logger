
[![build (windows)](https://img.shields.io/github/actions/workflow/status/aneurinsmith/Logger/build-windows.yml?logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZlcnNpb249IjEiIHdpZHRoPSI2MDAiIGhlaWdodD0iNjAwIiB2aWV3Qm94PSItMTAwIC0xMDAgMTAwMCAxMDAwIj48cGF0aCBkPSJNMCAxMTIuNzQ0IDMyNC40MjQgNjguNTZsMC4xNDQgMzEyLjkzNiAtMzI0LjI3MiAxLjg0OHptMzI0LjI3MiAzMDQuODA4IDAuMjU2IDMxMy4yMDhMMC4yNTYgNjg2LjE4NCAwLjI0IDQxNS40NTZ6TTM2My42IDYyLjc3NiA3OTMuNzYgMHYzNzcuNTJsLTQzMC4xNiAzLjQxNnptNDMwLjI2NCAzNTcuNzIgLTAuMTA0IDM3NS44MTYgLTQzMC4xNiAtNjAuNzEyIC0wLjYgLTMxNS44MDh6IiBmaWxsPSIjZmZmIi8+PC9zdmc+&style=flat-square)](https://github.com/aneurinsmith/Logger/actions/workflows/build-windows.yml)
[![build (linux)](https://img.shields.io/github/actions/workflow/status/aneurinsmith/Logger/build-linux.yml?logo=ubuntu&logoColor=white&style=flat-square)](https://github.com/aneurinsmith/Logger/actions/workflows/build-linux.yml)
[![release](https://img.shields.io/github/v/release/aneurinsmith/Logger?style=flat-square)](https://github.com/aneurinsmith/Logger/releases/latest)

##
Requires `libx11-dev`, `libxft-dev`, and `libxrandr-dev` when compiling on linux.\
Tested on x86 and x64 using ninja-build, wsl (ubuntu 18.04), and clang++.

<br>

## CMake Dependency

The full list of releases can be found [here](https://github.com/aneurinsmith/Logger/releases).
```cmake
FetchContent_Declare(logger 
	URL https://github.com/aneurinsmith/Logger/releases/download/v1.0.1/logger-src.zip
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
```

<sub>_CMake version 3.12 is required_.</sub>\
<sub>_C++ version 17 is required_.</sub>

<br>

## Output Sinks

Each output sink has a corresponding inline factory for easy instantiation.
 *  ```c++
    LOG::fStreamSink(name, fmt)
    ```
 *  ```c++
    LOG::oStreamSink(fmt)
    ```
 *  ```c++
    LOG::consoleSink(name, size, fmt)
    ```

