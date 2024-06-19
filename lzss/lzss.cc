#include <lzss/lzss.h>
#include <lzss/linked_list_matcher.h>
#include <bin/bin.h>
size_t lzss::LinkedListNode::count = 0;
namespace lzss{

	
	




	/*
	   unoptimized in the way it uses bits as strings 
	   need to fix that
	*/
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_compress(const char* buffer, const size_t& buffer_size, 
																			size_t& return_len, 	uint8_t& sym_len, 
																			const CompressionType& mode){
																				
		std::string bit_string = "";
		lzss::LinkedListMatcher matcher(mode);
		
		for(size_t i= 0; i < buffer_size; i++){
    	//std::cout << matcher;
    	    bool is_match = false;
    	    char* token_buffer;
    	    size_t copy_len;
    	    size_t token_size = 0;
    	    if(mode == LZSS_HUFFMAN_4KB){
    	    	// auto match_tpl = matcher.find_largest_match<LzssToken4KB>(buffer, buffer_size, i);
    	    	// is_match = match_tpl.is_match;
    	    	// token_size = sizeof(LzssToken4KB);
    	    	// token_buffer = new char[token_size];
    	    	// memcpy(token_buffer, &match_tpl.token, token_size);
    	    	// copy_len = match_tpl.token.copy_len;
    	    	
			}else if(mode == LZSS_HUFFMAN_8KB){
				// auto match_tpl = matcher.find_largest_match<LzssToken8KB>(buffer, buffer_size, i);
    	    	// is_match = match_tpl.is_match;
    	    	// token_size = sizeof(LzssToken8KB);
    	    	// token_buffer = new char[token_size];
    	    	// memcpy(token_buffer, &match_tpl.token, token_size);
    	    	// copy_len = match_tpl.token.copy_len;
				
			}else if(mode == LZSS_HUFFMAN_32KB){
				// auto match_tpl = matcher.find_largest_match<LzssToken32KB>(buffer, buffer_size, i);
    	    	// is_match = match_tpl.is_match;
    	    	// token_size = sizeof(LzssToken32KB);
    	    	// token_buffer = new char[token_size];
    	    	// memcpy(token_buffer, &match_tpl.token, token_size);
    	    	// copy_len = match_tpl.token.copy_len;
				
			}else if(mode == LZSS_HUFFMAN_64KB){
				// auto match_tpl = matcher.find_largest_match<LzssToken64KB>(buffer, buffer_size, i);
    	    	// is_match = match_tpl.is_match;
    	    	// token_size = sizeof(LzssToken64KB);
    	    	// token_buffer = new char[token_size];
    	    	// memcpy(token_buffer, &match_tpl.token, token_size);
    	    	// copy_len = match_tpl.token.copy_len;
				
			}else{
				return HaregError("Error unknown dictionary type given!", __FILE__, __LINE__);
			}
	    	
	    	if(is_match){
//	    		std::cout <<"<" << match_tpl.token.back_ptr << ", " << match_tpl.token.copy_len << ">";
                bit_string += '1'; //signifies the block next is compressed
                auto block_as_string = bin::str_to_bins(token_buffer, token_size);
                bit_string += block_as_string;
	
	    		for(int64_t j = i; j< i+ copy_len;j++){
	    			matcher.add_char(uint8_t(buffer[j]));
				}
	    		i += copy_len  - 1; //because i++ will fix it
	    	   
			}else{
//				std::cout << name[i];
                bit_string += '0'; // this bit signifies that the follow up is uncompressed char
                auto byte_as_string = bin::to_binary(uint8_t(buffer[i]));
                bit_string += byte_as_string;
				matcher.add_char(uint8_t(buffer[i]));
			}
		
	   }
	
    	sym_len = bin::symetrify(bit_string);

    	auto buffer_s_tpl =  bin::to_chars(bit_string, return_len);
    	if(buffer_s_tpl.is_error){
    		return buffer_s_tpl.err;
		}
		return std::move(buffer_s_tpl.value);
	}
	
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_compress(const std::string& buffer,  size_t& return_len, uint8_t& sym_len){
		return unoptimized_nonstream_lzss_compress(buffer.c_str(),buffer.size() , return_len, 	sym_len);
    }
    
    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_decompress(const char* buffer, const size_t& buffer_size, 
																	 	const uint8_t& sym_len, const size_t& decompressed_len, 
																		 const CompressionType& mode){
			    if(decompressed_len > SIZE_MAX){
			    	return HaregError("BUffer too large!", __FILE__, __LINE__);
				}
				auto rv_s_raw = new (std::nothrow)uint8_t[decompressed_len + sizeof(LzssBlockInfo)]{};
				if(rv_s_raw == nullptr){
					return HaregError("Error could not init memory", __FILE__, __LINE__);
				}
				//memset(rv_s_raw, '\0', decompressed_len); //might not be needed
				auto rv_s = std::unique_ptr<uint8_t[]>(rv_s_raw);
				size_t rv_s_len = 0;
				
				std::string input_bits = bin::str_to_bins(buffer, buffer_size);
				
				size_t ptr = 0;
				DEBUG_CALL(printf("Decompressed lenth is %d\n", decompressed_len));
				while(rv_s_len < decompressed_len){
					if(input_bits[ptr] == '0'){
						//uncompressed
						uint8_t ch = bin::get_char_from_bin(input_bits, ptr+1);
						//std::cout << (char)(ch);
						//std::cout<<"Got char: " <<ch << " from "<<'\n';
						rv_s[rv_s_len++]= ch;
						ptr += 9;
					}else{
						//compressed file
						//we need to go back and fetch the data
						size_t token_size;
						uint16_t back_ptr; 
						uint16_t copy_len;
						
//						size_t p;
//						auto token_bin = input_bits.substr(ptr+1, sizeof(LzssToken)*8);
//						auto token_char_tpl = bin::to_chars(token_bin, p);
//						
//						
//						auto token = reinterpret_cast<const LzssToken*>((token_char_tpl.value).get());
//						uint16_t back_ptr = token->back_ptr;
//						uint16_t copy_len = token->copy_len;
                        if(mode == LZSS_HUFFMAN_4KB){
                        	token_size =  sizeof(LzssToken4KB);
                        	size_t p;
							auto token_bin = input_bits.substr(ptr+1, token_size*8);
							auto token_char_tpl = bin::to_chars(token_bin, p);
							
							
							auto token = reinterpret_cast<const LzssToken4KB*>((token_char_tpl.value).get());
							back_ptr = token->back_ptr;
							copy_len = token->copy_len;
                        	
						}else if(mode == LZSS_HUFFMAN_8KB){
							token_size =  sizeof(LzssToken8KB);
                        	size_t p;
							auto token_bin = input_bits.substr(ptr+1, token_size*8);
							auto token_char_tpl = bin::to_chars(token_bin, p);
							
							
							auto token = reinterpret_cast<const LzssToken8KB*>((token_char_tpl.value).get());
							back_ptr = token->back_ptr;
							copy_len = token->copy_len;
							
						}else if(mode == LZSS_HUFFMAN_32KB){
							token_size =  sizeof(LzssToken32KB);
                        	size_t p;
							auto token_bin = input_bits.substr(ptr+1, token_size*8);
							auto token_char_tpl = bin::to_chars(token_bin, p);
							
							
							auto token = reinterpret_cast<const LzssToken32KB*>((token_char_tpl.value).get());
							back_ptr = token->back_ptr;
							copy_len = token->copy_len;
							
						}else if(mode == LZSS_HUFFMAN_64KB){
						    token_size =  sizeof(LzssToken64KB);
                        	size_t p;
							auto token_bin = input_bits.substr(ptr+1, token_size*8);
							auto token_char_tpl = bin::to_chars(token_bin, p);
							
							
							auto token = reinterpret_cast<const LzssToken64KB*>((token_char_tpl.value).get());
							back_ptr = token->back_ptr;
							copy_len = token->copy_len;
							
						}else {
							return HaregError("Error unkown dictionary format given!", __FILE__, __LINE__);
						}
                       

					//	std::cout << back_ptr <<','<<copy_len<<'\n';
						//system("pause");
						//now we copy from the already decompressed 
						
						uint32_t start = rv_s_len - back_ptr;
						for(uint32_t k = 0; k < copy_len; k++){
							rv_s[rv_s_len++]  = rv_s[start+k];
							//std::cout << (char)(rv_s[rv_s_len-1]);
						}
						ptr += token_size * 8 + 1;
					}
					
				}



				return std::move(rv_s);
																				
    }
    HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_lzss_decompress(const std::string& buffer, 	const uint8_t& sym_len,  const size_t& uncompressed_size){
    		return unoptimized_nonstream_lzss_decompress(buffer.c_str(),buffer.size() , 	sym_len,  uncompressed_size);
	}
	
}

