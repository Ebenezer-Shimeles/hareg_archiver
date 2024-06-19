#pragma once

template<class T>
struct LzStringMatch{
		bool is_match;
		T token;
		
		
		LzStringMatch():is_match(false){}
		LzStringMatch(uint16_t b_offset, uint16_t c_sz) : is_match(true), token({b_offset, c_sz}){};
		
};