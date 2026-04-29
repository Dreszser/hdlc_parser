#include <hdlc_parser/writer/sig/writer.hpp>
#include <fstream>

namespace hdlc_parser {
void SigWriter::write(const char* filename,
                      const frames_t& frames) {
  std::ofstream file(filename, std::ios::binary);
  for (const auto& frame : frames) {
    uint16_t size = static_cast<uint16_t>(frame.size());
    file.put(static_cast<uint8_t>(size & 0xFF));
    file.put(static_cast<uint8_t>((size >> 8) & 0xFF));
    for (uint8_t byte : frame) {
      file.put(byte);
    }
  }
}
}  // namespace hdlc_parser 