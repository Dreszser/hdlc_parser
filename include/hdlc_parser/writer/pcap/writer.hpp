#pragma once
#include <hdlc_parser/writer/writer.hpp>

namespace hdlc_parser {
class PcapWriter : public Writer {
 public:
  explicit PcapWriter(const char* filename);
  void write(const char* filename, const frames_t& frames) override;

 private:
  void write_global_header();
  void write_packet(const frame_t& frame);

  std::ofstream file_;
};
}  // namespace hdlc_parser