#include <iostream>

using namespace std;
typedef unsigned char byte_t;
class Byte {
    private:

    bool* bits = nullptr;

    public:
    Byte();

    void setBits(bool* bits);

    const byte_t getValue();

};

Byte::Byte() {
    bits = new bool[8];
}

void Byte::setBits(bool* _bits) {
    for (int i = 0; i < 8; i++) {
        bits[i] = _bits[i];
    }
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

int main() {
    Byte* byte = new Byte();
    bool* bits = nullptr;
    bits = new bool[8];
    for (int i = 0; i < 8; i++) {
        bits[i] = false;
    }
    bits[7] = true;
    byte->setBits(bits);
    cout << ((int)byte->getValue()) << endl;
    cout << "Works..." << endl;
}