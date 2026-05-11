#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace hdlc_parser {

class BitReader {
   public:
    BitReader() = default;

    void SetBuffer(const char* data, std::size_t size);

    bool ReadBit(uint8_t& bit);

    // Check if there are more bits to read
    bool HasBit() const;

    size_t RemainingBytesInBuffer() const;

   private:
    const char* buffer_ = nullptr;
    size_t buffer_size_ = 0;

    std::size_t byte_index_ = 0;  // Current byte position
    uint8_t bit_index_ = 0;  // Current bit within the byte (0-7, where 0 = MSB)
};

}  // namespace hdlc_parser