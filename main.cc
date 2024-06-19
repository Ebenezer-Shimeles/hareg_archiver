#include <cstring>
#include <compression/compression.h>
#include <iostream>


void show_help(const char* pr_name){
	printf(
	    "usage: %s <compress|decompress> file\n" , pr_name
	);
}

int main(int argc, char** argv){ 
   if(argc != 3){
   	   show_help(argv[0]);
   	   return -1;
   }
   const char* file_name = argv[2];
   const char* cmd = argv[1];
   
   if(strncmp(cmd, "compress", 8) == 0){
       auto compression_tuple = compression::unoptimized_nonstream_compress_file(file_name, strlen(file_name));
       if(compression_tuple.is_error)
       {
       	  std::cerr << compression_tuple.err << '\n';
       	  return -1;
	   }
   }
   else if(strncmp(cmd, "decompress", 10) == 0){
   	    auto decompression_tuple = compression::unoptimized_nonstream_decompress_file(file_name, strlen(file_name));
   	    if(decompression_tuple.is_error)
       {
       	  std::cerr << decompression_tuple.err << '\n';
       	  return -1;
	   }
   }else{
   	  show_help(argv[0]);
   	  return -1;
   }
   return 0;
}

