#pragma once
#include <hdlc_parser/writer/writer.hpp>

namespace hdlc_parser {
class SigWriter : public Writer {
 public:
  explicit SigWriter(const char* filename);
  void write(const frames_t& frames) override;

 private:
  std::ofstream file_;
};
}  // namespace hdlc_parser