#pragma once

#include <string>
#include <cstdint>
#include <cstddef>

#include <common/hareg_error.h>
#include <common/hareg_val.h>
#include <memory>


/**
 * This namespace contains files related to binary operations
 * Optmizations are needed here especially in memory usage
 */

namespace bin{
  
    std::string to_binary(uint8_t num);
    uint8_t symetrify(std::string& str);
    uint8_t get_char_from_bin(const char* bins, const size_t& bins_len, const int64_t& start);
    uint8_t get_char_from_bin(const std::string& bins, const int64_t& start);
    HaregVal<std::unique_ptr<uint8_t[]>> to_chars(const std::string& bins, size_t& return_length);
    std::string str_to_bins(const char* str, const size_t& l);
    std::string str_to_bins(const std::string& str);

}
