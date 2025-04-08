# Gimme some bytes 

![Unit tests](https://github.com/LTLA/byteme/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/LTLA/byteme/actions/workflows/doxygenate.yaml/badge.svg)
[![codecov](https://codecov.io/gh/LTLA/byteme/branch/master/graph/badge.svg?token=7I3UBJLHSO)](https://codecov.io/gh/LTLA/byteme)

## Overview

This library implements a few functors to read buffered inputs from uncompressed or Gzip-compressed files or buffers.
Classes can be exchanged at compile- or run-time to easily re-use the same code across different input sources.
The aim is to consolidate some common boilerplate across several projects, e.g., [**tatami**](https://github.com/LTLA/tatami), [**singlepp**](https://github.com/LTLA/singlepp).
Interfacing with Zlib is particularly fiddly and I don't want to be forced to remember how to do it in each project.

## Usage

To read bytes, create an instance of the desired `Reader` class and loop until no bytes remain in the source.

```cpp
#include "byteme/byteme.hpp"

const char* filepath = "input.gz";
byteme::GzipFileReader reader(filepath, {}); 

while (reader.load()) {
    const unsigned char * buffer = reader.buffer();
    size_t available = reader.available();

    /* Do something with the available bytes in the buffer */
}
```

To write bytes, create the desired `Writer` class and supply an array of bytes until completion.

```cpp
#include "byteme/byteme.hpp"

std::vector<std::string> lyrics { 
    "Kimi dake o kimi dake o", 
    "Suki de ita yo",
    "Kaze de me ga nijinde",
    "Tooku naru yo"
};

byteme::GzipFileWriter writer("something.gz", {});
const char newline = '\n';
for (const auto& line : lyrics) {
    writer.write(reinterpret_cast<const unsigned char*>(line.c_str()), line.size());
    writer.write(reinterpret_cast<const unsigned char*>(&newline), 1);
}

writer.finish();
```

More details can be found in the [reference documentation](https://ltla.github.io/byteme).

## Supported classes

For the readers:

| Class | Description |
|-------|-------------|
|`RawBufferReader`| Read from a uncompressed buffer|
|`RawFileReader`| Read from an uncompressed file|
|`ZlibBufferReader`| Read from a Zlib-compressed buffer|
|`GzipFileReader`| Read from an Gzip-compressed file|
|`SomeBufferReader`| Read from a possibly-Gzip-compressed buffer|
|`SomeFileReader`| Read from a possibly-Gzip-compressed file|
|`IstreamReader`| Read from a `std::istream`|

For the writers:

| Class | Description |
|-------|-------------|
|`RawBufferWriter`| Write to a uncompressed buffer|
|`RawFileWriter`| Write to an uncompressed file|
|`ZlibBufferWriter`| Write to a Zlib-compressed buffer|
|`GzipFileWriter`| Write to an Gzip-compressed file|
|`OstreamWriter`| Write to a `std::ostream`|

The different subclasses can be switched at compile time via templating, or at run-time by exploiting the class hierarchy:

```cpp
#include "byteme/byteme.hpp"
#include <memory>

std::vector<unsigned char> input_buffer;
auto buffer = input_buffer.data();
size_t length = input_buffer.size();

std::unique_ptr<byteme::Reader> ptr;
if (some_condition) {
    ptr.reset(new byteme::ZlibBufferReader(buffer, length, {}));
} else {
    ptr.reset(new byteme::RawBufferReader(buffer, length, {}));
}

if (ptr->load()) {
    auto bufptr = ptr->buffer();
    auto available = ptr->available();
    /* Do something with the buffer. */
}
```

Most of the `Reader` and `Writer` constructors will also accept a matching `Options` instance to fine-tune their behavior.

```cpp
// For readers.
byteme::ZlibBufferReaderOptions zopt;
zopt.buffer_size = 8096;
zopt.mode = 2; // Gzip
byteme::ZlibBufferReader zreader(buffer, length, zopt);

// For writers.
byteme::ZlibBufferWriterOptions zwopt;
zwopt.buffer_size = 8096;
zwopt.mode = 0; // DEFLATE
zwopt.compression_level = 9;
byteme::ZlibBufferReader zwriter(zwopt);
```

## Reading byte-by-byte

Users may prefer to wrap the `Reader` in a `PerByteSerial` instance to access one input byte at a time.
This avoids the boilerplate of managing all of the other (yet-to-be-used) bytes from `available()`.

```cpp
byteme::PerByteSerial<char> pb(new byteme::GzipFileReader(filepath, {}));
auto valid = pb.valid();
while (valid) {
    char x = pb.get();
    // Do something with 'x'.
    valid = pb.advance();
}
```

We can also extract a range of bytes:

```cpp
byteme::PerByteSerial<unsigned char> pb(new byteme::GzipFileReader(filepath, {}));
while (valid) {
    int32_t value;
    auto outcome = pb.extract(reinterpret_cast<unsigned char*>(&value), sizeof(int32_t)); 
    if (outcome.first != sizeof(int32_t)) {
        // uh oh, not enough bytes.
    } else {
        // do something with the extracted integer.
    }
    valid = outcome.second;
}
```

We can even perform the reading in a separate thread via the `PerByteParallel` class.
This allows the (possibly expensive) disk IO operations to be performed in parallel to the user-level parsing.

```cpp
byteme::PerByteParallel<char> pb(new byteme::GzipFileReader(filepath, {}));
auto valid = pb.valid();
while (valid) {
    char x = pb.get();
    // Do something with 'x'.
    valid = pb.advance();
}
```

## Building projects

### CMake using `FetchContent`

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
  byteme 
  GIT_REPOSITORY https://github.com/LTLA/byteme
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(byteme)
```

Then you can link to **byteme** to make the headers available during compilation:

```cmake
# For executables:
target_link_libraries(myexe byteme)

# For libaries
target_link_libraries(mylib INTERFACE byteme)
```

### CMake using `find_package()`

You can install the library by cloning a suitable version of this repository and running the following commands:

```sh
mkdir build && cd build
cmake .. -DBYTEME_TESTS=OFF
cmake --build . --target install
```

Then you can use `find_package()` as usual:

```cmake
find_package(ltla_byteme CONFIG REQUIRED)
target_link_libraries(mylib INTERFACE ltla::byteme)
```

### Manual

If you're not using CMake, the simple approach is to just copy the files the `include/` subdirectory -
either directly or with Git submodules - and include their path during compilation with, e.g., GCC's `-I`.

### Adding Zlib support

To support Gzip-compressed files, we also need to link to Zlib.
When using CMake, **byteme** will automatically attempt to use `find_package()` to find the system Zlib.
If no Zlib is found, it is skipped and no Gzip functionality is provided by the libary.
Users can also set the `BYTEME_FIND_ZLIB` option to `OFF` to provide their own Zlib.

## Further comments

I thought about using C++ streams, much like how the [**zstr**](https://github.com/mateidavid/zstr) library handles Gzip (de)compression.
However, I'm not very knowledgeable about the `std::istream` interface, so I decided to go with something simpler.
Just in case, I did add a `byteme::IstreamReader` class so that **byteme** clients can easily leverage custom streams. 
