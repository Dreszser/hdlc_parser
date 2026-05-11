#pragma once
#include <hdlc_parser/types.hpp>
#include <iostream>

namespace hdlc_parser {

class HdlcLogger {
   public:
    template <typename... Args>
    static void LogMessage(Args&&... args) {
        (std::cout << ... << args) << "\n";
    }

    static void LogCrcMismatch(const uint16_t calculated_crc,
                               const uint16_t recieved_crc,
                               const size_t frame_start_byte_index);
    static void LogFrameHexDump(const frame_t& data);
};

}  // namespace hdlc_parser