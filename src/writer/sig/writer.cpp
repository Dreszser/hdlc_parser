#include <fstream>
#include <hdlc_parser/writer/sig/writer.hpp>

namespace hdlc_parser {

SigWriter::SigWriter(const char* filename)
    : file_(filename, std::ios::binary) {}

void SigWriter::write(const frames_t& frames) {
    for (const auto& frame : frames) {
        uint16_t size = static_cast<uint16_t>(frame.size());
        file_.put(static_cast<uint8_t>((size >> 8) & 0xFF));
        file_.put(static_cast<uint8_t>(size & 0xFF));
        for (uint8_t byte : frame) {
            file_.put(byte);
        }
    }
}

SigWriter::~SigWriter() { file_.close(); }
}  // namespace hdlc_parser