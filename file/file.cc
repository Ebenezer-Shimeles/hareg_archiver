#include <common/hareg_error.h>
#include <common/hareg_val.h>
#include <common/common.h>
#include <cstdio>
#include <cstddef>
#include <memory>
namespace file{
	
	 /*
		   First save file pointer it to a temp ptr
		   then go to the end of the file
		   get the cursor position
		   return to previous address
	*/
	[[nodiscard]]
	size_t get_file_size(FILE *fp){

		size_t init_pos = ftell(fp);
		fseek(fp, 0, SEEK_END);
		
		size_t rv = ftell(fp);
		fseek(fp, init_pos, SEEK_SET);
		
		return rv;
	}
	
	/*
	    get file size first, then allocate memory on heap equal to file size,
	    read file buffer to heap and move ownership of that memory
	*/
	[[nodiscard]]
	HaregVal<std::unique_ptr<char[]>>  get_buffer_of(const char* file_name, size_t& buffer_size){
		FILE *file = fopen(file_name, "rb");
		if(!file){
			return { HaregError("file does not exist", __FILE__, __LINE__)};
		}
		buffer_size = get_file_size(file);
		
		auto buffer =  std::unique_ptr<char[]>(new char[buffer_size]);
		fread(buffer.get(), 1, buffer_size, file);
		fclose(file);
		return std::move(buffer);
		
	}
	
	/*
	   Saves the given buffer with the given size to file_name
	   Make sure to deallocate hareg error!
	*/
	[[nodiscard]]
	HaregVal<void> save_buffer_on_file(const char* file_name,const char* buffer, const size_t& buffer_size ){
		FILE *fp = fopen(file_name, "wb");
		if(!fp){
			return  HaregError("unable to open file", __FILE__, __LINE__);
		}
	    DEBUG_CALL(printf("Saving Buffer of size %d\n", buffer_size));
		fwrite(buffer, 1, buffer_size, fp);
		fclose(fp);
        return {};
	 
	}
	
	

	HaregVal<void> append_buffer_on_file(const char* file_name,const char* buffer, const size_t& buffer_size ){
		FILE *fp = fopen(file_name, "ab+");
		if(!fp){
			return  HaregError("unable to open file", __FILE__, __LINE__);
		}
	    DEBUG_CALL(printf("Appending Buffer of size %d\n", buffer_size));
		fwrite(buffer, 1, buffer_size, fp);
		fclose(fp);
        return {};
	 
	}
	
}
