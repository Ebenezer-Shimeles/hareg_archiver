#pragma once

#include <cstdint>

#include <common/common.h>
PACK(struct HuffmanBlockInfoNode{
		uint8_t sym_len;
		int32_t freqs[256]{};
		
});
	