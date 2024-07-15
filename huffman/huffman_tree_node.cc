#include <cstdint>
#include <cstddef>
#include <ostream>

// struct HuffmanNode{
	
#include <huffman/huffman.h>	
		
HuffmanNode::HuffmanNode(void){}
		
HuffmanNode::HuffmanNode(uint8_t cval, int32_t fval): char_val(cval), freq_val(fval), left(nullptr), right(nullptr){}


/*
	Copy constructor
	not that expensive and also used in the heap
*/
HuffmanNode::HuffmanNode(const HuffmanNode& other){
		
		
			this->char_val = other.char_val;
			this->freq_val = other.freq_val;
			this->left = other.left;
			this->right = other.right;
}
		
	
bool HuffmanNode::is_leaf() const {
	return left == nullptr && right == nullptr;
}

/*
	We use postorder traversal so if the root is deleted all the nodes in the tree are also deleted
*/
void HuffmanNode::deallocate(){
	if(left != nullptr){
	    left->deallocate();
	    delete left;
	}
	if(right != nullptr){
	    right->deallocate();
	    delete right;
	}
}
	
 
int operator <<(std::ostream& o, HuffmanNode *node){
	if(node !=nullptr){
		o <<"Huffman Node: "<<  node->char_val << " with frequency " << node->freq_val << '\n';
		o << node->left;
		o << "\n\n\n\n\n";
		o << node ->right;
		o << "\n\n\n\n\n";			
	}
	return 0;

}
int operator <<(std::ostream& o, HuffmanNode node){
	o << "Huffman Node : " <<  node.char_val << " with frequency " << node.freq_val << '\n';
	o << node.left;
	o << "\n\n\n\n\n";
	o << node.right;
	o << "\n\n\n\n\n";
	return 0;
}