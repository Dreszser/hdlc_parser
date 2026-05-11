#pragma once
#include <fstream>
#include <hdlc_parser/writer/writer.hpp>

namespace hdlc_parser {
class PcapWriter : public Writer {
   public:
    explicit PcapWriter(const std::string& filename);
    void write(const frames_t& frames) override;

    ~PcapWriter() = default;

    bool is_open() const override;

   private:
    void WriteGlobalHeader();
    void WriteFrame(const frame_t& frame);

    std::ofstream file_;
};
}  // namespace hdlc_parser