#ifndef PIPER_MIDI_H
#define PIPER_MIDI_H

#include <cstdint>

namespace PiperMidi {

  struct PackedPiperMidiMessage {
    uint8_t byte1;
    uint8_t byte2;
  };

  /**
   * Enum representing the status of a Piper MIDI message.
   * The status is determined by the two most significant bits of the first byte:
   * - 00: Note On
   * - 01: Note Off
   * - 10: Piston Press
   * - 11: Reserved
   */
  enum class PiperMidiMessageType { NoteOn, NoteOff, PistonPress, Reserved };

  /**
   * Represents a Piper MIDI message with rank number, note on/off status, and pipe number.
   * The message is packed into 2 bytes:
   * - Byte 0: Bits 0-6 for column number (0-63), Bits 7-8 for status bits.
   * (00 = note on, 01 = note off, 10 = piston press, 11 = reserved)
   * - Byte 1: Number (0-255)
   */
  struct PiperMidiMessage {
    uint8_t columnNumber;
    PiperMidiMessageType status;
    uint8_t number;

    static_assert(sizeof(uint8_t) == 1, "Platform must support 8-bit bytes");

    /**
     * Packs the PiperMidiMessage into a 2-byte buffer
     * @param buffer Pointer to a PackedPiperMidiMessage buffer where the packed message will be stored
     */
    void pack(PackedPiperMidiMessage* buffer) const {
      if (buffer == nullptr)
        return;
      if (buffer->byte2 >= 256)
        return;

      uint8_t status = (columnNumber & 0x3F);
      status |= (static_cast<uint8_t>(status) << 6);

      buffer->byte1 = status;
      buffer->byte2 = number;
    }

    /**
     * Unpacks a PiperMidiMessage from a 2-byte buffer
     * Note: the buffer must be at least 2 bytes long and properly formatted as per the pack() method.
     * @param buffer Pointer to a PackedPiperMidiMessage buffer containing the properly formed packed message
     */
    void unpack(const PackedPiperMidiMessage* buffer) {
      columnNumber = buffer->byte1 & 0x3F;
      status = static_cast<PiperMidiMessageType>((buffer->byte1 >> 6) & 0x03);
      number = buffer->byte2;
    }
  };
} // namespace PiperMidi

#endif // PIPER_MIDI_H