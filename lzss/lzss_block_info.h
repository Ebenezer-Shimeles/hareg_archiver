#include <cstdint>
#pragma once

struct LzssBlockInfo{
		uint8_t sym_len;
		size_t decompressed_size;
};
	