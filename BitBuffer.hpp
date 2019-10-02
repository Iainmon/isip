#pragma once

typedef unsigned char byte_t;

class Byte;

class BitBuffer {
    private:

    bool currentClockSignal = false;
    bool lastClockSignal = false;
    const bool clockSignalDidChange();

    byte_t currentBitAddress = 0;
    bool* bits = nullptr;
    const bool allBitsAND();

    byte_t byteCount = 0;
    byte_t currentByteAddress = 0;
    Byte** byteStorage = nullptr;

    void append();

    void record();

    public:
    BitBuffer();

    const bool listenForOpenConnection(const unsigned int scaledClockTime = 16000);

    void recordIncomingData();

    Byte** getBytes();

};