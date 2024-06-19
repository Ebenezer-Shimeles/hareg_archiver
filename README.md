# Hareg Archiver

Hareg Archiver is a robust and efficient file compression tool that utilizes LZSS and Huffman coding algorithms. Future developments include a user-friendly interface built with Flutter, additional algorithms like LZW and LZMA, file streaming support, and optimizations using CUDA and SIMD.

## Features

- **Currently Accomplished**
  -  &#9745; **LZSS**: A dictionary-based compression algorithm that replaces repeated occurrences of data with references to a single copy of that data existing earlier in the uncompressed data stream.
  - &#9745; **Huffman Coding**: An entropy encoding algorithm used for lossless data compression.
  - &#9745; **Simple File Format**: a simple file format to compress/decompress single files.

- **Planned Features**
  - &#9744; **Binary Search Tree Optimization for lzss matcher**: Optimizer the Linked List Matcher(O(N^2)) to Binary seartch Tree((O(log(n))))
  - &#9744; **Unit Test**: Test files using gtest.
  
  - &#9744; **Stream Compression**: Handle compression/decompression in a block style to handle file size that are greater than the ram.
  - &#9744; **Folders**: Compressing folders and files 
  - &#9744; **UI with Flutter**: A cross-platform UI toolkit to provide a seamless and interactive user experience.
  - **Additional Compression Algorithms**:
    - &#9744; **LZW**: A dictionary-based compression algorithm that works by replacing strings of characters with single codes.
    - &#9744; **LZMA**: An algorithm that achieves a high compression ratio by using a dictionary compression scheme.
    - &#9744; **Bzip**: A block-sorting compression algorithm that uses the Burrows–Wheeler transform.
    - &#9744; **Delta Compression**
  - &#9744; **File Streaming Support**: Capability to handle continuous data streams for compression and decompression.
  - &#9744; **CUDA Optimizations**: Utilize NVIDIA's parallel computing platform to enhance performance.
  - &#9744; **SIMD Optimizations**: Use Single Instruction, Multiple Data techniques to process data more efficiently.
  - &#9744; **General Optimizations**

## Getting Started

### Prerequisites

- C++ Compiler
- CMake
- Git

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/hareg-archiver.git
   cd hareg-archiver
   cmake .
