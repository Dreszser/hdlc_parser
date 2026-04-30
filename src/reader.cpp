#include <fstream>
#include <hdlc_parser/reader.hpp>

namespace hdlc_parser {
Reader::Reader(uint32_t chunk_size)
    : chunk_size_(chunk_size), bit_reader_(), shift_(0) {}

Reader::ReadResult Reader::read(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return ReadResult::FileNotFound;
    }
    char* buffer = new char[chunk_size_];
    while (file.read(buffer, chunk_size_)) {
        bit_reader_.append(buffer, file.gcount());
        parse_bits();
    }
    delete[] buffer;
    return ReadResult::Success;
}

void Reader::parse_bits() {
    while (bit_reader_.hasBit()) {
        uint8_t bit = bit_reader_.readBit();
        shift_ = (shift_ << 1) | bit;
        if (shift_ == 0x7E) {
            if (in_frame_) {
                // End of frame
                in_frame_ = false;
            } else {
                // start of frame
                in_frame_ = true;
            }
        }
    }
}

void Reader::process_chunk(const char* buffer, std::streamsize size,
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
}
}  // namespace hdlc_parser