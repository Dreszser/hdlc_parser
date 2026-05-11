#include <hdlc_parser/utility/bit_operations.hpp>

namespace hdlc_parser {

inline uint8_t BitOperations::reverse_bits_in_byte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void BitOperations::reverse_bits_in_frame(frame_t& frame) {
    for (auto& b : frame) {
        b = reverse_bits_in_byte(b);
    }
}

}  // namespace hdlc_parser
