#pragma once
#include <cstdint>
#include <hdlc_parser/types.hpp>

namespace hdlc_parser {

class BitOperations {
   public:
    static inline uint8_t ReverseBitsInByte(uint8_t b);
    static void ReverseBitsInFrame(frame_t& frame);
};

}  // namespace hdlc_parser