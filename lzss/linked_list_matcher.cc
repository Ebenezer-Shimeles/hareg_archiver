#include <lzss/linked_list_matcher.h>

#include <cstring>

lzss::LinkedListMatcher::LinkedListMatcher(LinkedListMatcher&& other){ //movable!
	this->m_curr_size = other.m_curr_size;
	this->m_ll_head = other.m_ll_head;
	this->m_ll_tail = other.m_ll_tail;
	this->m_mode = other.m_mode;
	for(int8_t i=0;i<256;i++){
		this->m_occurs[i] = other.m_occurs[i];
	}
				
	//Make the other object hollow
	other.m_curr_size = 0;
	other.m_ll_head = nullptr;
	other.m_ll_tail = nullptr;

}
lzss::LinkedListMatcher::LinkedListMatcher(CompressionType mode ): m_curr_size(0), m_mode(mode){}
lzss::LinkedListMatcher::~LinkedListMatcher(){
	auto p = m_ll_head;
	while(p != nullptr){
		auto next = p->next;
		delete p;
		p = next;
	}
}
			
void lzss::LinkedListMatcher::add_char(uint8_t ch, CompressionType mode){
				
			
	auto WINDOW_SIZE = WINDOW_SIZE_4KB;
				
				
	if(m_mode == LZSS_HUFFMAN_8KB){
		WINDOW_SIZE = WINDOW_SIZE_8KB;
				
					
	}else if(m_mode == LZSS_HUFFMAN_32KB){
		WINDOW_SIZE = WINDOW_SIZE_32KB;
				
	}
	else if(m_mode == LZSS_HUFFMAN_64KB){
		WINDOW_SIZE = WINDOW_SIZE_64KB;
				
	}
				
	if(m_ll_head == nullptr){
		m_ll_head = new LinkedListNode();
		memset(m_ll_head, '\0', sizeof(LinkedListNode));
		m_ll_head->char_val = ch;
		m_ll_tail = m_ll_head;
	}else{
		auto new_tail = new LinkedListNode();
		new_tail->char_val = ch;
		m_ll_tail->next = new_tail;
		m_ll_tail = new_tail;
	}
	m_curr_size++;
	if(m_curr_size == WINDOW_SIZE){
	//	std::cout << "FULL\n";
		auto ch = m_ll_head->char_val;
		m_occurs[ch].erase(m_ll_head);
		auto next = m_ll_head->next;
		delete m_ll_head;
		m_ll_head = next;
		m_curr_size--;
	}
	m_occurs[ch].insert(m_ll_tail);
}
			
size_t lzss::LinkedListMatcher::find_match_len(const LinkedListNode* root, const char* buffer,const size_t& strlen,   size_t offset ){
	size_t match_len = 0;
				
	auto MAX_COPY_LEN = MAX_COPY_LEN_4KB;
	auto WINDOW_SIZE = WINDOW_SIZE_4KB;
				
				
	if(m_mode == LZSS_HUFFMAN_8KB){
		WINDOW_SIZE = WINDOW_SIZE_8KB;
		MAX_COPY_LEN = MAX_COPY_LEN_8KB;
					
	}else if(m_mode == LZSS_HUFFMAN_32KB){
		WINDOW_SIZE = WINDOW_SIZE_32KB;
		MAX_COPY_LEN = MAX_COPY_LEN_32KB;
	}
	else if(m_mode == LZSS_HUFFMAN_64KB){
		WINDOW_SIZE = WINDOW_SIZE_64KB;
		MAX_COPY_LEN = MAX_COPY_LEN_64KB;
	}
				
	auto p = root;
	while(p != nullptr && offset < strlen && match_len < MAX_COPY_LEN ){
		if(p->char_val != uint8_t(buffer[offset++]) ){
			break;
		}
		match_len++;
		p = p->next;
	}
	return match_len;
}
			
			
			
			
// template<class T>
// LzStringMatch<T> lzss::LinkedListMatcher::find_largest_match(const char* buffer, const size_t& strlen, const size_t& start_offset){
// 	static_assert(
// 		std::is_same<T, LzssToken4KB>::value ||
// 		std::is_same<T, LzssToken8KB>::value ||
// 		std::is_same<T, LzssToken32KB>::value ||
// 		std::is_same<T, LzssToken64KB>::value 
// 		, 
				
// 	"Invalid Lzss Token struct given!"); //template safety so we can't use any new thing that is not a valid lzss struct
				
// 	auto ch = uint8_t(buffer[start_offset]);
// 	auto possibles = m_occurs[ch];
				
// 	int64_t max_match = 0; //just in case!!!!
// 	LinkedListNode* max_match_node = nullptr;
// 	for(LinkedListNode* pos : possibles){
// 		auto match = find_match_len(pos, buffer,strlen, start_offset);
// 		if(match > max_match){
// 			max_match = match;
// 			max_match_node = pos;
// 		}
// 	}
				
// 	auto token_size = sizeof(LzssToken4KB);
// 	if(m_mode == LZSS_HUFFMAN_8KB){
// 		token_size = sizeof(LzssToken8KB);
					
// 	}else if(m_mode == LZSS_HUFFMAN_32KB){
// 			token_size = sizeof(LzssToken32KB);
// 	}
// 	else if(m_mode == LZSS_HUFFMAN_64KB){
// 			token_size = sizeof(LzssToken64KB);
// 	}
				
// 	if(max_match < token_size){
// 		return {};
// 	}
// 			//	std::cout << "max match is "<< max_match <<'\n';
// 	return { (uint16_t)(m_ll_tail->id -  max_match_node->id + 1), (uint16_t)max_match};
				
// }
			

		
