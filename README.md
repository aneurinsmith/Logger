
[![build (windows)](https://img.shields.io/github/actions/workflow/status/aneurinsmith/Logger/build-windows.yml?logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZlcnNpb249IjEiIHdpZHRoPSI2MDAiIGhlaWdodD0iNjAwIiB2aWV3Qm94PSItMTAwIC0xMDAgMTAwMCAxMDAwIj48cGF0aCBkPSJNMCAxMTIuNzQ0IDMyNC40MjQgNjguNTZsMC4xNDQgMzEyLjkzNiAtMzI0LjI3MiAxLjg0OHptMzI0LjI3MiAzMDQuODA4IDAuMjU2IDMxMy4yMDhMMC4yNTYgNjg2LjE4NCAwLjI0IDQxNS40NTZ6TTM2My42IDYyLjc3NiA3OTMuNzYgMHYzNzcuNTJsLTQzMC4xNiAzLjQxNnptNDMwLjI2NCAzNTcuNzIgLTAuMTA0IDM3NS44MTYgLTQzMC4xNiAtNjAuNzEyIC0wLjYgLTMxNS44MDh6IiBmaWxsPSIjZmZmIi8+PC9zdmc+)](https://github.com/aneurinsmith/Logger/actions/workflows/build-windows.yml)
[![build (linux)](https://img.shields.io/github/actions/workflow/status/aneurinsmith/Logger/build-linux.yml?logo=ubuntu&logoColor=white)](https://github.com/aneurinsmith/Logger/actions/workflows/build-linux.yml)
[![release](https://img.shields.io/github/v/release/aneurinsmith/Logger.svg?style=flat)](https://github.com/aneurinsmith/Logger/releases/latest)

## CMake dependency

The full list of releases can be found [here](https://github.com/aneurinsmith/Logger/releases).\
_CMake version 3.12 is required_. 
```
include(FetchContent)
FetchContent_Declare(logger URL https://github.com/aneurinsmith/Logger/releases/download/v0.2.1/logger-src.zip)
FetchContent_MakeAvailable(logger)
```

## Output sinks

Each output sink has a corresponding inline factory for easy instantiation.
 * `LOG::fStreamSink`
 * `LOG::oStreamSink`
 * `LOG::consoleSink`

