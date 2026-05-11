#include <hdlc_parser/utility/hdlc_logger.hpp>
#include <iomanip>
#include <sstream>

namespace hdlc_parser {

void HdlcLogger::LogCrcMismatch(const uint16_t calculated_crc,
                                const uint16_t recieved_crc,
                                const size_t frame_start_byte_index) {
    std::stringstream ss;
    ss << "CRC mismatch in frame from byte " << std::setw(4)
       << std::setfill('0') << std::hex << frame_start_byte_index
       << "\nCalculated CRC: " << std::setw(4) << std::setfill('0') << std::hex
       << calculated_crc << "\nRecieved CRC: " << std::setw(4)
       << std::setfill('0') << std::hex << recieved_crc << "\n";

    std::cout << ss.str();
}

void HdlcLogger::LogFrameHexDump(const frame_t& data) {
    std::stringstream ss;
    ss << "bytes: " << data.size() << "\n";

    for (const auto& byte : data) {
        ss << std::setw(2) << std::setfill('0') << std::hex
           << static_cast<int>(byte) << " ";
    }
    ss << "\n";
    std::cout << ss.str();
}

}  // namespace hdlc_parser
