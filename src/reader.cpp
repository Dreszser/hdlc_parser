#include <fstream>
#include <hdlc_parser/reader.hpp>

namespace hdlc_parser {
Reader::Reader(uint32_t chunk_size) : chunk_size_(chunk_size), bit_reader_() {}

Reader::ReadResult Reader::read(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return ReadResult::FileNotFound;
    }
    char* buffer = new char[chunk_size_];
    /* while (file.read(buffer, chunk_size_)) {
        bit_reader_.Append(buffer, file.gcount());
        parse_bits();
    } */
    for (int i = 0; i < 20; ++i) {
        file.read(buffer, chunk_size_);
        bit_reader_.Append(buffer, file.gcount());
        parse_bits();
    }

    delete[] buffer;
    return ReadResult::Success;
}

void Reader::parse_bits() {
    uint8_t bit;
    while (bit_reader_.ReadBit(bit)) {
        shift_ = (shift_ << 1) | bit;

        if (ones_count_ >= 6 && bit == 1) {
            if (in_frame_) {
                current_frame_.clear();
                in_frame_ = false;
            }
            reset_bit_state();
            shift_ = 0;
            continue;
        }
        if (shift_ == 0x7E) {
            if (in_frame_) {
                finish_frame();
                in_frame_ = false;
            }
            start_new_frame();
            in_frame_ = true;

            shift_ = 0;
            continue;
        }

        if (!in_frame_) {
            continue;
        }

        if (ones_count_ == 5 && bit == 0) {
            ones_count_ = 0;
            continue;
        }

        if (bit == 1) {
            ones_count_++;
        } else {
            ones_count_ = 0;
        }

        current_byte_ = (current_byte_ << 1) | bit;
        bit_count_++;

        if (bit_count_ == 8) {
            current_frame_.push_back(current_byte_);
            current_byte_ = 0;
            bit_count_ = 0;
        }
    }
}

void Reader::start_new_frame() {
    current_frame_.clear();
    reset_bit_state();
}

void Reader::finish_frame() {
    if (!current_frame_.empty()) {
        // TODO: check CRC, write frame

        // TODO: remove later
        dump_hex(current_frame_);
    }
}
// TODO: remove later
void dump_hex(const frame_t& data) {
    static uint8_t count = 1;
    std::printf("Frame: %d \n", count++);
    std::printf("bytes: %zu \n", data.size());
    for (uint8_t b : data) {
        std::printf("%02X ", b);
    }
    std::printf("\n");
}

void Reader::reset_bit_state() {
    ones_count_ = 0;
    current_byte_ = 0;
    bit_count_ = 0;
}

/* void Reader::process_chunk(const char* buffer, std::streamsize size,
                           frames_t& frames) {
    frame_t frame(std::move(last_frame_begining_));
    for (std::streamsize i = 0; i < size; ++i) {
        uint8_t byte = static_cast<uint8_t>(buffer[i]);
        if (byte == 0x7E) {
            if (!frame.empty()) {
                frames.push_back(std::move(frame));
            }
            continue;
        }
        frame.push_back(byte);
    }
    last_frame_begining_ = std::move(frame);
} */
}  // namespace hdlc_parser