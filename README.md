# Read bytes from somewhere

![Unit tests](https://github.com/clusterfork/buffin/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/clusterfork/buffin/actions/workflows/doxygenate.yaml/badge.svg)

## Overview

This library implements a few functors to read buffered inputs from uncompressed or Gzip-compressed files or buffers.
Classes can be exchanged at compile- or run-time to easily re-use the same code across different input sources.
The aim is to consolidate some common boilerplate across several projects, e.g., [**tatami**](https://github.com/LTLA/tatami), [**singlepp**](https://github.com/clusterfork/singlepp).
Interfacing with Zlib is particularly fiddly and I don't want to be forced to remember how to do it in each project.

## Usage

Usage is simple - create an instance of the desired `Reader` class and loop until no bytes remain in the source.

```cpp
#include "byteme/GzipFileReader.hpp"

byteme::GzipFileReader reader(filepath); 
bool remaining = true;

do {
    remaining = reader();
    const unsigned * buffer = reader.buffer();
    size_t available = reader.available();

    /* Do something with the available bytes in the buffer */

} while (remaining);
```

The different `Reader` subclasses can be switched at compile-time via templating, or at run-time by exploiting the class hierarchy:

```cpp
#include "byteme/GzipBufferReader.hpp"
#include "byteme/RawBufferReader.hpp"
#include <memory>

std::unique_ptr<byteme::Reader> ptr;
if (some_condition) {
    ptr.reset(new GzipBufferReader(buffer, length));
} else {
    ptr.reset(new RawBufferReader(buffer, length));
}

/* Loop with ptr->operator()(), ptr->buffer(), etc. */
```

More details can be found in the [reference documentation](https://clusterfork.github.io/buffin).

## Building projects

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```
include(FetchContent)

FetchContent_Declare(
  byteme 
  GIT_REPOSITORY https://github.com/clusterfork/buffin
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(byteme)
```

Then you can link to **byteme** to make the headers available during compilation:

```
# For executables:
target_link_libraries(myexe byteme)

# For libaries
target_link_libraries(mylib INTERFACE byteme)
```

If you're not using CMake, the simple approach is to just copy the files and include their path during compilation.

## Further comments

I thought about using C++ streams, much like how the [**zstr**](https://github.com/mateidavid/zstr) library handles Gzip (de)compression.
However, I'm not very knowledgeable about the `std::istream` interface, so I decided to go with something simpler.
Just in case, I did add a `byteme::IstreamReader` class so that **byteme** clients can easily leverage custom streams. 
