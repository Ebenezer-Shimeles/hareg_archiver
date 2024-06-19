# Hareg Archiver

Hareg Archiver is a robust and efficient file compression tool that utilizes LZSS and Huffman coding algorithms. Future developments include a user-friendly interface built with Flutter, additional algorithms like LZW and LZMA, file streaming support, and optimizations using CUDA and SIMD.

## Features

- **Compression Algorithms**
  - **LZSS**: A dictionary-based compression algorithm that replaces repeated occurrences of data with references to a single copy of that data existing earlier in the uncompressed data stream.
  - **Huffman Coding**: An entropy encoding algorithm used for lossless data compression.

- **Planned Features**
  - **UI with Flutter**: A cross-platform UI toolkit to provide a seamless and interactive user experience.
  - **Additional Compression Algorithms**:
    - **LZW**: A dictionary-based compression algorithm that works by replacing strings of characters with single codes.
    - **LZMA**: An algorithm that achieves a high compression ratio by using a dictionary compression scheme.
    - **Bzip**: A block-sorting compression algorithm that uses the Burrows–Wheeler transform.
  - **File Streaming Support**: Capability to handle continuous data streams for compression and decompression.
  - **CUDA Optimizations**: Utilize NVIDIA's parallel computing platform to enhance performance.
  - **SIMD Optimizations**: Use Single Instruction, Multiple Data techniques to process data more efficiently.

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
