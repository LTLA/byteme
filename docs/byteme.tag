<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>byteme.hpp</name>
    <path>byteme/</path>
    <filename>byteme_8hpp.html</filename>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>ChunkedBufferReader.hpp</name>
    <path>byteme/</path>
    <filename>ChunkedBufferReader_8hpp.html</filename>
    <class kind="class">byteme::ChunkedBufferReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>GzipFileReader.hpp</name>
    <path>byteme/</path>
    <filename>GzipFileReader_8hpp.html</filename>
    <class kind="class">byteme::GzipFileReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>GzipFileWriter.hpp</name>
    <path>byteme/</path>
    <filename>GzipFileWriter_8hpp.html</filename>
    <class kind="class">byteme::GzipFileWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>IstreamReader.hpp</name>
    <path>byteme/</path>
    <filename>IstreamReader_8hpp.html</filename>
    <class kind="class">byteme::IstreamReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>OstreamWriter.hpp</name>
    <path>byteme/</path>
    <filename>OstreamWriter_8hpp.html</filename>
    <class kind="class">byteme::OstreamWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>PerByte.hpp</name>
    <path>byteme/</path>
    <filename>PerByte_8hpp.html</filename>
    <class kind="class">byteme::PerByte</class>
    <class kind="class">byteme::PerByteParallel</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>RawBufferReader.hpp</name>
    <path>byteme/</path>
    <filename>RawBufferReader_8hpp.html</filename>
    <class kind="class">byteme::RawBufferReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>RawBufferWriter.hpp</name>
    <path>byteme/</path>
    <filename>RawBufferWriter_8hpp.html</filename>
    <class kind="class">byteme::RawBufferWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>RawFileReader.hpp</name>
    <path>byteme/</path>
    <filename>RawFileReader_8hpp.html</filename>
    <class kind="class">byteme::RawFileReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>RawFileWriter.hpp</name>
    <path>byteme/</path>
    <filename>RawFileWriter_8hpp.html</filename>
    <class kind="class">byteme::RawFileWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>Reader.hpp</name>
    <path>byteme/</path>
    <filename>Reader_8hpp.html</filename>
    <class kind="class">byteme::Reader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>SomeBufferReader.hpp</name>
    <path>byteme/</path>
    <filename>SomeBufferReader_8hpp.html</filename>
    <class kind="class">byteme::SomeBufferReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>SomeFileReader.hpp</name>
    <path>byteme/</path>
    <filename>SomeFileReader_8hpp.html</filename>
    <class kind="class">byteme::SomeFileReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>temp_file_path.hpp</name>
    <path>byteme/</path>
    <filename>temp__file__path_8hpp.html</filename>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>Writer.hpp</name>
    <path>byteme/</path>
    <filename>Writer_8hpp.html</filename>
    <class kind="class">byteme::Writer</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>ZlibBufferReader.hpp</name>
    <path>byteme/</path>
    <filename>ZlibBufferReader_8hpp.html</filename>
    <class kind="class">byteme::ZlibBufferReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>ZlibBufferWriter.hpp</name>
    <path>byteme/</path>
    <filename>ZlibBufferWriter_8hpp.html</filename>
    <class kind="class">byteme::ZlibBufferWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="class">
    <name>byteme::ChunkedBufferReader</name>
    <filename>classbyteme_1_1ChunkedBufferReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>ChunkedBufferReader</name>
      <anchorfile>classbyteme_1_1ChunkedBufferReader.html</anchorfile>
      <anchor>af708f65b9ab92834408bb591d30b65f3</anchor>
      <arglist>(const unsigned char *buffer, size_t length, size_t chunk_size)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ChunkedBufferReader</name>
      <anchorfile>classbyteme_1_1ChunkedBufferReader.html</anchorfile>
      <anchor>a39e73da2644e5f924dedf581a8002056</anchor>
      <arglist>(const char *buffer, size_t length, size_t chunk_size)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1ChunkedBufferReader.html</anchorfile>
      <anchor>ae5f68b4486a3346e6ae062bf1a105226</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1ChunkedBufferReader.html</anchorfile>
      <anchor>afffa05cf40a60a82c01f0f546e78e65c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1ChunkedBufferReader.html</anchorfile>
      <anchor>af4159e974920585c143d57f52a0e8464</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::GzipFileReader</name>
    <filename>classbyteme_1_1GzipFileReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>GzipFileReader</name>
      <anchorfile>classbyteme_1_1GzipFileReader.html</anchorfile>
      <anchor>ac6fba0ca5a6b14614286503060bf0381</anchor>
      <arglist>(const char *path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GzipFileReader</name>
      <anchorfile>classbyteme_1_1GzipFileReader.html</anchorfile>
      <anchor>a80a27441b6d4428c56a0daecf1507fa5</anchor>
      <arglist>(const std::string &amp;path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1GzipFileReader.html</anchorfile>
      <anchor>aa077d794ad9e305842adf83311906b8a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1GzipFileReader.html</anchorfile>
      <anchor>a5ebdbb98c1f78cb78883fccb11701bfd</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1GzipFileReader.html</anchorfile>
      <anchor>a16d150b28d80163e4cda48583cbf4c74</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::GzipFileWriter</name>
    <filename>classbyteme_1_1GzipFileWriter.html</filename>
    <base>byteme::Writer</base>
    <member kind="function">
      <type></type>
      <name>GzipFileWriter</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>abd9e2f19a9e4acf877343038fe736adf</anchor>
      <arglist>(const char *path, int compression_level=6, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GzipFileWriter</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>a39c958cd9bf253663299608cb6191232</anchor>
      <arglist>(const std::string &amp;path, int compression_level=6, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>a7408c1f7ad682e65113d94705f4f0997</anchor>
      <arglist>(const unsigned char *buffer, size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>add2b71c32bcd4cc6e2e6443d4219ffb3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>a1ecb1ea0461dfdb0f11dcadfb6fcc9fb</anchor>
      <arglist>(const std::string &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>aca9683db3d581100d802b9a0b884dcc3</anchor>
      <arglist>(const char *x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>a3e2f96d90f4304638ac6fe332bf64a46</anchor>
      <arglist>(char x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::IstreamReader</name>
    <filename>classbyteme_1_1IstreamReader.html</filename>
    <templarg>class Pointer_</templarg>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>IstreamReader</name>
      <anchorfile>classbyteme_1_1IstreamReader.html</anchorfile>
      <anchor>ada90bd58bab480c588ecbae6f149587c</anchor>
      <arglist>(Pointer_ input, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1IstreamReader.html</anchorfile>
      <anchor>aa09922b5fa256cca3ced445206e9a6dd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1IstreamReader.html</anchorfile>
      <anchor>a35ce4f9a45b6331d11be9cceb23ff80a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1IstreamReader.html</anchorfile>
      <anchor>a52f73d6079ea22af024bd074e6de8c79</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::OstreamWriter</name>
    <filename>classbyteme_1_1OstreamWriter.html</filename>
    <templarg>class Pointer_</templarg>
    <base>byteme::Writer</base>
    <member kind="function">
      <type></type>
      <name>OstreamWriter</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>a3cbc4a7eec70e40dd6a1c370f165084c</anchor>
      <arglist>(Pointer_ output)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>ad6dfbb6fb76c395c0f334ce6f9de7f81</anchor>
      <arglist>(const unsigned char *buffer, size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>accafb6643fbd952a9c036e14b3bf3cb7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>a1ecb1ea0461dfdb0f11dcadfb6fcc9fb</anchor>
      <arglist>(const std::string &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>aca9683db3d581100d802b9a0b884dcc3</anchor>
      <arglist>(const char *x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>a3e2f96d90f4304638ac6fe332bf64a46</anchor>
      <arglist>(char x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::PerByte</name>
    <filename>classbyteme_1_1PerByte.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class Pointer_</templarg>
    <member kind="function">
      <type></type>
      <name>PerByte</name>
      <anchorfile>classbyteme_1_1PerByte.html</anchorfile>
      <anchor>a3ce08a7c7d915293ba571841e66a29ba</anchor>
      <arglist>(Pointer_ reader)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>classbyteme_1_1PerByte.html</anchorfile>
      <anchor>a69828af5d6f2f64d8bf331066815917c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>advance</name>
      <anchorfile>classbyteme_1_1PerByte.html</anchorfile>
      <anchor>ae505a5be32c2f79313a779d32190e25f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Type_</type>
      <name>get</name>
      <anchorfile>classbyteme_1_1PerByte.html</anchorfile>
      <anchor>a58d1dabde5c62dd9235c0509e8072061</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>position</name>
      <anchorfile>classbyteme_1_1PerByte.html</anchorfile>
      <anchor>a003b805b826d56dd8117be66bbcc0d34</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>std::pair&lt; size_t, bool &gt;</type>
      <name>extract</name>
      <anchorfile>classbyteme_1_1PerByte.html</anchorfile>
      <anchor>a948f500bf0efb8d7f185715e609d5f16</anchor>
      <arglist>(size_t number, Type_ *output)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::PerByteParallel</name>
    <filename>classbyteme_1_1PerByteParallel.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class Pointer_</templarg>
    <member kind="function">
      <type></type>
      <name>PerByteParallel</name>
      <anchorfile>classbyteme_1_1PerByteParallel.html</anchorfile>
      <anchor>af8b6d8751483f0505569e0d739f4e834</anchor>
      <arglist>(Pointer_ reader)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>classbyteme_1_1PerByteParallel.html</anchorfile>
      <anchor>adf2beca863938a062dc172b7c656a85c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>advance</name>
      <anchorfile>classbyteme_1_1PerByteParallel.html</anchorfile>
      <anchor>a29a16a1bd5ff330b57a9686564a91687</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Type_</type>
      <name>get</name>
      <anchorfile>classbyteme_1_1PerByteParallel.html</anchorfile>
      <anchor>a1a0e9a739d270ed970292726a4723b96</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>position</name>
      <anchorfile>classbyteme_1_1PerByteParallel.html</anchorfile>
      <anchor>a68b5d094fa7ac5ddd74ca116e9f28ccd</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>std::pair&lt; size_t, bool &gt;</type>
      <name>extract</name>
      <anchorfile>classbyteme_1_1PerByteParallel.html</anchorfile>
      <anchor>a83a9f0dc44c7fdb90fe86e7fdaa5d626</anchor>
      <arglist>(size_t number, Type_ *output)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::RawBufferReader</name>
    <filename>classbyteme_1_1RawBufferReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>RawBufferReader</name>
      <anchorfile>classbyteme_1_1RawBufferReader.html</anchorfile>
      <anchor>a52edfb966cd7ce5db47ba2cbb3cd245d</anchor>
      <arglist>(const unsigned char *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RawBufferReader</name>
      <anchorfile>classbyteme_1_1RawBufferReader.html</anchorfile>
      <anchor>acb412b1fa76c1f74b4b9a1e34846d96c</anchor>
      <arglist>(const char *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1RawBufferReader.html</anchorfile>
      <anchor>a6405fcb9973d2836019db1d294a4d0cf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1RawBufferReader.html</anchorfile>
      <anchor>a9b38a2b7acc6c2e2f07cf2981053f8e3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1RawBufferReader.html</anchorfile>
      <anchor>aa7e496c2f99981b0e3724e804ba0f3b5</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::RawBufferWriter</name>
    <filename>classbyteme_1_1RawBufferWriter.html</filename>
    <base>byteme::Writer</base>
    <member kind="function">
      <type></type>
      <name>RawBufferWriter</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>af52cf05755d5330d60813dc6ef9e5752</anchor>
      <arglist>(size_t reserve=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>af48cfca7172a4e12afb75a9a59045295</anchor>
      <arglist>(const unsigned char *buffer, size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>a9f5da6446f98b72afcdbbcf504e10512</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; unsigned char &gt; &amp;</type>
      <name>get_output</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>a77085bb097a701c3b132f1278df923b8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>a1ecb1ea0461dfdb0f11dcadfb6fcc9fb</anchor>
      <arglist>(const std::string &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>aca9683db3d581100d802b9a0b884dcc3</anchor>
      <arglist>(const char *x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>a3e2f96d90f4304638ac6fe332bf64a46</anchor>
      <arglist>(char x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::RawFileReader</name>
    <filename>classbyteme_1_1RawFileReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>RawFileReader</name>
      <anchorfile>classbyteme_1_1RawFileReader.html</anchorfile>
      <anchor>a003ec82a4c79cc6baf05285da75acb9f</anchor>
      <arglist>(const char *path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RawFileReader</name>
      <anchorfile>classbyteme_1_1RawFileReader.html</anchorfile>
      <anchor>a0754fa094b5fcfe96995288b06d21086</anchor>
      <arglist>(const std::string &amp;path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1RawFileReader.html</anchorfile>
      <anchor>a1477482100fb90bc49d743ea0339d885</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1RawFileReader.html</anchorfile>
      <anchor>ae4d96df0332cc6b55f7c2702d87c6797</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1RawFileReader.html</anchorfile>
      <anchor>acd21606c899971a36f9a8b2112982c39</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::RawFileWriter</name>
    <filename>classbyteme_1_1RawFileWriter.html</filename>
    <base>byteme::Writer</base>
    <member kind="function">
      <type></type>
      <name>RawFileWriter</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>aec0092b0773fa4510ac45ad2e3612d71</anchor>
      <arglist>(const char *path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RawFileWriter</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>a4702fdac7150872c7bf98618f5eb6372</anchor>
      <arglist>(const std::string &amp;path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>ae7293d0c49e244bdcedeb49bbd023fba</anchor>
      <arglist>(const unsigned char *buffer, size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>a5cf864429e6b353e3d8a715ccdc4d535</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>a1ecb1ea0461dfdb0f11dcadfb6fcc9fb</anchor>
      <arglist>(const std::string &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>aca9683db3d581100d802b9a0b884dcc3</anchor>
      <arglist>(const char *x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>a3e2f96d90f4304638ac6fe332bf64a46</anchor>
      <arglist>(char x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::Reader</name>
    <filename>classbyteme_1_1Reader.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1Reader.html</anchorfile>
      <anchor>a60671d10f28ce83fa256d7342c769ba5</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1Reader.html</anchorfile>
      <anchor>a5b1d291e37d73e5e1dcb6219915c28f0</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1Reader.html</anchorfile>
      <anchor>a9a09114f17cef5b01171fd97dd0306b0</anchor>
      <arglist>() const =0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::SomeBufferReader</name>
    <filename>classbyteme_1_1SomeBufferReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>SomeBufferReader</name>
      <anchorfile>classbyteme_1_1SomeBufferReader.html</anchorfile>
      <anchor>ac01d576c1c03cf7cb31045405a0ab349</anchor>
      <arglist>(const unsigned char *buffer, size_t length, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SomeBufferReader</name>
      <anchorfile>classbyteme_1_1SomeBufferReader.html</anchorfile>
      <anchor>a34de7a22469161ca8c174dd61cf46d29</anchor>
      <arglist>(const char *buffer, size_t length, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1SomeBufferReader.html</anchorfile>
      <anchor>a12ae0af8ee14ca87e9e2fed95e3ed4ae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1SomeBufferReader.html</anchorfile>
      <anchor>a845dc3ba53b3ec9fde4edf1ef3c9b2ab</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1SomeBufferReader.html</anchorfile>
      <anchor>a8c5300a163ff203723ab87454b4bce6d</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::SomeFileReader</name>
    <filename>classbyteme_1_1SomeFileReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>SomeFileReader</name>
      <anchorfile>classbyteme_1_1SomeFileReader.html</anchorfile>
      <anchor>ac7ba4b558c83e39e2ff42bb8767cf963</anchor>
      <arglist>(const char *path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SomeFileReader</name>
      <anchorfile>classbyteme_1_1SomeFileReader.html</anchorfile>
      <anchor>a2267b7fcb53a5c78c415d2718bce06e0</anchor>
      <arglist>(const std::string &amp;path, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1SomeFileReader.html</anchorfile>
      <anchor>ac811bf5ceb6ee162201821403835b8bf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1SomeFileReader.html</anchorfile>
      <anchor>a88e3abc2c4f024f0003e8589e425033b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1SomeFileReader.html</anchorfile>
      <anchor>ae2bc3e4bcbf088dc674859f145294b96</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::Writer</name>
    <filename>classbyteme_1_1Writer.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>a58d7347c900dfd9df0b28b68ae34ca1b</anchor>
      <arglist>(const unsigned char *buffer, size_t n)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>a8317429c5645d0e447a4207091953164</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>a1ecb1ea0461dfdb0f11dcadfb6fcc9fb</anchor>
      <arglist>(const std::string &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>aca9683db3d581100d802b9a0b884dcc3</anchor>
      <arglist>(const char *x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>a3e2f96d90f4304638ac6fe332bf64a46</anchor>
      <arglist>(char x)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::ZlibBufferReader</name>
    <filename>classbyteme_1_1ZlibBufferReader.html</filename>
    <base>byteme::Reader</base>
    <member kind="function">
      <type></type>
      <name>ZlibBufferReader</name>
      <anchorfile>classbyteme_1_1ZlibBufferReader.html</anchorfile>
      <anchor>ab710f262473d4e675dfad66d1fb1885a</anchor>
      <arglist>(const unsigned char *buffer, size_t length, int mode=3, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>load</name>
      <anchorfile>classbyteme_1_1ZlibBufferReader.html</anchorfile>
      <anchor>ad7205cb26d279ca957ada858598a86a3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const unsigned char *</type>
      <name>buffer</name>
      <anchorfile>classbyteme_1_1ZlibBufferReader.html</anchorfile>
      <anchor>add07b8eea79991fc364205667b90a00b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>available</name>
      <anchorfile>classbyteme_1_1ZlibBufferReader.html</anchorfile>
      <anchor>a2317cd92892fafe298dec159ce3938d1</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::ZlibBufferWriter</name>
    <filename>classbyteme_1_1ZlibBufferWriter.html</filename>
    <base>byteme::Writer</base>
    <member kind="function">
      <type></type>
      <name>ZlibBufferWriter</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>a07c41d2092ad91f3ed63bde1a3f0ff9c</anchor>
      <arglist>(int mode=2, int compression_level=6, size_t buffer_size=65536)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>ab227f7b9f62f0bfa7ae2a1a20a7fabd1</anchor>
      <arglist>(const unsigned char *buffer, size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>a192a702db3794bf64766b0cee7ab4fac</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; unsigned char &gt; &amp;</type>
      <name>get_output</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>ac0c286b480ef272974c3ee3d4209ec0e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>a1ecb1ea0461dfdb0f11dcadfb6fcc9fb</anchor>
      <arglist>(const std::string &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>aca9683db3d581100d802b9a0b884dcc3</anchor>
      <arglist>(const char *x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>a3e2f96d90f4304638ac6fe332bf64a46</anchor>
      <arglist>(char x)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>byteme</name>
    <filename>namespacebyteme.html</filename>
    <class kind="class">byteme::ChunkedBufferReader</class>
    <class kind="class">byteme::GzipFileReader</class>
    <class kind="class">byteme::GzipFileWriter</class>
    <class kind="class">byteme::IstreamReader</class>
    <class kind="class">byteme::OstreamWriter</class>
    <class kind="class">byteme::PerByte</class>
    <class kind="class">byteme::PerByteParallel</class>
    <class kind="class">byteme::RawBufferReader</class>
    <class kind="class">byteme::RawBufferWriter</class>
    <class kind="class">byteme::RawFileReader</class>
    <class kind="class">byteme::RawFileWriter</class>
    <class kind="class">byteme::Reader</class>
    <class kind="class">byteme::SomeBufferReader</class>
    <class kind="class">byteme::SomeFileReader</class>
    <class kind="class">byteme::Writer</class>
    <class kind="class">byteme::ZlibBufferReader</class>
    <class kind="class">byteme::ZlibBufferWriter</class>
    <member kind="function">
      <type>std::string</type>
      <name>temp_file_path</name>
      <anchorfile>namespacebyteme.html</anchorfile>
      <anchor>a06ff6bac0c55a089e31301408937b058</anchor>
      <arglist>(const std::string &amp;prefix, const std::string &amp;ext)</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>temp_file_path</name>
      <anchorfile>namespacebyteme.html</anchorfile>
      <anchor>a94163b5d4600eeacc222e31540453b01</anchor>
      <arglist>(const std::string &amp;prefix)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Gimme some bytes</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="Gimme some bytes">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
