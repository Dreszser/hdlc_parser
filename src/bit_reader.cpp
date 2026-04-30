#include <hdlc_parser/bit_reader.hpp>
#include <stdexcept>

namespace hdlc_parser {

BitReader::BitReader() : byte_index_(0), bit_index_(0) {}

void BitReader::append(const char* data, std::size_t size) {
    buffer_.reserve(buffer_.size() + size);
    buffer_.insert(buffer_.end(), data, data + size);
}

uint8_t BitReader::readBit() {
    if (!hasBit()) {
        throw std::runtime_error("BitReader: no more bits to read");
    }

    uint8_t byte = buffer_[byte_index_];
    uint8_t bit = (byte >> (7 - bit_index_)) & 1;

    // Advance to next bit
    bit_index_++;
    if (bit_index_ >= 8) {
        bit_index_ = 0;
        byte_index_++;
    }

    return bit;
}

uint8_t BitReader::peekBit() const {
    if (!hasBit()) {
        throw std::runtime_error("BitReader: no more bits to peek");
    }

    uint8_t byte = buffer_[byte_index_];
    uint8_t bit = (byte >> (7 - bit_index_)) & 1;

    return bit;
}

bool BitReader::hasBit() const { return byte_index_ < buffer_.size(); }

void BitReader::reset() {
    byte_index_ = 0;
    bit_index_ = 0;
}

}  // namespace hdlc_parser
