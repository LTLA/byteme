# Buffered inputs

This library contains a few utilities to handle buffered inputs from uncompressed or Gzip-compressed files.
By supplying an object with an `.add()` method to process a buffer of `char`s, we can easily read from (compressed) buffers or files.
