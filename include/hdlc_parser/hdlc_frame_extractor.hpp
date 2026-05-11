#pragma once
#include <hdlc_parser/bit_reader.hpp>
#include <hdlc_parser/types.hpp>

namespace hdlc_parser {

class HdlcFrameExtractor {
    constexpr static uint8_t FRAMES_TO_PUSH_COUNT = 10;

   public:
    enum class ReadResult { Success, FileNotFound, ReadError };

    HdlcFrameExtractor(size_t chunk_size = 1024);

    ReadResult ReadFromFile(const char* filename);

    ~HdlcFrameExtractor();

   private:
    void ParseBitChunk();

    void FinishFrame();
    void ResetFrameState();

    void HandleAbortSequence();
    void HandleFlag();
    void AccumulateBit(uint8_t bit);

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
};

}  // namespace hdlc_parser