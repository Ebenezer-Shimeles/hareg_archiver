#pragma once

#include <common/common.h>
#include <cstdint>


PACK(struct LzssToken4KB{
		uint16_t back_ptr: 12;
		uint16_t copy_len: 4;
});
	
	
PACK(struct LzssToken8KB{
		uint16_t back_ptr: 13;
		uint16_t copy_len: 11;
});
	
PACK(struct LzssToken32KB{
		uint16_t back_ptr: 15;
		uint32_t copy_len: 17;
});
		
PACK(struct LzssToken64KB{
		uint16_t back_ptr;
		uint32_t copy_len: 24;
});

constexpr size_t WINDOW_SIZE_4KB = 4 * 1024;
constexpr size_t WINDOW_SIZE_8KB = 8 * 1024;
constexpr size_t WINDOW_SIZE_32KB = 32 * 1024;
constexpr size_t WINDOW_SIZE_64KB = 64 * 1024;

constexpr size_t MAX_COPY_LEN_4KB = 15;
constexpr size_t MAX_COPY_LEN_8KB = 2047;
constexpr size_t MAX_COPY_LEN_32KB = 131071;
constexpr size_t MAX_COPY_LEN_64KB = 16777215;
