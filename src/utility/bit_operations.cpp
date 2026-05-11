#include <hdlc_parser/utility/bit_operations.hpp>

namespace hdlc_parser {

inline uint8_t BitOperations::ReverseBitsInByte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void BitOperations::ReverseBitsInFrame(frame_t& frame) {
    for (auto& b : frame) {
        b = ReverseBitsInByte(b);
    }
}

}  // namespace hdlc_parser
