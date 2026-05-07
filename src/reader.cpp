#include <fstream>
#include <hdlc_parser/queue.hpp>
#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/utility/crc_calculator.hpp>
#include <iostream>

namespace hdlc_parser {

Reader::Reader(uint32_t chunk_size) : chunk_size_(chunk_size), bit_reader_() {}

Reader::ReadResult Reader::read(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << filename;
        return ReadResult::FileNotFound;
    }
    std::vector<char> buffer(chunk_size_);
    while (file.read(buffer.data(), chunk_size_)) {
        size_t n = file.gcount();
        bit_reader_.SetBuffer(buffer.data(), n);
        file_byte_index_ += n;
        parse_bits();
    }
    if (valid_frames_.size() > 0) {
        FrameQueue::getInstance().push(std::move(valid_frames_));
    }
    std::cout << std::format("Total valid frames parsed: {}",
                             total_valid_frames_count_);
    return ReadResult::Success;
}

void Reader::parse_bits() {
    uint8_t bit;

    while (bit_reader_.ReadBit(bit)) {
        shift_ = (shift_ << 1) | bit;

        if (in_frame_ && ones_count_ > 6) {
            handle_abort();
        } else if (shift_ == 0x7E) {
            handle_flag();
        } else if (in_frame_) {
            if (bit == 1) {
                ones_count_++;
            } else {
                if (ones_count_ == 5) {
                    ones_count_ = 0;
                    continue;
                }
                ones_count_ = 0;
            }
            accumulate_bit(bit);
        }
    }
}

void Reader::finish_frame() {
    if (current_frame_.size() >= 4) {
        uint16_t recieved_crc =
            (static_cast<uint16_t>(current_frame_[current_frame_.size() - 2])
             << 8) |
            current_frame_[current_frame_.size() - 1];
        uint16_t calculated_crc = CRCalculator::calculate(
            current_frame_.data(), current_frame_.size() - 2);

        if (recieved_crc == calculated_crc) {
            ++total_valid_frames_count_;
            valid_frames_.push_back(std::move(current_frame_));
            if (valid_frames_.size() == FRAMES_TO_PUSH_COUNT) {
                FrameQueue::getInstance().push(std::move(valid_frames_));
                valid_frames_.clear();
            }
        } else {
            log_mismatch_crc_frame(calculated_crc, recieved_crc);
        }
    }
    current_frame_.clear();
}

void Reader::log_mismatch_crc_frame(const uint16_t& calculated_crc,
                                    const uint16_t& recieved_crc) {
    std::cout << std::format("CRC mismatch in frame from byte {:04X}\n",
                             frame_start_byte_index_);
    std::cout << std::format("Calculated CRC: {:04X}\n", calculated_crc);
    std::cout << std::format("Recieved CRC: {:04X}\n", recieved_crc);
}

void Reader::handle_abort() {
    current_frame_.clear();
    in_frame_ = false;
    reset_frame_state();
    ones_count_ = 0;
}

void Reader::handle_flag() {
    if (in_frame_) {
        finish_frame();
        reset_frame_state();
    }
    in_frame_ = true;
    ones_count_ = 0;

    frame_start_byte_index_ =
        file_byte_index_ - bit_reader_.RemainingBytesInBuffer();
}

void Reader::accumulate_bit(uint8_t bit) {
    current_byte_ = (current_byte_ << 1) | bit;
    bit_count_++;

    if (bit_count_ == 8) {
        current_frame_.push_back(current_byte_);
        reset_frame_state();
    }
}

void Reader::reset_frame_state() {
    bit_count_ = 0;
    current_byte_ = 0;
}

/* pushing terminal sequence */
Reader::~Reader() { FrameQueue::getInstance().push({}); }

/* helper function */
void Reader::dump_hex(const frame_t& data) {
    std::printf("bytes: %zu\n", data.size());

    for (const auto& byte : data) {
        std::printf("%02X ", byte);
    }
    std::printf("\n");
}

}  // namespace hdlc_parser