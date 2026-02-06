#ifndef PIPER_MIDI_H
#define PIPER_MIDI_H

#include <cstdint>

namespace PiperMidi {

  /**
   * Represents a Piper MIDI message with rank number, note on/off status, and pipe number.
   * The message is packed into 2 bytes:
   * - Byte 0: Bits 0-6 for rank number (0-127), Bit 7 for note on/off (1 for on, 0 for off)
   * - Byte 1: Note number (0-255)
   */
  struct PiperMidiMessage {
    uint8_t rankNumber;
    bool isNoteOn;
    uint8_t noteNumber;

    static_assert(sizeof(uint8_t) == 1, "Platform must support 8-bit bytes");

    /**
     * Packs the PiperMidiMessage into a 2-byte buffer
     * @param buffer Pointer to a 2-byte buffer where the packed message will be stored
     */
    void pack(uint8_t* buffer) const {
      if (buffer == nullptr)
        return;
      if (buffer[1] >= 256)
        return;

      uint8_t status = (rankNumber & 0x7F);
      if (isNoteOn)
        status |= 0x80;

      buffer[0] = status;
      buffer[1] = noteNumber;
    }

    /**
     * Unpacks a PiperMidiMessage from a 2-byte buffer
     * Note: the buffer must be at least 2 bytes long and properly formatted as per the pack() method.
     * @param buffer Pointer to a 2-byte buffer containing the properly formed packed message
     * @return Unpacked PiperMidiMessage
     */
    static PiperMidiMessage unpack(const uint8_t* buffer) {
      PiperMidiMessage message;
      message.rankNumber = buffer[0] & 0x7F;
      message.isNoteOn = (buffer[0] & 0x80) != 0;
      message.noteNumber = buffer[1];
      return message;
    }
  };

} // namespace PiperMidi

#endif // PIPER_MIDI_H