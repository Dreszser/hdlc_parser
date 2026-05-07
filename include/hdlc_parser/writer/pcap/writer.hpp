#pragma once
#include <fstream>
#include <hdlc_parser/writer/writer.hpp>

namespace hdlc_parser {
class PcapWriter : public Writer {
   public:
    explicit PcapWriter(const char* filename);
    void write(const frames_t& frames) override;

    ~PcapWriter() = default;

   private:
    void write_global_header();
    void write_frame(const frame_t& frame);

    std::ofstream file_;
};
}  // namespace hdlc_parser