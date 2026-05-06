#include <hdlc_parser/utility/crc_calculator.hpp>

namespace hdlc_parser {

std::array<uint16_t, 256> CRCalculator::table_;

void CRCalculator::init() {
    table_.fill(0);
    for (int i = 0; i < 256; ++i) {
        uint16_t crc = static_cast<uint16_t>(i) << 8;
        for (int j = 0; j < 8; ++j) {
            if ((crc & 0x8000) != 0) {
                crc = (crc << 1) ^ polynomial_;
            } else {
                crc <<= 1;
            }
        }
        table_[i] = crc;
    }
}

uint16_t CRCalculator::calculate(const uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; ++i) {
        uint8_t index = static_cast<uint8_t>((crc >> 8) ^ data[i]);
        crc = (crc << 8) ^ table_[index];
    }
    return static_cast<int16_t>(crc) ^ 0xFFFF;
}

}  // namespace hdlc_parser