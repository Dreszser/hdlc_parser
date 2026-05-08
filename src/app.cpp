#include <hdlc_parser/app.hpp>
#include <hdlc_parser/queue.hpp>
#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/utility/crc_calculator.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>
#include <hdlc_parser/writer/sig/writer.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

namespace hdlc_parser {

void reader_cb(const std::string& filename, size_t chunk_size) {
    Reader reader(chunk_size);
    reader.read(filename.data());
}

void writer_cb(const Config& cfg) {
    std::vector<std::unique_ptr<Writer>> writers;
    switch (cfg.output_format) {
        case Config::OutputFormat::PCAP: {
            auto writer =
                std::make_unique<PcapWriter>(cfg.output_file_pcap.data());
            if (!writer->is_open()) {
                std::cout << "Failed to open PCAP output file: "
                          << cfg.output_file_pcap << "\n";
                return;
            }
            writers.emplace_back(std::move(writer));
            break;
        }
        case Config::OutputFormat::SIG: {
            auto writer =
                std::make_unique<SigWriter>(cfg.output_file_sig.data());
            if (!writer->is_open()) {
                std::cout << "Failed to open SIG output file: "
                          << cfg.output_file_sig << "\n";
                return;
            }
            writers.emplace_back(std::move(writer));

            break;
        }
        case Config::OutputFormat::BOTH: {
            auto pcap_writer =
                std::make_unique<PcapWriter>(cfg.output_file_pcap.data());
            if (!pcap_writer->is_open()) {
                std::cout << "Failed to open PCAP output file: "
                          << cfg.output_file_pcap << "\n";
                return;
            }
            writers.emplace_back(std::move(pcap_writer));

            auto sig_writer =
                std::make_unique<SigWriter>(cfg.output_file_sig.data());
            if (!sig_writer->is_open()) {
                std::cout << "Failed to open SIG output file: "
                          << cfg.output_file_sig << "\n";
                return;
            }
            writers.emplace_back(std::move(sig_writer));
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
    std::thread reader_thread(reader_cb, config.input_file,
                              config.read_chunk_size);

    std::thread writer_thread(writer_cb, config);

    reader_thread.join();
    writer_thread.join();
}
}  // namespace hdlc_parser