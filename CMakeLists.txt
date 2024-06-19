cmake_minimum_required(VERSION 3.10)


project(hareg)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR})
add_executable(hareg main.cc bin/bin.cc common/hareg_error.cc 
                    compression/compression.cc file/file.cc huffman/huffman.cc huffman/huffman_tree_node.cc lzss/linked_list_matcher.cc lzss/lzss.cc 
                    
                    )
