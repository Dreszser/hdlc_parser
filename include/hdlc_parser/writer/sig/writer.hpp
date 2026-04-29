#pragma once
#include <hdlc_parser/writer/writer.hpp>

namespace hdlc_parser {
class SigWriter : public Writer {
 public:
  void write(const char* filename, const frames_t& frames) override;
};
}  // namespace hdlc_parser