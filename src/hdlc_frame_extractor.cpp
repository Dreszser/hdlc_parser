#include <fstream>
#include <hdlc_parser/hdlc_frame_extractor.hpp>
#include <hdlc_parser/queue.hpp>
#include <hdlc_parser/utility/bit_operations.hpp>
#include <hdlc_parser/utility/crc_calculator.hpp>
#include <hdlc_parser/utility/hdlc_logger.hpp>

namespace hdlc_parser {

HdlcFrameExtractor::HdlcFrameExtractor(size_t chunk_size)
    : chunk_size_(chunk_size), bit_reader_() {}

HdlcFrameExtractor::ReadResult HdlcFrameExtractor::ReadFromFile(
    const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        HdlcLogger::LogMessage(
            "Error: ", "Failed to open input file: ", filename);
        return ReadResult::FileNotFound;
    }
    std::vector<char> buffer(chunk_size_);
    while (true) {
        file.read(buffer.data(), buffer.size());
        std::streamsize n = file.gcount();
        if (n == 0) {
            break;
        }
        bit_reader_.SetBuffer(buffer.data(), n);
        file_byte_index_ += n;
        ParseBitChunk();
    }
    if (valid_frames_.size() > 0) {
        FrameQueue::GetInstance().push(std::move(valid_frames_));
    }
    HdlcLogger::LogMessage("Total valid frames parsed from ", filename, ": ",
                           total_valid_frames_count_);
    return ReadResult::Success;
}

void HdlcFrameExtractor::ParseBitChunk() {
    uint8_t bit;

    while (bit_reader_.ReadBit(bit)) {
        shift_ = (shift_ << 1) | bit;

        if (in_frame_ && ones_count_ > 6) {
            HandleAbortSequence();
        } else if (shift_ == 0x7E) {
            HandleFlag();
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
            AccumulateBit(bit);
        }
    }
}

void HdlcFrameExtractor::FinishFrame() {
    if (current_frame_.size() >= 4) {
        uint16_t recieved_crc =
            (static_cast<uint16_t>(current_frame_[current_frame_.size() - 2])
             << 8) |
            current_frame_[current_frame_.size() - 1];
        uint16_t calculated_crc = CRCalculator::Calculate(
            current_frame_.data(), current_frame_.size() - 2);

        if (recieved_crc == calculated_crc) {
            ++total_valid_frames_count_;
            BitOperations::ReverseBitsInFrame(current_frame_);
            valid_frames_.push_back(std::move(current_frame_));
            if (valid_frames_.size() == FRAMES_TO_PUSH_COUNT) {
                FrameQueue::GetInstance().push(std::move(valid_frames_));
                valid_frames_.clear();
            }
        } else {
            HdlcLogger::LogCrcMismatch(calculated_crc, recieved_crc,
                                       frame_start_byte_index_);
        }
    }
    current_frame_.clear();
}

void HdlcFrameExtractor::HandleAbortSequence() {
    current_frame_.clear();
    in_frame_ = false;
    ResetFrameState();
    ones_count_ = 0;
}

void HdlcFrameExtractor::HandleFlag() {
    if (in_frame_) {
        FinishFrame();
        ResetFrameState();
    }
    in_frame_ = true;
    ones_count_ = 0;

    frame_start_byte_index_ =
        file_byte_index_ - bit_reader_.RemainingBytesInBuffer();
}

void HdlcFrameExtractor::AccumulateBit(uint8_t bit) {
    current_byte_ = (current_byte_ << 1) | bit;
    bit_count_++;

    if (bit_count_ == 8) {
        current_frame_.push_back(current_byte_);
        ResetFrameState();
    }
}

void HdlcFrameExtractor::ResetFrameState() {
    bit_count_ = 0;
    current_byte_ = 0;
}

/* pushing terminal sequence */
HdlcFrameExtractor::~HdlcFrameExtractor() {
    FrameQueue::GetInstance().push({});
}

}  // namespace hdlc_parser