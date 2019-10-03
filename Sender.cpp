#include "Sender.hpp"
#include "Byte.hpp"

#include <vector>
#include <thread>
#include <chrono>

#define MESSAGE_READY_SIGNAL_LENGTH 10
#define MESSAGE_READY_SIGNAL_SLEEP_TIME_MICROS 100

#define BIT_WRITE_DELAY_MICROS 200

using namespace std;

Sender::Sender() {
    
    bit_t* bitTemplate = nullptr;
    bitTemplate = new bit_t[8];

    for (byte_t i = 0; i < 8; i++) {
        bitTemplate[i] = false;
    }

    zeroByte = new Byte();
    zeroByte->setBits(bitTemplate);

    for (byte_t i = 0; i < 8; i++) {
        bitTemplate[i] = true;
    }
    fullByte = new Byte();
    fullByte->setBits(bitTemplate);

    delete [] bitTemplate;
}

void Sender::loadMessage(const vector<Byte*> _bytes) {
    byteMessage = _bytes;
}

void Sender::writeBit(const bit_t _bit) {
    lastClockValue = !lastClockValue;
    writeClockBit(lastClockValue);
    writeDataBit(_bit);
    this_thread::sleep_for(chrono::microseconds(BIT_WRITE_DELAY_MICROS));
}

void Sender::writeByte(Byte* _byte) {
    for (byte_t i = 0; i < 8; i++) {
        writeBit(_byte->getBit(i));
    }
}

void Sender::registerClockWriteFunction(void (*_clockWriteFunction)(bit_t)) {
    writeClockBit = _clockWriteFunction;
}
void Sender::registerDataWriteFunction(void (*_dataWriteFunction)(bit_t)) {
    writeDataBit = _dataWriteFunction;
}

void Sender::beginMessageReadySequence() {
    for (unsigned long i = 0; i < MESSAGE_READY_SIGNAL_LENGTH; i++) {
        writeBit(true);
        this_thread::sleep_for(chrono::microseconds(MESSAGE_READY_SIGNAL_SLEEP_TIME_MICROS));
    }
}

void Sender::writeMessageWillStartSignal() {
    for (byte_t i = 0; i < 8; i++) {
        writeBit(false);
    }
}

void Sender::writeMessageLengthPrefix() {

    byte_t messageLength = byteMessage.size();

    for (byte_t i = 0; i < 8; i++) {
        writeBit(messageLength & (1 << i));
    }
}

void Sender::writeMessageContents() {
    for (byte_t i = 0; i < byteMessage.size(); i++) {
        writeByte(byteMessage[i]);
    }
    writeBit(false);
}

void Sender::send() {
    beginMessageReadySequence();
    writeMessageWillStartSignal();
    writeMessageLengthPrefix();
    writeMessageContents();
}