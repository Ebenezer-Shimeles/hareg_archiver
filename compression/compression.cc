#include <compression/compression.h>

#include <lzss/lzss.h>
#include <huffman/huffman.h>
#include <file/file.h>
namespace compression{
	
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_compress_buffer(const char* buffer, const size_t& buffer_size, 
	                                                              size_t& compressed_len,
																  uint8_t& lzss_sym_len,
	                                                              CompressionType mode ){
		//this functoin combines the lzss and huffmanc
		
		size_t lzss_compressed_len;
		DEBUG_CALL(printf("Buffer size is %i\n", buffer_size));
        DEBUG_CALL(printf("Compressing using LZSS Algorithm\n"));
		auto lzss_compressed_tuple = lzss::unoptimized_nonstream_lzss_compress(buffer, buffer_size, lzss_compressed_len, lzss_sym_len, mode);
		if(lzss_compressed_tuple.is_error){
			return lzss_compressed_tuple.err;
		}
		DEBUG_CALL(printf("LZSS compressed size is %i\n", lzss_compressed_len));
		DEBUG_CALL(printf("\nCompressing using Huffman coding\n"));
		//unoptimized_nonstream_huffman_compress_buffer(const char* buffer, const size_t& buffer_len, size_t& return_len)
		auto huffman_compressed_tuple = huffman::unoptimized_nonstream_huffman_compress_buffer((char*)lzss_compressed_tuple.value.get(), lzss_compressed_len, compressed_len);
        
        if(huffman_compressed_tuple.is_error){
        	return huffman_compressed_tuple.err;
		}
		DEBUG_CALL(printf("\nHuffman compressed size is %i\n", compressed_len));
		return std::move(huffman_compressed_tuple.value);
      
       
		
	}
	HaregVal<std::unique_ptr<uint8_t[]>> unoptimized_nonstream_compress(const std::string& buffer, size_t& compressed_len,
																  uint8_t& lzss_sym_len,
	                                                              CompressionType mode){
		return unoptimized_nonstream_compress_buffer(buffer.c_str(), buffer.size(), compressed_len, lzss_sym_len, mode);
	}
	
	HaregVal<void> unoptimized_nonstream_compress_file(const char *input_file_name, const size_t& input_file_name_size, const CompressionType& mode,
	                                              char *output_file_name,  size_t output_file_name_size){
        DEBUG_CALL(printf("Starting Compression\n"));
		if(input_file_name_size > FILENAME_MAX || input_file_name_size <= 0){
        	return HaregError("Invalid input file name given!", __FILE__, __LINE__);
		}
		if(output_file_name_size > FILENAME_MAX || output_file_name_size < 0){
        	return HaregError("Invalid output file name given!", __FILE__, __LINE__);
		}
		
	    char  output_n[FILENAME_MAX];
		if(output_file_name_size == 0){
			
			memset(output_n, '\0', FILENAME_MAX);
			output_file_name = output_n;
			strncpy(output_file_name, input_file_name, input_file_name_size);
			strcat(output_file_name, ".hrz"); //hareg zip
			output_file_name_size = input_file_name_size + 4;
		}
		

        size_t buffer_size;
        DEBUG_CALL(printf("Reading file  buffer\n"));
        auto file_buffer = file::get_buffer_of(input_file_name, buffer_size);
        
        if(file_buffer.is_error){
        	return file_buffer.err;
		}
		
		size_t compressed_len;
		uint8_t lzss_sym_len;
		
        auto compressed_buffer = unoptimized_nonstream_compress_buffer(file_buffer.value.get(), buffer_size, compressed_len, lzss_sym_len, mode);
        
		auto file_header = file::FileInfoBlock();
	    //	struct FileInfoBlock{
//		uint8_t lzzs_sym; //no of added bits to lzss bit stream to make it modulo 8
//		uint8_t file_name[FILENAME_MAX]{};
//		size_t compressed_length;
//		size_t decompresssed_length;
//		
//		
//	};
        memset(&file_header, '\0',  sizeof(file::FileInfoBlock));
        file_header.lzzs_sym = lzss_sym_len;
        file_header.compressed_length = compressed_len;
        file_header.decompresssed_length = buffer_size;
        file_header.comp_type = mode;
		file_header.file_name_size = input_file_name_size;
        //strncpy((char*)file_header.file_name, input_file_name, input_file_name_size);
        
        
        DEBUG_CALL(printf("Output file name is %s\n", output_file_name));
        auto save_header_tpl = file::save_buffer_on_file(output_file_name, (char*)&file_header, sizeof(file::FileInfoBlock));
        if(save_header_tpl.is_error)
        {
        	return save_header_tpl.err;
		}
		auto save_file_name_tpl = file::append_buffer_on_file(output_file_name, input_file_name, input_file_name_size);
		if (save_file_name_tpl.is_error)
		{
			return save_file_name_tpl.err;
		}
		auto append_content_tpl = file::append_buffer_on_file(output_file_name, (char*)compressed_buffer.value.get(), compressed_len);  
		if(append_content_tpl.is_error)
        {
        	return append_content_tpl.err;
		}
        float compression_ratio = buffer_size/(float)compressed_len;
        DEBUG_CALL(printf("Compression Ratio is %f\n", compression_ratio));
		 
		return {};                             
	    
	    
	}
	HaregVal<void> unoptimized_nonstream_decompress_file(const char* file_name, const size_t& file_name_len){
		
		if(file_name_len > FILENAME_MAX || file_name_len <= 0){
        	return HaregError("Invalid input file name given!", __FILE__, __LINE__);
		}
		
	    size_t compressed_buffer_size;
		auto buffer_tpl = file::get_buffer_of(file_name, compressed_buffer_size);
		if(buffer_tpl.is_error){
			return buffer_tpl.err;
		}
		
		if(compressed_buffer_size <= sizeof(file::FileInfoBlock)){
			return HaregError("File is corrupted", __FILE__, __LINE__);
		}
		
		file::FileInfoBlock file_info;
		memcpy(&file_info, buffer_tpl.value.get(), sizeof(file::FileInfoBlock));
		DEBUG_CALL(printf("FileInfoBlock size is %l, compressed_length is %i, compressed_buffer_size is %i\n",
			sizeof(file::FileInfoBlock), file_info.compressed_length, compressed_buffer_size)
			);
		if(file_info.compressed_length != compressed_buffer_size -   sizeof(file::FileInfoBlock) - file_info.file_name_size){
			return HaregError("Error archive is corrupted", __FILE__, __LINE__);	
		}
		DEBUG_CALL(printf("Metadata is read\n"));
		DEBUG_CALL(printf("Compressed File name size is %d\n", file_info.file_name_size));
		char* decompressed_file_name = new char[file_info.file_name_size+1];
		memset((void*)decompressed_file_name, '\0', file_info.file_name_size+1);
		memcpy((void*)decompressed_file_name, buffer_tpl.value.get() + sizeof(file::FileInfoBlock), file_info.file_name_size);
		DEBUG_CALL(printf("Compressed File name is %s\n", decompressed_file_name));
		DEBUG_CALL(printf("The compression enum type %d\n", file_info.comp_type));
		size_t huffman_decompressed_size;
		auto huffman_decompressed_tpl = huffman::unoptimized_nonstream_huffman_decompress_buffer(buffer_tpl.value.get() + file_info.file_name_size +  sizeof(file::FileInfoBlock), file_info.compressed_length, huffman_decompressed_size);
	    if(huffman_decompressed_tpl.is_error){
	    	return huffman_decompressed_tpl.err;
		}
		DEBUG_CALL(printf("LZSS Decompressiong is running!\n"));
		size_t total_decompressed_size;
		/*
		   unoptimized_nonstream_lzss_decompress(const char* buffer, const size_t& buffer_size, 
																	 	const uint8_t& sym_len, const size_t& decompressed_len)
		
		*/
		auto lzss_decompressed_tpl = lzss::unoptimized_nonstream_lzss_decompress((char *)huffman_decompressed_tpl.value.get(), huffman_decompressed_size, 
																	 file_info.lzzs_sym, file_info.decompresssed_length, (CompressionType)file_info.comp_type);
	    if(lzss_decompressed_tpl.is_error){
	    	return lzss_decompressed_tpl.err;
		}
	
		
		file::save_buffer_on_file(decompressed_file_name, (char*)(lzss_decompressed_tpl.value.get()), file_info.decompresssed_length);
		delete[] decompressed_file_name;
		return {};
	}
}
