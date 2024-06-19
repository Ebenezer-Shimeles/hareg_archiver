#include <cstdint>

#pragma once

namespace lzss{
	struct LinkedListNode{
			uint8_t char_val;
			LinkedListNode *next = nullptr;
			size_t id;
			static size_t count;
			LinkedListNode(){
				id = count ++;
			}
			
	};
}
