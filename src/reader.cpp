#include <fstream>
#include <hdlc_parser/reader.hpp>

namespace hdlc_parser {

Reader::Reader(uint32_t chunk_size) : chunk_size_(chunk_size), bit_reader_() {}

Reader::ReadResult Reader::read(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return ReadResult::FileNotFound;
    }
    std::vector<char> buffer(chunk_size_);
    /* while (file.read(buffer, chunk_size_)) {
        bit_reader_.Append(buffer, file.gcount());
        parse_bits();
    } */
    for (int i = 0; i < 5; ++i) {
        file.read(buffer.data(), chunk_size_);
        bit_reader_.Append(buffer.data(), file.gcount());
        parse_bits();
    }
    return ReadResult::Success;
}

void Reader::parse_bits() {
    uint8_t bit;

    while (bit_reader_.ReadBit(bit)) {
        shift_ = ((shift_ << 1) | bit) & 0xFF;

        if (in_frame_ && ones_count_ > 6) {
            handle_abort();
        } else if (shift_ == 0x7E) {
            handle_flag();
        } else if (in_frame_) {
            if (!handle_bit_stuffing(bit)) {
                accumulate_bit(bit);
            }
        }
    }
}

void Reader::start_new_frame() {
    current_frame_.clear();
    reset_bit_state();
}

void Reader::finish_frame() {
    if (!current_frame_.empty()) {
        size_t frame_bits_count = current_frame_.size() * 8 - (7 - bit_count_);
        // TODO: check CRC, write frame

        // TODO: remove later
        dump_hex(current_frame_, frame_bits_count);
    }
}

// TODO: remove later
void Reader::dump_hex(const frame_t& data, size_t payload_bits) {
    static uint8_t count = 1;
    std::printf("Frame: %d\n", count++);

    size_t full_bytes = payload_bits / 8;
    size_t leftover_bits = payload_bits % 8;

    std::printf("bits: %zu\n", payload_bits);

    for (size_t i = 0; i < full_bytes; ++i) {
        std::printf("%02X ", data[i]);
    }

    // Print last partial byte in binary if exists
    if (leftover_bits) {
        uint8_t last_byte = data[full_bytes];
        last_byte &= ((1 << leftover_bits) - 1) << (8 - leftover_bits);

        std::printf("| ");
        for (int i = 7; i >= 8 - (int)leftover_bits; --i) {
            std::printf("%d", (last_byte >> i) & 1);
        }
    }
    std::printf("\n");
}

void Reader::reset_bit_state() {
    ones_count_ = 0;
    reset_bit_accumulator();
}

void Reader::reset_bit_accumulator() {
    current_byte_ = 0;
    bit_count_ = 0;
}

void Reader::handle_abort() {
    current_frame_.clear();
    in_frame_ = false;
    reset_bit_state();
    shift_ = 0;
}

void Reader::handle_flag() {
    if (in_frame_) {
        finish_frame();
    }
    start_new_frame();
    in_frame_ = true;
    shift_ = 0;
}

// true if bit is stuffed and should be ignored, false otherwise
bool Reader::handle_bit_stuffing(uint8_t bit) {
    if (ones_count_ == 5 && bit == 0) {
        ones_count_ = 0;
        return true;
    }
    return false;
}

void Reader::accumulate_bit(uint8_t bit) {
    current_byte_ = (current_byte_ << 1) | bit;
    bit_count_++;

    if (bit_count_ == 8) {
        current_frame_.push_back(current_byte_);
        reset_bit_accumulator();
    }
    ones_count_ = (bit == 1) ? (ones_count_ + 1) : 0;
}

}  // namespace hdlc_parser