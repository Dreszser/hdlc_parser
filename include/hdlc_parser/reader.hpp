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
    /* void process_chunk(const char* buffer, std::streamsize size,
                       frames_t& frames); */

    void parse_bits();

    void start_new_frame();
    void finish_frame();
    void reset_bit_state();

    frame_t current_frame_;
    uint32_t chunk_size_;

    BitReader bit_reader_;

    uint8_t ones_count_ = 0;

    uint8_t current_byte_ = 0;
    uint8_t bit_count_ = 0;

    uint8_t shift_ = 0;
    bool in_frame_ = false;
};

// TODO: remove later
void dump_hex(const frame_t& data);

}  // namespace hdlc_parser