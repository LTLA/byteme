#ifndef BYTEME_BYTEME_HPP
#define BYTEME_BYTEME_HPP

#include "Reader.hpp"
#include "RawBufferReader.hpp"
#include "RawFileReader.hpp"
#include "IstreamReader.hpp"
#include "ChunkedBufferReader.hpp"

#include "Writer.hpp"
#include "RawBufferWriter.hpp"
#include "RawFileWriter.hpp"
#include "OstreamWriter.hpp"

#include "PerByte.hpp"

#ifndef BYTEME_NO_ZLIB
#include "GzipFileReader.hpp"
#include "ZlibBufferReader.hpp"

#include "SomeBufferReader.hpp"
#include "SomeFileReader.hpp"

#include "GzipFileWriter.hpp"
#include "ZlibBufferWriter.hpp"
#endif

/**
 * @file byteme.hpp
 * @brief Umbrella header for all **byteme** classes
 */

/**
 * @namespace byteme
 * @brief Simple byte readers and writers
 */
namespace byteme {}

#endif
