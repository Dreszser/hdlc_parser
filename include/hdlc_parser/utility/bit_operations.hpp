#pragma once
#include <cstdint>
#include <hdlc_parser/types.hpp>

namespace hdlc_parser {

class BitOperations {
   public:
    static inline uint8_t reverse_bits_in_byte(uint8_t b);
    static void reverse_bits_in_frame(frame_t& frame);
};

}  // namespace hdlc_parser