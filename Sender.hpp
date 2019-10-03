#pragma once

#include <vector>

typedef unsigned char byte_t;
typedef bool bit_t;

class Byte;

using namespace std;

class Sender {
    private:

    bit_t lastClockValue = false;

    void (*writeClockBit)(bit_t) = nullptr;
    void (*writeDataBit)(bit_t) = nullptr;

    vector<Byte*> byteMessage;

    void writeBit(const bit_t _bit);
    void writeByte(Byte* _byte);

    void beginMessageReadySequence();
    void writeMessageWillStartSignal();
    void writeMessageLengthPrefix();
    void writeMessageContents();

    Byte* zeroByte = nullptr;
    Byte* fullByte = nullptr;

    public:
    Sender();

    void loadMessage(const vector<Byte*> _bytes);
    
    void send();

    void registerClockWriteFunction(void (*_clockWriteFunction)(bit_t));
    void registerDataWriteFunction(void (*_clockDataFunction)(bit_t));

};