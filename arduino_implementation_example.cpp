#include "BitBuffer.hpp"
#include "Byte.hpp"

#define CLOCK_PIN 10
#define DATA_PIN 9

bool digitalRead(int _pin);

bool _readClockBit(void);
bool _readDataBit(void);

// Example of three different subroutines
void subroutine1(void);
void subroutine2(void);
void subroutine3(void);

// Declares bitBuffer object refference
BitBuffer* bitBuffer = nullptr;

// Array of subroutines that the program will run depending on the active subroutine
void (* subroutines [])(void) = {subroutine1, subroutine2, subroutine3};
int currentSubroutine = 0; // The current subroutine that is active

void setup() {
    // Instantiates a new BitBuffer object
    bitBuffer = new BitBuffer();

    // Assigns the implementation specific callbacks for reading the bits of the clock and data pins
    bitBuffer->registerClockReadFunction(_readClockBit);
    bitBuffer->registerDataReadFunction(_readDataBit);
}

void loop() {

    // Checks for the ready to send signal
    if (bitBuffer->listenForOpenConnection()) {

        bitBuffer->recordIncomingData(); // Records the bitstream until the entire message has been sent
        
        if (bitBuffer->getBytes() == nullptr) {return;}

        int nextRoutine = (int)bitBuffer->getBytes()[0]->getValue(); // It has been agreed that the first byte will be used for the subroutine selction.
        
        // Makes sure the new routine index exists in the array to prevent any segfaults
        if (nextRoutine <= sizeof(subroutines)/sizeof(subroutines[0]) - 1) {
            currentSubroutine = nextRoutine; // The active subroutine is changed
        }
    }

    // Calls the active subroutine
    subroutines[currentSubroutine]();
}

// Implementation specific bit reading functions
bool _readClockBit(void) {return digitalRead(CLOCK_PIN);}
bool _readDataBit(void) {return digitalRead(DATA_PIN);}
