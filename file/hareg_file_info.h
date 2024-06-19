#pragma once

#include <cstdint>	

namespace file{
	struct HaregFileInfo{
			uint8_t magic[3] = {(uint8_t)'H',(uint8_t)'?',(uint8_t)'B'};
			uint8_t version; //hareg format version not hareg(software)'s version	
		
	};
}