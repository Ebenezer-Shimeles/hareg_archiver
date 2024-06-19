#pragma once

#include <common/hareg_error.h>
#include <common/hareg_val.h>
#include <common/common.h>
#include <cstdio>


#include <compression/compression_type.h>

/**
 * Contains files that join compression algorithsm
 * TODO: add store
 */

namespace compression {

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_compress_buffer(const char* buffer, const size_t& buffer_size, 
                                                                               size_t& compressed_len,
                                                                               uint8_t& lzss_sym_len,
                                                                               CompressionType mode = LZSS_HUFFMAN_4KB);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_compress(const std::string& buffer, size_t& compressed_len,
                                                                        uint8_t& lzss_sym_len,
                                                                        CompressionType mode = LZSS_HUFFMAN_4KB);

    HaregVal<void> unoptimized_nonstream_compress_file(const char *input_file_name, const size_t& input_file_name_size, 
                                                       const CompressionType& mode = LZSS_HUFFMAN_4KB,
                                                       char *output_file_name = "", size_t output_file_name_size = 0);

    HaregVal<void> unoptimized_nonstream_decompress_file(const char* file_name, const size_t& file_name_len);

}
