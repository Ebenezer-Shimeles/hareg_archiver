#pragma once

#include <lzss/linked_list_matcher.h>
#include <common/common.h>
#include <memory>
#include <compression/compression_type.h>
#include <string>
#include <common/hareg_val.h>
#include <lzss/dictionary.h>

#include <lzss/lzss_block_info.h>

namespace lzss {

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_compress(const char* buffer, const size_t& buffer_size, 
                                                                             size_t& return_len, uint8_t& sym_len, 
                                                                             const CompressionType& mode = LZSS_HUFFMAN_4KB);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_compress(const std::string& buffer, size_t& return_len, uint8_t& sym_len);
    
    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_decompress(const char* buffer, const size_t& buffer_size, 
                                                                               const uint8_t& sym_len, const size_t& decompressed_len, 
                                                                               const CompressionType& mode = LZSS_HUFFMAN_4KB);
    
    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_decompress(const std::string& buffer, const uint8_t& sym_len, const size_t& uncompressed_size);

}
