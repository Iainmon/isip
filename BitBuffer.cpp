#include "BitBuffer.hpp"
#include "Byte.hpp"

typedef char int1_t;
typedef unsigned char uint1_t;

const bool BitBuffer::allBitsAND() {
    return bits[0] && bits[1] && bits[2] && bits[3] && bits[4] && bits[5] && bits[6] && bits[7];
}

const bool BitBuffer::clockSignalDidChange() {
    currentClockSignal = readClockBit();

    if (currentClockSignal == !lastClockSignal) {
        lastClockSignal = currentClockSignal;
        return true;
    }
    return false;
}

BitBuffer::BitBuffer() {
    bits = new bool[8];
    for (int i = 0; i < 8; i++) {
        bits[i] = false;
    }

    *byteStorage = new Byte[255];
    for (int i = 0; i < 255; i++) {
        bits[i] = new Byte(bits);
    }

}

void BitBuffer::registerClockReadFunction(bool (*_clockReadFunction)(void)) {
    readClockBit = _clockReadFunction;
}
void BitBuffer::registerDataReadFunction(bool (*_dataReadFunction)(void)) {
    readDataBit = _dataReadFunction;
}

const bool BitBuffer::listenForOpenConnection(const unsigned int scaledClockTime = 16000) {

    currentBitAddress = 0;
    bits[currentBitAddress] = readDataBit();

    if (bits[currentBitAddress] == true) {

        lastClockSignal = readClockBit();
        currentBitAddress = 1;

        const unsigned int maxIterations = scaledClockTime / 16; // However many operations each iteration takes
        for (unsigned int i = 0; i < maxIterations; i++) {
            if (clockSignalDidChange()) {
                bits[currentBitAddress] = readDataBit();
                currentBitAddress++;
                if (currentBitAddress == 8) {
                    if (allBitsAND()) {
                        return true;
                    }
                    currentBitAddress = 0;
                }
            }
        }
    }

    return false;
}

void BitBuffer::recordIncomingData() {
    currentBitAddress = 0;
    currentByteAddress = 0;

    uint1_t blankBytePrefixSumWriteCount = 0;
    int1_t blankBytePrefixSum = 0;

    bool zeroBitStarted = false;
    bool bitValue;

    while (true) {
        if (clockSignalDidChange()) {
            
            bitValue = readDataBit();

            // Makes sure no comparisons are made until a 0 bit is sent
            if (!zeroBitStarted) {
                if (bitValue) {
                    break;
                }
                zeroBitStarted = true;
            }

            if (!bitValue) {
                blankBytePrefixSum--;
            } else {
                blankBytePrefixSum++;
            }
            blankBytePrefixSumWriteCount++;

            if (blankBytePrefixSum + blankBytePrefixSumWriteCount > 1 && blankBytePrefixSumWriteCount >= 8) {
                record();
                break;
            }
        }
    }
}

void BitBuffer::record() {

    currentBitAddress = 0;
    currentByteAddress = 0;

    do {
        if (clockSignalDidChange()) {
            append();
            lastClockSignal = currentClockSignal;
        }
    } while (currentByteAddress < byteCount - 1);
    
}

void BitBuffer::append() {
    bits[currentBitAddress] = readDataBit();
    currentBitAddress++;

    if (currentBitAddress == 8) {
        byteStorage[currentByteAddress]->setBits(bits);
        currentByteAddress++;
        currentBitAddress = 0;
    }
}

Byte** BitBuffer::getBytes() {
    Byte** bytes = nullptr;

    for (int i = 0; i < byteCount; i++) {
        bytes[i] = byteStorage[i];
    }

    return bytes;
}

const byte_t BitBuffer::getByteCount() {
    return byteCount;
}