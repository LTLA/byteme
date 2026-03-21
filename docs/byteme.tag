<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>BufferedReader.hpp</name>
    <path>byteme/</path>
    <filename>BufferedReader_8hpp.html</filename>
    <class kind="class">byteme::BufferedReader</class>
    <class kind="class">byteme::SerialBufferedReader</class>
    <class kind="class">byteme::ParallelBufferedReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>BufferedWriter.hpp</name>
    <path>byteme/</path>
    <filename>BufferedWriter_8hpp.html</filename>
    <class kind="class">byteme::BufferedWriter</class>
    <class kind="class">byteme::SerialBufferedWriter</class>
    <class kind="class">byteme::ParallelBufferedWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>byteme.hpp</name>
    <path>byteme/</path>
    <filename>byteme_8hpp.html</filename>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>GzipFileReader.hpp</name>
    <path>byteme/</path>
    <filename>GzipFileReader_8hpp.html</filename>
    <class kind="struct">byteme::GzipFileReaderOptions</class>
    <class kind="class">byteme::GzipFileReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>GzipFileWriter.hpp</name>
    <path>byteme/</path>
    <filename>GzipFileWriter_8hpp.html</filename>
    <class kind="struct">byteme::GzipFileWriterOptions</class>
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
    <name>magic_numbers.hpp</name>
    <path>byteme/</path>
    <filename>magic__numbers_8hpp.html</filename>
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
    <class kind="struct">byteme::RawBufferWriterOptions</class>
    <class kind="class">byteme::RawBufferWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>RawFileReader.hpp</name>
    <path>byteme/</path>
    <filename>RawFileReader_8hpp.html</filename>
    <class kind="struct">byteme::RawFileReaderOptions</class>
    <class kind="class">byteme::RawFileReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>RawFileWriter.hpp</name>
    <path>byteme/</path>
    <filename>RawFileWriter_8hpp.html</filename>
    <class kind="struct">byteme::RawFileWriterOptions</class>
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
    <class kind="struct">byteme::ZlibBufferReaderOptions</class>
    <class kind="class">byteme::ZlibBufferReader</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="file">
    <name>ZlibBufferWriter.hpp</name>
    <path>byteme/</path>
    <filename>ZlibBufferWriter_8hpp.html</filename>
    <class kind="struct">byteme::ZlibBufferWriterOptions</class>
    <class kind="class">byteme::ZlibBufferWriter</class>
    <namespace>byteme</namespace>
  </compound>
  <compound kind="class">
    <name>byteme::BufferedReader</name>
    <filename>classbyteme_1_1BufferedReader.html</filename>
    <templarg>typename Type_</templarg>
    <member kind="function">
      <type>bool</type>
      <name>advance</name>
      <anchorfile>classbyteme_1_1BufferedReader.html</anchorfile>
      <anchor>ab2267652e79cc367a079d5d0af0af3b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Type_</type>
      <name>get</name>
      <anchorfile>classbyteme_1_1BufferedReader.html</anchorfile>
      <anchor>aa28ff2d69e79103078a842cc0d295b45</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>unsigned long long</type>
      <name>position</name>
      <anchorfile>classbyteme_1_1BufferedReader.html</anchorfile>
      <anchor>ab2e8c16f833432e10414208a89fb26c6</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>valid</name>
      <anchorfile>classbyteme_1_1BufferedReader.html</anchorfile>
      <anchor>a14efd33907a2c64607bc1733aa17aecc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>std::pair&lt; std::size_t, bool &gt;</type>
      <name>extract</name>
      <anchorfile>classbyteme_1_1BufferedReader.html</anchorfile>
      <anchor>a1c15c13a980c5f5a99d6a780317c413e</anchor>
      <arglist>(std::size_t number, Type_ *output)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::BufferedWriter</name>
    <filename>classbyteme_1_1BufferedWriter.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class WriterPointer_</templarg>
    <member kind="function">
      <type>unsigned long long</type>
      <name>number</name>
      <anchorfile>classbyteme_1_1BufferedWriter.html</anchorfile>
      <anchor>affe873267293a3ef93e86280cf70bea0</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1BufferedWriter.html</anchorfile>
      <anchor>ac03128fe53c7fc631feb313f9ba723ea</anchor>
      <arglist>(Type_ input)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1BufferedWriter.html</anchorfile>
      <anchor>a4c903547482d7c9c46cf8d8cf2899781</anchor>
      <arglist>(const Type_ *input, std::size_t number)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1BufferedWriter.html</anchorfile>
      <anchor>a86e83c82b20d4f78b50a5392e2bdb90a</anchor>
      <arglist>(const char *string)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>flush</name>
      <anchorfile>classbyteme_1_1BufferedWriter.html</anchorfile>
      <anchor>a260d914d0ab15662751589d0c521b66f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1BufferedWriter.html</anchorfile>
      <anchor>a198e776adf0a3c616d298d59859db282</anchor>
      <arglist>()=0</arglist>
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
      <anchor>aecf4eebc3a244525b2c71a269d112800</anchor>
      <arglist>(const char *path, const GzipFileReaderOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classbyteme_1_1GzipFileReader.html</anchorfile>
      <anchor>a4e293b837f53b17cbdaac22e0305d669</anchor>
      <arglist>(unsigned char *buffer, std::size_t n)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>byteme::GzipFileReaderOptions</name>
    <filename>structbyteme_1_1GzipFileReaderOptions.html</filename>
    <member kind="variable">
      <type>std::optional&lt; unsigned &gt;</type>
      <name>buffer_size</name>
      <anchorfile>structbyteme_1_1GzipFileReaderOptions.html</anchorfile>
      <anchor>ae263b9d0fe4f7d7c0bd922fe8cc19cb5</anchor>
      <arglist></arglist>
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
      <anchor>ad3b9a74789a8182cc40f3ba2d4edf306</anchor>
      <arglist>(const char *path, const GzipFileWriterOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>a5d30e2b029f3a45433ac32d9074a3088</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1GzipFileWriter.html</anchorfile>
      <anchor>add2b71c32bcd4cc6e2e6443d4219ffb3</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>byteme::GzipFileWriterOptions</name>
    <filename>structbyteme_1_1GzipFileWriterOptions.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>compression_level</name>
      <anchorfile>structbyteme_1_1GzipFileWriterOptions.html</anchorfile>
      <anchor>a9072c378aefb629c314f80ff8b77a964</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::optional&lt; unsigned &gt;</type>
      <name>gzbuffer_size</name>
      <anchorfile>structbyteme_1_1GzipFileWriterOptions.html</anchorfile>
      <anchor>ad3ae50598f977a1063e4cd9b438d2dcc</anchor>
      <arglist></arglist>
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
      <anchor>a9ec92866979a86aa6a654068b734224c</anchor>
      <arglist>(Pointer_ input)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classbyteme_1_1IstreamReader.html</anchorfile>
      <anchor>aced8bbf4560953a5c331c6ec18edd742</anchor>
      <arglist>(unsigned char *buffer, std::size_t n)</arglist>
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
      <anchor>abdc25527304a9a7d006a56005bc22b84</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1OstreamWriter.html</anchorfile>
      <anchor>accafb6643fbd952a9c036e14b3bf3cb7</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::ParallelBufferedReader</name>
    <filename>classbyteme_1_1ParallelBufferedReader.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class Pointer_</templarg>
    <base>byteme::BufferedReader&lt; Type_ &gt;</base>
    <member kind="function">
      <type></type>
      <name>ParallelBufferedReader</name>
      <anchorfile>classbyteme_1_1ParallelBufferedReader.html</anchorfile>
      <anchor>ab0097f7773c2b49bd9d96f6bd57c3f94</anchor>
      <arglist>(Pointer_ reader, std::size_t buffer_size)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::ParallelBufferedWriter</name>
    <filename>classbyteme_1_1ParallelBufferedWriter.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class Pointer_</templarg>
    <base>byteme::BufferedWriter&lt; Type_, WriterPointer_ &gt;</base>
    <member kind="function">
      <type></type>
      <name>ParallelBufferedWriter</name>
      <anchorfile>classbyteme_1_1ParallelBufferedWriter.html</anchorfile>
      <anchor>a269dc96008f8783973324b72b605f85d</anchor>
      <arglist>(Pointer_ writer, std::size_t buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1ParallelBufferedWriter.html</anchorfile>
      <anchor>a9161f8d9c967e2f78ff3b01a6d43d79b</anchor>
      <arglist>()</arglist>
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
      <anchor>a80988c695311f5ebd52680bf93f92847</anchor>
      <arglist>(const unsigned char *buffer, std::size_t length)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classbyteme_1_1RawBufferReader.html</anchorfile>
      <anchor>aa2ed5c3a15c37cb69e0ad6a6141f6441</anchor>
      <arglist>(unsigned char *buffer, std::size_t n)</arglist>
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
      <anchor>a2536f89eb9c6a58a68269abfa907f3bd</anchor>
      <arglist>(const RawBufferWriterOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawBufferWriter.html</anchorfile>
      <anchor>a64f231b9bd5ce618ccf78ec9f0a5f0cc</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
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
  </compound>
  <compound kind="struct">
    <name>byteme::RawBufferWriterOptions</name>
    <filename>structbyteme_1_1RawBufferWriterOptions.html</filename>
    <member kind="variable">
      <type>std::size_t</type>
      <name>reserve</name>
      <anchorfile>structbyteme_1_1RawBufferWriterOptions.html</anchorfile>
      <anchor>ace854857d6b1b607ff4fedca07d05e1b</anchor>
      <arglist></arglist>
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
      <anchor>a8408ecd4c0c0d0c03d84b205f941f8d2</anchor>
      <arglist>(const char *path, const RawFileReaderOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classbyteme_1_1RawFileReader.html</anchorfile>
      <anchor>a42ba2360c659f3365a79d0654e26f737</anchor>
      <arglist>(unsigned char *buffer, std::size_t n)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>byteme::RawFileReaderOptions</name>
    <filename>structbyteme_1_1RawFileReaderOptions.html</filename>
    <member kind="variable">
      <type>std::optional&lt; std::size_t &gt;</type>
      <name>buffer_size</name>
      <anchorfile>structbyteme_1_1RawFileReaderOptions.html</anchorfile>
      <anchor>a9e418f946361c2f816b449794acd3c9d</anchor>
      <arglist></arglist>
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
      <anchor>a85b813251f5ce733364d3b02914ad209</anchor>
      <arglist>(const char *path, const RawFileWriterOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>aaa50768a47322b9bd793592fe6307bcc</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1RawFileWriter.html</anchorfile>
      <anchor>a5cf864429e6b353e3d8a715ccdc4d535</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>byteme::RawFileWriterOptions</name>
    <filename>structbyteme_1_1RawFileWriterOptions.html</filename>
    <member kind="variable">
      <type>std::optional&lt; std::size_t &gt;</type>
      <name>bufsiz</name>
      <anchorfile>structbyteme_1_1RawFileWriterOptions.html</anchorfile>
      <anchor>a968198bb36d902d8f17aa0a6b1786ff4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::Reader</name>
    <filename>classbyteme_1_1Reader.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual std::size_t</type>
      <name>read</name>
      <anchorfile>classbyteme_1_1Reader.html</anchorfile>
      <anchor>ac39e5bfe2feae64643a177ba7dcb7fa9</anchor>
      <arglist>(unsigned char *buffer, std::size_t n)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::SerialBufferedReader</name>
    <filename>classbyteme_1_1SerialBufferedReader.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class Pointer_</templarg>
    <base>byteme::BufferedReader&lt; Type_ &gt;</base>
    <member kind="function">
      <type></type>
      <name>SerialBufferedReader</name>
      <anchorfile>classbyteme_1_1SerialBufferedReader.html</anchorfile>
      <anchor>a0171352dd5853d7fdbbcff353de3a379</anchor>
      <arglist>(Pointer_ reader, std::size_t buffer_size)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::SerialBufferedWriter</name>
    <filename>classbyteme_1_1SerialBufferedWriter.html</filename>
    <templarg>typename Type_</templarg>
    <templarg>class Pointer_</templarg>
    <base>byteme::BufferedWriter&lt; Type_, WriterPointer_ &gt;</base>
    <member kind="function">
      <type></type>
      <name>SerialBufferedWriter</name>
      <anchorfile>classbyteme_1_1SerialBufferedWriter.html</anchorfile>
      <anchor>a8a01232e87e3ab7512225fc4ebf747f2</anchor>
      <arglist>(Pointer_ writer, std::size_t buffer_size)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1SerialBufferedWriter.html</anchorfile>
      <anchor>a3a0701de854de3031d406dd2a7175939</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>byteme::Writer</name>
    <filename>classbyteme_1_1Writer.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Writer</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>ad385ffe4a1d27fe4d7db5c5425cf4588</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>a8fee63f29e632dd029eff3c8e06020a0</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>finish</name>
      <anchorfile>classbyteme_1_1Writer.html</anchorfile>
      <anchor>a8317429c5645d0e447a4207091953164</anchor>
      <arglist>()=0</arglist>
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
      <anchor>a3cd5f4e25a1b3744237f6e6811d231dd</anchor>
      <arglist>(const unsigned char *buffer, std::size_t length, const ZlibBufferReaderOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>std::size_t</type>
      <name>read</name>
      <anchorfile>classbyteme_1_1ZlibBufferReader.html</anchorfile>
      <anchor>a932af82edf6c8d316f0fb9b54d032179</anchor>
      <arglist>(unsigned char *buffer, std::size_t n)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>byteme::ZlibBufferReaderOptions</name>
    <filename>structbyteme_1_1ZlibBufferReaderOptions.html</filename>
    <member kind="variable">
      <type>std::optional&lt; ZlibCompressionMode &gt;</type>
      <name>mode</name>
      <anchorfile>structbyteme_1_1ZlibBufferReaderOptions.html</anchorfile>
      <anchor>a80c0baf0a1085d9810f0aae0f993d125</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::size_t</type>
      <name>buffer_size</name>
      <anchorfile>structbyteme_1_1ZlibBufferReaderOptions.html</anchorfile>
      <anchor>ac5780ce7c036cc541c644241d307bf0a</anchor>
      <arglist></arglist>
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
      <anchor>a09d7572eb0c546087fb66c13aa8e2d84</anchor>
      <arglist>(const ZlibBufferWriterOptions &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>write</name>
      <anchorfile>classbyteme_1_1ZlibBufferWriter.html</anchorfile>
      <anchor>a7cb49e8471cfc494af1341e16f81403a</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
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
  </compound>
  <compound kind="struct">
    <name>byteme::ZlibBufferWriterOptions</name>
    <filename>structbyteme_1_1ZlibBufferWriterOptions.html</filename>
    <member kind="variable">
      <type>ZlibCompressionMode</type>
      <name>mode</name>
      <anchorfile>structbyteme_1_1ZlibBufferWriterOptions.html</anchorfile>
      <anchor>acc4d28aae986d64257b017231ecc6478</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>compression_level</name>
      <anchorfile>structbyteme_1_1ZlibBufferWriterOptions.html</anchorfile>
      <anchor>a31ad029c8bd29c6edfe6e1228ee2aab7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::size_t</type>
      <name>buffer_size</name>
      <anchorfile>structbyteme_1_1ZlibBufferWriterOptions.html</anchorfile>
      <anchor>ae3577ab909e788c1cfffaebafc9994c0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>byteme</name>
    <filename>namespacebyteme.html</filename>
    <class kind="class">byteme::BufferedReader</class>
    <class kind="class">byteme::BufferedWriter</class>
    <class kind="class">byteme::GzipFileReader</class>
    <class kind="struct">byteme::GzipFileReaderOptions</class>
    <class kind="class">byteme::GzipFileWriter</class>
    <class kind="struct">byteme::GzipFileWriterOptions</class>
    <class kind="class">byteme::IstreamReader</class>
    <class kind="class">byteme::OstreamWriter</class>
    <class kind="class">byteme::ParallelBufferedReader</class>
    <class kind="class">byteme::ParallelBufferedWriter</class>
    <class kind="class">byteme::RawBufferReader</class>
    <class kind="class">byteme::RawBufferWriter</class>
    <class kind="struct">byteme::RawBufferWriterOptions</class>
    <class kind="class">byteme::RawFileReader</class>
    <class kind="struct">byteme::RawFileReaderOptions</class>
    <class kind="class">byteme::RawFileWriter</class>
    <class kind="struct">byteme::RawFileWriterOptions</class>
    <class kind="class">byteme::Reader</class>
    <class kind="class">byteme::SerialBufferedReader</class>
    <class kind="class">byteme::SerialBufferedWriter</class>
    <class kind="class">byteme::Writer</class>
    <class kind="class">byteme::ZlibBufferReader</class>
    <class kind="struct">byteme::ZlibBufferReaderOptions</class>
    <class kind="class">byteme::ZlibBufferWriter</class>
    <class kind="struct">byteme::ZlibBufferWriterOptions</class>
    <member kind="enumeration">
      <type></type>
      <name>ZlibCompressionMode</name>
      <anchorfile>namespacebyteme.html</anchorfile>
      <anchor>a684463951a944a548255a6abe484ef2b</anchor>
      <arglist></arglist>
      <enumvalue file="namespacebyteme.html" anchor="a684463951a944a548255a6abe484ef2baf17e827fa227b759dc5c053b334d77cc">DEFLATE</enumvalue>
      <enumvalue file="namespacebyteme.html" anchor="a684463951a944a548255a6abe484ef2ba87c23ff88ac9b1c4390a679b63ea3b8c">ZLIB</enumvalue>
      <enumvalue file="namespacebyteme.html" anchor="a684463951a944a548255a6abe484ef2baf1cd28417b03cfab3a8e16d66e34e18a">GZIP</enumvalue>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_zlib</name>
      <anchorfile>namespacebyteme.html</anchorfile>
      <anchor>ad3b16db0612e6afdf861519e51d71653</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_gzip</name>
      <anchorfile>namespacebyteme.html</anchorfile>
      <anchor>acb08f1cce34b8b237e3174b307a34178</anchor>
      <arglist>(const unsigned char *buffer, std::size_t n)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>is_gzip</name>
      <anchorfile>namespacebyteme.html</anchorfile>
      <anchor>a079575e91360061a7495713a7e872258</anchor>
      <arglist>(const char *path)</arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Gimme some bytes</title>
    <filename>index.html</filename>
    <docanchor file="index.html">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
