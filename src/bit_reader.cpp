#include <hdlc_parser/bit_reader.hpp>
#include <stdexcept>

namespace hdlc_parser {

void BitReader::Append(const char* data, std::size_t size) {
    buffer_.reserve(buffer_.size() + size);
    buffer_.insert(buffer_.end(), data, data + size);
}

bool BitReader::ReadBit(uint8_t& bit) {
    if (!HasBit()) {
        return false;
    }

    uint8_t byte = buffer_[byte_index_];
    bit = (byte >> bit_index_) & 1;

    bit_index_++;
    if (bit_index_ >= 8) {
        bit_index_ = 0;
        byte_index_++;
    }

    return true;
}

bool BitReader::HasBit() const { return byte_index_ < buffer_.size(); }

void BitReader::Reset() {
    byte_index_ = 0;
    bit_index_ = 0;
    buffer_.clear();
}

}  // namespace hdlc_parser
