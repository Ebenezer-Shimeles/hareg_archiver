#pragma once

#include <lzss/linked_list_node.h>
#include <compression/compression_type.h>
#include <lzss/lz_string_match.h>
#include <lzss/dictionary.h>

#include <unordered_set>
#include <ostream>
#include <string>
namespace lzss{
        class LinkedListMatcher {
            private:
                size_t m_curr_size;
                LinkedListNode* m_ll_head;
                LinkedListNode* m_ll_tail;
                CompressionType m_mode;
                std::unordered_set<LinkedListNode*> m_occurs[256];

            public:
                LinkedListMatcher(const LinkedListMatcher&) = delete; // not copyable
                LinkedListMatcher(LinkedListMatcher&& other); // movable

                LinkedListMatcher(CompressionType mode = LZSS_HUFFMAN_4KB);
                ~LinkedListMatcher();

                void add_char(uint8_t ch, CompressionType mode = LZSS_HUFFMAN_4KB);

                size_t find_match_len(const LinkedListNode* root, const char* buffer, const size_t& strlen, size_t offset);

                template<class T>
                LzStringMatch<T> find_largest_match(const char* buffer, const size_t& strlen, const size_t& start_offset){
                    {
                    static_assert(
                        std::is_same<T, LzssToken4KB>::value ||
                        std::is_same<T, LzssToken8KB>::value ||
                        std::is_same<T, LzssToken32KB>::value ||
                        std::is_same<T, LzssToken64KB>::value 
                        , 
                                
                    "Invalid Lzss Token struct given!"); //template safety so we can't use any new thing that is not a valid lzss struct
                                
                    auto ch = uint8_t(buffer[start_offset]);
                    auto possibles = m_occurs[ch];
                                
                    int64_t max_match = 0; //just in case!!!!
                    LinkedListNode* max_match_node = nullptr;
                    for(LinkedListNode* pos : possibles){
                        auto match = find_match_len(pos, buffer,strlen, start_offset);
                        if(match > max_match){
                            max_match = match;
                            max_match_node = pos;
                        }
                    }
                                
                    auto token_size = sizeof(LzssToken4KB);
                    if(m_mode == LZSS_HUFFMAN_8KB){
                        token_size = sizeof(LzssToken8KB);
                                    
                    }else if(m_mode == LZSS_HUFFMAN_32KB){
                            token_size = sizeof(LzssToken32KB);
                    }
                    else if(m_mode == LZSS_HUFFMAN_64KB){
                            token_size = sizeof(LzssToken64KB);
                    }
                                
                    if(max_match < token_size){
                        return {};
                    }
                            //	std::cout << "max match is "<< max_match <<'\n';
                    return { (uint16_t)(m_ll_tail->id -  max_match_node->id + 1), (uint16_t)max_match};
                                
                }
			

		

                }

                int operator <<(std::ostream& o) {
             
                        o << "Size is: " << m_curr_size << '[';
                        auto r = m_ll_head;
                        while (r != nullptr) {
                            o << r->char_val;
                            if (r->next != nullptr) {
                                o << ", ";
                            }
                            r = r->next;
                        }
                        o << "]\n";
                        return 0;
                    

                }
        };

      
}
