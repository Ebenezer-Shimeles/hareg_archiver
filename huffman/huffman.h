#pragma once

#include <huffman/huffman_block_info_node.h>
#include <huffman/huffman_tree_node.h>
#include <common/hareg_val.h>
#include <common/hareg_error.h>
#include <bin/bin.h>
#include <memory>
#include <cstddef>
#include <string>
namespace huffman {

    HaregVal<void> get_char_codes_from_tree(HuffmanNode* root, std::string curr_code, std::string* map);

    HaregVal<HuffmanNode> get_huffman_tree(const int32_t* freqs);

    [[nodiscard]]
    HaregVal<std::unique_ptr<std::string[]>> get_char_codes_from_freqs(const int32_t * freqs);


    [[nodiscard]]
    std::unique_ptr<int32_t[]> count_freqs(const char* buffer, const size_t& size);

    HaregVal<uint8_t> get_next_char_from_huffman_tree(HuffmanNode* root, const std::string& codes, size_t& offset);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decode(const char* str, const size_t& len,
                                                                              const int32_t *freqs,
                                                                              const uint8_t& sym_len, size_t& return_len);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decode(const std::string& str, const int32_t* freqs, 
                                                                              const uint8_t& sym_len, size_t& return_len);

    HaregVal<void> unoptimized_nonstream_huffman_encode(const char* str, const size_t& size, std::string& rv, uint8_t& sym_len);

    HaregVal<void> unoptimized_nonstream_huffman_encode(const std::string& str, std::string& rv, uint8_t& sym_len);

    [[nodiscard]]
    std::unique_ptr<int32_t[]> count_freqs(const std::string& str);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_compress_buffer(const char* buffer, const size_t& buffer_len, size_t& return_len);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_compress_buffer(const std::string& buffer, size_t& return_len);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decompress_buffer(const char* buffer, const size_t& buffer_len, size_t& return_len);

    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decompress_buffer(const std::string& buffer, size_t& return_len);

}
