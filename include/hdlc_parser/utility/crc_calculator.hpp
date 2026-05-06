#pragma once
#include <array>

namespace hdlc_parser
{

class CRCalculator
{
    constexpr static uint16_t polynomial_ = 0x1021;

public:
    static void init();

    static uint16_t calculate(const uint8_t* data, size_t length);

private:
    static std::array<uint16_t, 256> table_;
};

}