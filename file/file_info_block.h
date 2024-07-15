#pragma once

#include <common/common.h>
#include <compression/compression_type.h>
#include <cstddef>
#include <cstdint>
#include <cstddef>

#include <cstdio>

namespace file{
        PACK(struct FileInfoBlock {
            uint8_t lzzs_sym; //no of added bits to lzss bit stream to make it modulo 8
            //uint8_t file_name[FILENAME_MAX]{}; //DAMN THIS SUCKED 
            size_t compressed_length;
            size_t decompresssed_length;
            uint8_t comp_type; //todo:: add this only to HaregFleInfo
            uint8_t file_name_size;
        });
}