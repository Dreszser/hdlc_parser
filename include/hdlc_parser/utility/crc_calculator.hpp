#pragma once
#include <array>

namespace hdlc_parser {

class CRCalculator {
   public:
    static uint16_t Calculate(const uint8_t* data, size_t length);

   private:
    constexpr static uint16_t polynomial_ = 0x1021;

    static std::array<uint16_t, 256> table_;
};

}  // namespace hdlc_parser