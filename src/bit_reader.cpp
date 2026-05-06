#include <hdlc_parser/bit_reader.hpp>

namespace hdlc_parser {

void BitReader::SetBuffer(const char* data, size_t size) {
    buffer_ = data;
    buffer_size_ = size;
    byte_index_ = 0;
    // bit_index_ = 0;
}

bool BitReader::ReadBit(uint8_t& bit) {
    if (!HasBit()) {
        return false;
    }

    uint8_t byte = static_cast<uint8_t>(buffer_[byte_index_]);
    bit = (byte >> bit_index_) & 1;

    bit_index_++;
    if (bit_index_ >= 8) {
        bit_index_ = 0;
        byte_index_++;
    }

    return true;
}

bool BitReader::HasBit() const { return byte_index_ < buffer_size_; }

size_t BitReader::RemainingBytesInBuffer() const {
    return buffer_size_ - byte_index_;
}

}  // namespace hdlc_parser
