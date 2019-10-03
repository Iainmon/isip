#pragma once

typedef unsigned char byte_t;

class Byte {
    private:

    bool* bits = nullptr;

    public:
    Byte();
    Byte(bool* bits);

    void setBits(bool* bits);

    const bool getBit(const byte_t _index);

    const byte_t getValue();

};