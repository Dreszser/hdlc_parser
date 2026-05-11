#include <fstream>
#include <hdlc_parser/writer/sig/writer.hpp>

namespace hdlc_parser {

SigWriter::SigWriter(const char* filename)
    : file_(filename, std::ios::binary) {}

void SigWriter::write(const frames_t& frames) {
    for (const auto& frame : frames) {
        uint16_t size = static_cast<uint16_t>(frame.size());
        file_.write(reinterpret_cast<const char*>(&size), sizeof(size));
        file_.write(reinterpret_cast<const char*>(frame.data()), frame.size());
    }
}

bool SigWriter::is_open() const { return file_.is_open(); }

}  // namespace hdlc_parser