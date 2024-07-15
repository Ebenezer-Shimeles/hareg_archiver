#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>

struct HuffmanNode {

    uint8_t char_val;
    int32_t freq_val;

    HuffmanNode();
    HuffmanNode(uint8_t cval, int32_t fval);
    HuffmanNode(const HuffmanNode& other);

    HuffmanNode *left;
    HuffmanNode *right;

    bool is_leaf() const;
    void deallocate();
    /**
     * The following functions are used to debugging only
    */
    /*
   These operators are needed for priority queue
*/
    int operator >(const HuffmanNode& other)  const noexcept  {
        return freq_val > other.freq_val;
    }
    int operator <(const HuffmanNode& other)  const noexcept {
        return freq_val < other.freq_val;
    }

    int operator ==(const HuffmanNode& other) const noexcept{
        return freq_val == other.freq_val;
    }

};


