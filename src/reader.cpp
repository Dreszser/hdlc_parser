#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/utility/crc_calculator.hpp>
#include <fstream>

namespace hdlc_parser {

Reader::Reader(uint32_t chunk_size) : chunk_size_(chunk_size), bit_reader_() {}

Reader::ReadResult Reader::read(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return ReadResult::FileNotFound;
    }
    std::vector<char> buffer(chunk_size_);
    while (file.read(buffer.data(), chunk_size_)) {
        bit_reader_.Append(buffer.data(), file.gcount());
        parse_bits();
    }
    return ReadResult::Success;
}

void Reader::parse_bits() {
    uint8_t bit;

    uint8_t shift = 0;
    int i = 0;
    while (bit_reader_.ReadBit(bit)) {
        shift = (shift << 1) | bit;

        if (bit == 1) {
            ones_count_++;
            if (in_frame_ && ones_count_ > 6) {
                handle_abort();
            }
        } else {
            if (ones_count_ == 5) {
                ones_count_ = 0;
                continue;
            }
            ones_count_ = 0;
        }

        if (shift == 0x7E) {
            handle_flag();
        } else if (in_frame_) {
            accumulate_bit(bit);
        }
        ++i;
    }
    (void)i;
}

void Reader::finish_frame() {
    if (current_frame_.size() >= 4) {
        uint16_t recieved_crc = (static_cast<uint16_t>(current_frame_[current_frame_.size() - 2]) << 8) | 
                                 current_frame_[current_frame_.size() - 1];
        uint16_t calculated_crc = CRCalculator::calculate(current_frame_.data(), current_frame_.size() - 2);

        if (recieved_crc == calculated_crc) {
            // std::printf("Frame received successfully with CRC: %04X\n", recieved_crc);
            // TODO: remove later, change to write to file
        } else {
            std::printf("CRC mismatch\n");
            std::printf("Calculated CRC: %04X\n", calculated_crc);
            std::printf("Received CRC: %04X\n", recieved_crc);
        }
    }
    current_frame_.clear();
}

void Reader::handle_abort() {
    current_frame_.clear();
    in_frame_ = false;
    reset_frame_state();
}

void Reader::handle_flag() {
    if (in_frame_) {
        finish_frame();
        reset_frame_state();
    }
    in_frame_ = true;
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


}  // namespace hdlc_parser