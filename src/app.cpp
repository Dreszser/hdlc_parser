#include <hdlc_parser/app.hpp>
#include <hdlc_parser/queue.hpp>
#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/utility/crc_calculator.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>
#include <hdlc_parser/writer/sig/writer.hpp>
#include <memory>
#include <thread>
#include <vector>

namespace hdlc_parser {

void reader_cb(const std::string& filename, uint32_t chunk_size) {
    Reader reader(chunk_size);
    reader.read(filename.data());
}

void writer_cb(const Config& cfg) {
    std::vector<std::unique_ptr<Writer>> writers;
    switch (cfg.output_format) {
        case Config::OutputFormat::PCAP: {
            writers.emplace_back(
                std::make_unique<PcapWriter>(cfg.output_file_pcap.data()));
            break;
        }
        case Config::OutputFormat::SIG: {
            writers.emplace_back(
                std::make_unique<SigWriter>(cfg.output_file_sig.data()));
            break;
        }
        case Config::OutputFormat::BOTH: {
            writers.emplace_back(
                std::make_unique<PcapWriter>(cfg.output_file_pcap.data()));
            writers.emplace_back(
                std::make_unique<SigWriter>(cfg.output_file_sig.data()));
            break;
        }
        default:
            break;
    }

    while (true) {
        frames_t frames;
        if (!FrameQueue::getInstance().pop(frames)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }
        /* recieved terminal sequence */
        if (frames.empty()) {
            break;
        }

        for (const auto& writer : writers) {
            writer->write(frames);
        }
    }
}

void App::start(const Config& config) {
    CRCalculator::init();
    std::jthread reader_thread(reader_cb, config.input_file,
                               config.read_chunk_size);

    std::jthread writer_thread(writer_cb, config);
}
}  // namespace hdlc_parser