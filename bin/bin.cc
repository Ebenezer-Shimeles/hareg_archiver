#include <string>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <cstring>

#include <common/hareg_error.h>
#include <common/hareg_val.h>

namespace bin{

	std::string to_binary(uint8_t num){
		std::string rv = "";
		while(num > 0){
			//since strings in c++ are mutable this don't matter
			if(num & 1){
				rv += '1';
			}else{
				rv += '0';
			}
			num /=2; 
		}
	    while(rv.size() < 8){
	    	rv += '0';
		}
		return std::string(rv.rbegin(), rv.rend());
	}

	
	uint8_t symetrify(std::string& str){
		auto len = str.size();
		if(len % 8 == 0){
			return 0 ;
		}
		auto left = 8 - (len % 8);
		for(int8_t i=0;i < left;i++){
			str += "0";
		}
		return left;
		
		
	}
	uint8_t get_char_from_bin(const char* bins,const size_t& bins_len, const int64_t& start){
		uint8_t rv = 0;
		uint8_t p = 128;
		for(int64_t i=start;i<start+8;i++){
			if(bins[i] == '1'){
				rv += p;
			}
			p /= 2;
			
		}
		return rv;
	}
	uint8_t get_char_from_bin(const std::string& bins,const int64_t& start){

		return get_char_from_bin(bins.c_str(), bins.size(), start);
	}
	
	HaregVal<std::unique_ptr<uint8_t[]>> to_chars(const std::string& bins, size_t& return_length){
	//	std::cout << "Bins: " << bins << '\n';
		auto bin_sz = bins.size();
		auto ch_sz = bin_sz/8;
		return_length = ch_sz;
	//	std::cout << "bins size: " << bin_sz << '\n'; 
	//	std::cout << "char size: " <<ch_sz <<'\n';
		
	    auto rv = std::unique_ptr<uint8_t[]>(new uint8_t[ch_sz]);
	    memset(rv.get(), '\0', ch_sz);
	    
	    auto raw_ptr = rv.get();
	    
	    for(int64_t i=0;i<bin_sz;i+=8){
	    	auto ch_index = i/8;
	    	auto ch =  get_char_from_bin(bins, i);
	    //	std::cout << ch << ' ';
	    	
	    	raw_ptr[ch_index] = ch;
		}
	    return std::move(rv);
	}
	
	std::string str_to_bins(const char* str,const size_t& l){
		std::string rv="";
		
		for(int32_t i=0;i<l;i++){
			auto& ch = str[i];
			rv += to_binary(ch);
		}
	
		return rv;
		
	}
	std::string str_to_bins(const std::string& str){
	    return str_to_bins(str.c_str(), str.size());
		
	}
	
}
