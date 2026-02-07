# piper-midi

## Overview

piper-midi is a C++ library that provides a lightweight alternative for MIDI, specifically designed for pipe organs and the embedded systems that control them. Its original goal was to elimiate the time domain note smear that occurs when using standard MIDI over a single port to control multiple ranks of pipes.

## Background
Traditional MIDI runs at a baud rate of 31,250 bits per second. This translates to an effective note speed of 1 millisecond per note.
```cpp
struct MidiPacket {
    uint8_t status; // Status byte
    uint8_t note;   // Note number
    uint8_t velocity; // Velocity
};
```
```
1 start bit
8 data bits
1 stop bit
Total: 10 bits per byte
3 bytes per note (status, note number, velocity)
Total time per note: 30 bits * 32 microseconds/bit = 960 microseconds = 0.96 milliseconds
```
For normal organ ranks, we do not need velocity data, so we omit it. This reduces the time per note to 20 bits, or 640 microseconds. 
We can further reduce the amount of time it takes to send a note by increasing the baud rate. RS-485 can run at 1Mbaud, which would reduce the time per note to 20 microseconds. This is a significant improvement over standard MIDI:
```
20 bits per note
At 1Mbaud: 20 bits * 1 microsecond/bit = 20 microseconds per note
50,000 notes per second!
```
Even the largest pipe organs do not play more than half of the pipes at once, in fact some control systems have a "grand cipher" protection mechanism that turns off all outputs on a rank when more than half of the outputs are set to on. This project is not aimed at replacing control systems on large pipe organs, but rather to provide a lightweight alternative for small to medium sized organs, or for experimental projects.

## Packets

piper-midi uses a similar packet structure to MIDI, but increases the amount of channels and reduces the amount of message types. The packet structure is as follows:
```
Byte 1: Status
    Bits 1-6: unsigned 6-bit column number (0-63)
    Bits 7-8: status bits (00 = note on, 01 = note off, 10 = piston press, 11 = reserved)
    Byte 2: unsigned 8-bit number (0-255)
```
```cpp
  struct PiperMidiMessage {
    uint8_t columnNumber : 6;
    uint8_t status : 2;
    uint8_t number : 8;
  };
```