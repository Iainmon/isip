# ISIP Iain's Serial Instruction Protocall

A simple way for data to be sent across 2 pins with 2 different devices running at different speeds. See data_transmission_example.txt for a refference of what the message timeline looks like. The clock pin will negate it's value for every bit sent.

This is an example of what it would look like if the sender decided to send the value 2019.

| Clock Bits | Data Bits |
|------------|-----------|
| 1010 1010  | 1111 1111 |
| 1010 1010  | 0000 0000 |
| 1010 1010  | 0000 0100 |
| 1010 1010  | 0000 0010 |
| 1010 1010  | 0000 0000 |
| 1010 1010  | 0000 0001 |
| 1010 1010  | 0000 1001 |