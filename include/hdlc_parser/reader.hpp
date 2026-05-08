#pragma once
#include <hdlc_parser/bit_reader.hpp>
#include <hdlc_parser/types.hpp>

namespace hdlc_parser {

class Reader {
    constexpr static uint8_t FRAMES_TO_PUSH_COUNT = 10;

   public:
    enum class ReadResult { Success, FileNotFound, ReadError };

    Reader(size_t chunk_size = 1024);

    ReadResult read(const char* filename);

    ~Reader();

   private:
    void parse_bits();

    void finish_frame();
    void reset_frame_state();

    void handle_abort();
    void handle_flag();
    void accumulate_bit(uint8_t bit);

    void log_mismatch_crc_frame(const uint16_t& calculated_crc,
                                const uint16_t& recieved_crc);

    inline uint8_t reverse_bits(uint8_t b);
    void reverse_bits_in_frame(frame_t& frame);

    frame_t current_frame_;
    size_t chunk_size_;

    BitReader bit_reader_;

    uint8_t ones_count_ = 0;

    uint8_t current_byte_ = 0;
    uint8_t bit_count_ = 0;

    bool in_frame_ = false;

    frames_t valid_frames_;

    size_t file_byte_index_ = 0;
    size_t frame_start_byte_index_ = 0;

    uint8_t shift_ = 0;

    size_t total_valid_frames_count_ = 0;

    /* debug helper */
    void dump_hex(const frame_t& data);
};

}  // namespace hdlc_parser