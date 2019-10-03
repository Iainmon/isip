#include "Byte.hpp"

Byte::Byte() {
    bits = new bool[8];
}

Byte::Byte(bool* _bits) {
    
    bits = new bool[8];

    this->setBits(_bits);
}

void Byte::setBits(bool* _bits) {
    for (int i = 0; i < 8; i++) {
        bits[i] = _bits[i];
    }
}

const bool Byte::getBit(const byte_t _index) {
    return bits[_index];
}

const byte_t Byte::getValue() {
    byte_t value = 0;
    int j = 7;
    for (int i = 0; i < 8; i++, j--) {
        if (bits[j]) {
            value |= 1 << i;
        }
    }
    return value;
}