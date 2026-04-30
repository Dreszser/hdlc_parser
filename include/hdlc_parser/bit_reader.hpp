#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace hdlc_parser {

class BitReader {
   public:
    BitReader();

    void Append(const char* data, std::size_t size);

    bool ReadBit(uint8_t& bit);

    bool PeekBit(uint8_t& bit) const;

    // Check if there are more bits to read
    bool HasBit() const;

    // Reset all internal state (for testing or new file)
    void Reset();

   private:
    std::vector<uint8_t> buffer_;
    std::size_t byte_index_;  // Current byte position
    uint8_t bit_index_;  // Current bit within the byte (0-7, where 0 = MSB)
};

}  // namespace hdlc_parser