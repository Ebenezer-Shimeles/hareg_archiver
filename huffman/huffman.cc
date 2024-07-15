#include <huffman/huffman.h>

#include <queue>
#include <cstddef>
#include <cstring>
#include <memory>
namespace huffman{

	
	
	/*
	    Traverses the huffman tree and stops when it reaches the leaf
	    leaves are always chars
	    stores new binary codes in map which is shared
	*/
    HaregVal<void> get_char_codes_from_tree( HuffmanNode* root, std::string curr_code,  std::string* map){

        
		if(root != nullptr){
			//std::cout << root->char_val << " at location " << (intptr_t)(root) << '\n';
			if(root->is_leaf()){
				//std::cout << "Reached leaf\n";
			//	std::cout << root->char_val << ' ' << curr_code << '\n';
				const uint8_t cval = root->char_val;
				map[cval]  = curr_code;
			}else{
				
				if(root->left != nullptr){
					get_char_codes_from_tree(root->left, curr_code + "0", map);
				}
				if(root->right != nullptr){
					get_char_codes_from_tree(root->right, curr_code + "1", map);
				}
			}
		}else{
			//std::cout << "root is null";
		}
		return {};
		
	}

	
	/*
	    builds huffman tree using std::priority_queue
	    
	    lead nodes are always char codes
	    we assign smallest codes to frequent char and bigger codes to non-frequent ones
	*/
	HaregVal<HuffmanNode> get_huffman_tree(const int32_t* freqs){
		DEBUG_CALL(printf("Building Huffman Tree\n"));

		std::priority_queue<HuffmanNode, std::vector<HuffmanNode>, std::greater<HuffmanNode>> pq;
		try{
		    for(int32_t i = 0;i < 256;i++){
		
				const uint8_t ch = static_cast<uint8_t>(i);
				auto freq = freqs[ch];
				if(freq == 0){
					continue;
				}
				
			    HuffmanNode node  =  HuffmanNode(ch, freq);
			    
			    
			    pq.push(node);
	
			    
			   
			}
			
			while(pq.size() > 0 ){
				// We pop the lowest occuring char nodes and make a new parent for them
				// We then push the parent
				// This continues until one  root node is achieved
				// so 256 -> 128 -> 64 -> 32 -> 16 -> 8 -> 4 -> 2 -> 1
				
				HuffmanNode l = pq.top(); pq.pop();
				auto left = new HuffmanNode(l);
				
	
				
				HuffmanNode new_root =  HuffmanNode('Z', left->freq_val);
				new_root.left = left;
				
				
				if(pq.size() > 0){
					HuffmanNode r = pq.top(); pq.pop();
					auto right = new HuffmanNode(r);
			
				    new_root.right = right;
				    new_root.freq_val += right->freq_val;
				}
			
				pq.push(new_root);
		
				if(pq.size() == 1){
				    auto root = std::move(pq.top());
				    pq.pop();
				    return std::move(root);
				    
				   
				}
				
			}
	   }
			
      	catch(std::exception e){
			return HaregError(std::string("unable to build huffman tree: ") + e.what(),  __FILE__, __LINE__);
		}
		
 	}
	
	/*
	    Gives us new char codes when we give it frequencies
	*/
   [[nodiscard]]
	HaregVal<std::unique_ptr<std::string[]>> get_char_codes_from_freqs(const int32_t * freqs){
		   auto tree_tuple = get_huffman_tree(freqs);

		   if(tree_tuple.is_error){
		   	    return tree_tuple.err;
		   }
		   auto tree = tree_tuple.value;
		   auto rv = std::unique_ptr<std::string[]> (new std::string[256]);
		   DEBUG_CALL(printf("Building huffman codes from tree\n"));
		   get_char_codes_from_tree(&tree, "", rv.get());
//		   for(int i=0;i<256;i++){
//		   	  auto freq = (rv.get())[i];
//		   	  if(freq != ""){
//		   	     std::cout <<char(i) << " : " << freq << '\n';	
//			  }
//		   }
		   tree.deallocate();
		   return std::move(rv);
	}
	

	
	

    /*
	   Allocates array of 256 * sizeof(int32_t)
	   counts frequency of every value in extended ascii and uses the array as a hashmap
	   returns the array
	*/
	[[nodiscard]]
	std::unique_ptr<int32_t[]> count_freqs(const char* buffer, const size_t& size){
		auto freqs = std::unique_ptr<int32_t[]>(new int32_t[256]);
	  	memset(freqs.get(), 0x00, 256 * sizeof(int32_t));
		for(size_t i=0;i<size;i++){
			const uint8_t ch= buffer[i];
			freqs[ch]++;
		}
		return std::move(freqs);
	}
	
	/*
	    Gets a single character from a huffman tree by traversing to the leaf
	    0 => Go to the left of the tree
	    1 => Go to the right of the tree
	    also update offset
	*/
	HaregVal<uint8_t> get_next_char_from_huffman_tree(HuffmanNode* root, const std::string& codes, size_t&  offset){
		if(root == nullptr){
			return HaregError("Error end of tree", __FILE__, __LINE__);
		}
		if(root->is_leaf()){
			return root->char_val;
		}
		if(offset >= codes.size()){
			return HaregError("Error offset length is overflow", __FILE__, __LINE__);
		}
		if(codes[offset] == '0'){
			offset++;
			return get_next_char_from_huffman_tree(root->left,codes,  offset );
		}else if(codes[offset] == '1'){
			offset++;
			return get_next_char_from_huffman_tree(root->right, codes, offset);
		}else{
			return HaregError("Error unkown character given!", __FILE__, __LINE__);
		}
	}
	
	/*
	    This is the first version of implemented huffman code.
	    It is not optimized and also doesn't compress files when they are larger than the free ram size
	*/
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decode(const char* str, const size_t& len,
	                                                                       const int32_t *freqs,
	                                                                       const uint8_t& sym_len, size_t& return_len ){
        auto tree_tuple = get_huffman_tree(freqs);
        auto tree = tree_tuple.value;
        
        auto bin_str = bin::str_to_bins(str, len);
        size_t offset = 0;
        std::string rv_s = "";
        while(offset < bin_str.size() - sym_len){
        	const uint8_t ch = (get_next_char_from_huffman_tree(&tree, bin_str, offset).value);
        	rv_s += ch;
		}
        tree.deallocate();
        return_len = rv_s.size();
        auto rv = std::unique_ptr<uint8_t[]>(new uint8_t[rv_s.size()]);
        memcpy(rv.get(), rv_s.c_str(), rv_s.size());
        return std::move(rv);
		
	}
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decode(const std::string& str, const int32_t* freqs, const uint8_t& sym_len, size_t& return_len ){
	    return 	unoptimized_nonstream_huffman_decode(str.c_str(), str.size(),freqs, sym_len,return_len);		
	}
	
	/*
	    This is the first version of implemented huffman code.
	    It is not optimized and also doesn't compress files when they are larger than the ram size
	*/
	HaregVal<void> unoptimized_nonstream_huffman_encode(const char* str, const size_t& size, std::string& rv, uint8_t& sym_len){
		auto freqs = std::move(count_freqs(str, size));
		DEBUG_CALL(printf("Char Frequencies calculated\n"));
	    auto codes_tuple = get_char_codes_from_freqs(
			     freqs.get()
		);
		if(codes_tuple.is_error){
			return codes_tuple.err;
		}
		
		auto codes = std::move(
             codes_tuple.value
		);
		
		std::string huffman_bins = "";
		for(size_t i=0; i < size ; i++){
			uint8_t ch = str[i];
			huffman_bins += codes[ch];
		}
	//	std::cout << "huffman bins calculated:" << huffman_bins <<'\n';
		sym_len = bin::symetrify(huffman_bins);
		size_t char_len = 0;
		auto ch_tuple = bin::to_chars(huffman_bins, char_len);
		if(ch_tuple.is_error){
			return ch_tuple.err;
		}
	
		auto chars = std::move(ch_tuple.value);
		for(int64_t i=0; i < char_len; i++){
			rv += (chars.get())[i];
		}
       	DEBUG_CALL(printf("Chars encoded!\n"));
		return {};
		
		
	}
	HaregVal<void> unoptimized_nonstream_huffman_encode(const std::string& str, std::string& rv, uint8_t& sym_len){
		return unoptimized_nonstream_huffman_encode(str.c_str(), str.size(), rv, sym_len);
	}
	

	
	[[nodiscard]]
	std::unique_ptr<int32_t[]> count_freqs(const std::string& str){
		return std::move(count_freqs(str.c_str(), str.size()));
	}
	
	/*
	   This just compresses the file but also adds some header infos that are needed to decompress later
	*/
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_compress_buffer(const char* buffer, const size_t& buffer_len, size_t& return_len){
		 std::string rv_s = "";
		 uint8_t sym_len;
		 
		 auto freqs = std::move(count_freqs(buffer, buffer_len));
		 
		 auto huff_tuple = huffman::unoptimized_nonstream_huffman_encode(buffer, buffer_len, rv_s, sym_len);
		 if(huff_tuple.is_error){
		 	return huff_tuple.err;
		 }
		 
		 auto header =  HuffmanBlockInfoNode();
		 auto header_size = sizeof(HuffmanBlockInfoNode);
		 memset(&header, '\0', header_size);
		 
		 header.sym_len= sym_len;
		 for(int32_t i=0;i<256;i++){
		 	header.freqs[i] = freqs[i];
		 }
		 
		 return_len = rv_s.size() + header_size;
		 
		 auto rv = std::unique_ptr<uint8_t[]>(new uint8_t[return_len]);
		 DEBUG_CALL(printf("Huffman header size is %i\n", header_size));
		 DEBUG_CALL(printf("Huffman Compressed buffer size is %i\n", rv_s.size()));
		 memcpy(rv.get(), &header, header_size);
		 memcpy(rv.get() + header_size, rv_s.c_str(), rv_s.size());
		 
		 
		 
		 return std::move(rv);
	}
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_compress_buffer(const std::string& buffer, size_t& return_len){
		return  unoptimized_nonstream_huffman_compress_buffer(buffer.c_str(), buffer.size(),return_len);
	}
	
	
	/*
	
	*/
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decompress_buffer(const char* buffer, const size_t& buffer_len, size_t& return_len){
		 auto header_size = sizeof(HuffmanBlockInfoNode);
		 auto header = (reinterpret_cast<const HuffmanBlockInfoNode*>(buffer));
		 
         /*
		    (const char* str, const size_t& len,
	                                                                       const int32_t *freqs,
	                                                                       const uint8_t& sym_len, uint8_t& return_len )
		 */
		 auto decoded_tuple = unoptimized_nonstream_huffman_decode(buffer + header_size, size_t(buffer_len - header_size),
	                                                                       &(header->freqs[0]),
	                                                                       header->sym_len, return_len );
	     if(decoded_tuple.is_error){
	     	return decoded_tuple.err;
		 }
		 return std::move(decoded_tuple.value);
	            
		 
	}
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_huffman_decompress_buffer(const std::string& buffer, size_t& return_len){
		return  unoptimized_nonstream_huffman_decompress_buffer(buffer.c_str(), buffer.size(),return_len);
	}
	
	
}
