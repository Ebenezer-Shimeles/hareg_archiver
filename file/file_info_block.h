#pragma once

#include <common/common.h>
#include <compression/compression_type.h>

#include <cstdint>
#include <cstdio>

namespace file{
        PACK(struct FileInfoBlock {
            uint8_t lzzs_sym; //no of added bits to lzss bit stream to make it modulo 8
            uint8_t file_name[FILENAME_MAX]{};
            size_t compressed_length;
            size_t decompresssed_length;
            CompressionType comp_type; //todo:: add this only to HaregFleInfo


        });
}