#include <fstream>
#include <hdlc_parser/writer/pcap/header.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>

namespace hdlc_parser {

PcapWriter::PcapWriter(const char* filename)
    : file_(filename, std::ios::binary) {
  write_global_header();
}

void PcapWriter::write_global_header() {
  PcapGlobalHeader header;
  file_.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

void PcapWriter::write(const char* filename,
                       const frames_t& frame) {
  std::ofstream file(filename, std::ios::binary);

  // Implementation for writing PCAP format
}
}  // namespace hdlc_parser