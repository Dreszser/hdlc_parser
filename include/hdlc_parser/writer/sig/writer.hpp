#pragma once
#include <fstream>
#include <hdlc_parser/writer/writer.hpp>

namespace hdlc_parser {
class SigWriter : public Writer {
   public:
    explicit SigWriter(const char* filename);
    void write(const frames_t& frames) override;

    ~SigWriter() = default;

    bool is_open() const;

   private:
    std::ofstream file_;
};
}  // namespace hdlc_parser