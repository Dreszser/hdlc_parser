#pragma once
#include <hdlc_parser/bit_reader.hpp>
#include <hdlc_parser/types.hpp>

namespace hdlc_parser {

class Reader {
   public:
    enum class ReadResult { Success, FileNotFound, ReadError };

    Reader(uint32_t chunk_size = 1024);

    ReadResult read(const char* filename);

   private:
    void process_chunk(const char* buffer, std::streamsize size,
                       frames_t& frames);

    void parse_bits();

    frame_t last_frame_begining_;
    uint32_t chunk_size_;

    BitReader bit_reader_;

    uint8_t shift_;
    bool in_frame_;
};
}  // namespace hdlc_parser