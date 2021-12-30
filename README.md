# Buffered inputs

![Unit tests](https://github.com/clusterfork/buffin/actions/workflows/run-tests.yaml/badge.svg)
![Documentation](https://github.com/clusterfork/buffin/actions/workflows/doxygenate.yaml/badge.svg)

## Overview

This library contains a few utilities to parse buffered inputs from uncompressed or Gzip-compressed files.
By supplying an object with an `.add()` method to parser a buffer of (unsigned) `char`s, we can easily read from buffers or files, with or without Gzip compression.
This allows me to consolidate some common boilerplate across several projects, e.g., [**tatami**](https://github.com/LTLA/tatami), [**singlepp**](https://github.com/clusterfork/singlepp).
Interfacing with Zlib is particularly fiddly and I don't want to have to remember how to do it in each project.

## Usage

First we need to define a class that has an `add(const char*, size_t)` method.
The first argument is a pointer to a buffer while the second argument is the length of the buffer;
the method should then parse the contents of the buffer to load something from the file. 
This method should be templated to accept both `char` or `unsigned char` inputs.
See the [`LineReader`](tests/src/LineReader.h) class for an example of such a class.

Once we have an instance of this class, we can pass it to the **buffin** helper functions.
For example, we can parse a text file by passing in the `parser` instance:

```cpp
#include "buffin/parse_text_file.hpp"

buffin::parse_text_file(filepath, parser); 
```

This will repeatedly call `parser.add()` on chunks of the desired file until completion.
It is assumed that `parser` is modified by reference such that the parsed contents of the file can be extracted from `parser`'s members.
The same approach can be used to parser Gzip files or buffers, provided Zlib is available -
more details can be found in the [reference documentation](https://clusterfork.github.io/buffin).

## Building projects

If you're using CMake, you just need to add something like this to your `CMakeLists.txt`:

```
include(FetchContent)

FetchContent_Declare(
  buffin 
  GIT_REPOSITORY https://github.com/clusterfork/buffin
  GIT_TAG master # or any version of interest
)

FetchContent_MakeAvailable(buffin)
```

Then you can link to **buffin** to make the headers available during compilation:

```
# For executables:
target_link_libraries(myexe buffin)

# For libaries
target_link_libraries(mylib INTERFACE buffin)
```

If you're not using CMake, the simple approach is to just copy the files and include their path during compilation.
