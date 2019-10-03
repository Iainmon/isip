#include <iostream>
#include <vector>
#include <thread>
#include "Byte.hpp"
#include "Byte.cpp"
#include "BitBuffer.hpp"
#include "BitBuffer.cpp"
#include "Sender.hpp"
#include "Sender.cpp"

using namespace std;

bit_t clockPin = false;
bit_t dataPin = false;

bit_t readClock() {
    return clockPin;
}
bit_t readData() {
    return dataPin;
}
void writeClock(bit_t _value) {
    clockPin = _value;
}
void writeData(bit_t _value) {
    dataPin = _value;
}


void sender_program(void (*_writeClockFunc)(bit_t), void (*_writeDataFunc)(bit_t)) {

    cout << "Sender thread started!" << endl;

    Sender* sender = new Sender();
    sender->registerClockWriteFunction(_writeClockFunc);
    sender->registerDataWriteFunction(_writeDataFunc);

    cout << "Assigned callbacks." << endl;


    bit_t* bitsToPopulateByte = nullptr;
    bitsToPopulateByte = new bit_t[8];

    cout << "Created bit population array." << endl;

    vector<Byte*> message;
    message.push_back(new Byte());

    int intToSend;
    while (true) {
        
        cout << "Please enter a number between 0 and 255:" << endl;

        cin >> intToSend;

        cout << "Sending: " << intToSend << endl;

        for (byte_t i = 0; i < 8; i++) {
            bitsToPopulateByte[i] = (intToSend & (1 << i));
        }

        cout << "Processed bits." << endl;

        message[0]->setBits(bitsToPopulateByte);

        cout << "Loaded bits into Byte object." << endl;

        sender->loadMessage(message);
        
        cout << "Loaded message into sender." << endl;
        cout << "Sending message..." << endl;

        sender->send();

        cout << "Message sent!" << endl;

    }

}

void reciever_program(bit_t (*_readClockFunc)(void), bit_t (*_readDataFunc)(void)) {

    cout << "Reciever thread started!" << endl;

    BitBuffer* bitBuffer = new BitBuffer();

    cout << "Initialized BitBuffer object." << endl;

    bitBuffer->registerClockReadFunction(_readClockFunc);
    bitBuffer->registerDataReadFunction(_readDataFunc);

    cout << "Assigned callbacks." << endl;

    while (true) {
        if (bitBuffer->listenForOpenConnection()) {
            cout << "Message present!" << endl;

            bitBuffer->recordIncomingData(); // Records the bitstream until the entire message has been sent
            
            if (bitBuffer->getBytes() == nullptr) {continue;}

            int firstBitValue = (int)bitBuffer->getBytes()[0].getValue();
            
            cout << "Recieved value: " << firstBitValue << endl;

        }
    }
}



int main() {
    thread t_sender(sender_program, writeClock, writeData);
    thread t_reciever(reciever_program, readClock, readData);
    t_sender.join();
    t_reciever.join();
}