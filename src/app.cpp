#include <hdlc_parser/app.hpp>
#include <hdlc_parser/hdlc_frame_extractor.hpp>
#include <hdlc_parser/queue.hpp>
#include <hdlc_parser/utility/hdlc_logger.hpp>
#include <hdlc_parser/writer/pcap/pcap_writer.hpp>
#include <hdlc_parser/writer/sig/sig_writer.hpp>

namespace hdlc_parser {

void App::FrameExtractorCb(const std::string& filename, size_t chunk_size) {
    HdlcFrameExtractor frame_extractor(chunk_size);
    frame_extractor.ReadFromFile(filename);
}

bool App::AddWriter(std::vector<std::unique_ptr<Writer>>& writers,
                    std::unique_ptr<Writer> writer,
                    const std::string& filename) {
    if (!writer->is_open()) {
        HdlcLogger::LogMessage("Failed to open output file: ", filename);
        return false;
    }
    writers.emplace_back(std::move(writer));
    return true;
}

std::vector<std::unique_ptr<Writer>> App::InitializeWriters(const Config& cfg) {
    std::vector<std::unique_ptr<Writer>> writers;

    switch (cfg.output_format) {
        case Config::OutputFormat::PCAP:
            AddWriter(writers,
                      std::make_unique<PcapWriter>(cfg.output_file_pcap),
                      cfg.output_file_pcap);
            break;
        case Config::OutputFormat::SIG:
            AddWriter(writers, std::make_unique<SigWriter>(cfg.output_file_sig),
                      cfg.output_file_sig);
            break;
        case Config::OutputFormat::BOTH:
            AddWriter(writers,
                      std::make_unique<PcapWriter>(cfg.output_file_pcap),
                      cfg.output_file_pcap);
            AddWriter(writers, std::make_unique<SigWriter>(cfg.output_file_sig),
                      cfg.output_file_sig);
            break;
        default:
            break;
    }

    return writers;
}

void App::WriterCb(const Config& cfg) {
    std::vector<std::unique_ptr<Writer>> writers = InitializeWriters(cfg);

    if (writers.empty()) {
        return;
    }

    while (true) {
        frames_t frames;
        if (!FrameQueue::GetInstance().pop(frames)) {
            break;
        }

        for (const auto& writer : writers) {
            writer->write(frames);
        }
    }
}

void App::Start(const Config& config) {
    ThreadGuard extractor_guard(std::thread(
        App::FrameExtractorCb, config.input_file, config.read_chunk_size));

    ThreadGuard writer_guard(std::thread(App::WriterCb, config));
}
}  // namespace hdlc_parser