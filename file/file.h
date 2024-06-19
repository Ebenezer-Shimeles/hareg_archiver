#pragma once

#include <common/hareg_error.h>
#include <common/hareg_val.h>
#include <common/common.h>
#include <cstdio>
#include <file/file_info_block.h>

namespace file{

    [[nodiscard]]
    size_t get_file_size(FILE *fp);

    [[nodiscard]]
    HaregVal<std::unique_ptr<char[]>> get_buffer_of(const char* file_name, size_t& buffer_size);

    [[nodiscard]]
    HaregVal<void> save_buffer_on_file(const char* file_name, const char* buffer, const size_t& buffer_size);

    HaregVal<void> append_buffer_on_file(const char* file_name, const char* buffer, const size_t& buffer_size);

}
