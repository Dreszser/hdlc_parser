#include <chrono>
#include <hdlc_parser/app.hpp>
#include <iostream>
#include <stdexcept>

using namespace std::chrono;

int main(int argc, char** argv) {
    hdlc_parser::Config cfg;
    try {
        cfg = hdlc_parser::parse_args(argc, argv);

        cfg.output_format = hdlc_parser::compute_format(cfg);
        if (cfg.output_format == hdlc_parser::Config::OutputFormat::NONE) {
            throw std::runtime_error(
                "No output format specified. Use --output-sig and/or "
                "--output-pcap");
        }

        if (cfg.input_file.empty()) {
            cfg.input_file = "input.bin";
        }
        if (cfg.read_chunk_size == 0) {
            cfg.read_chunk_size = 1024;
        }

        std::cout << "----- Parser configuration ------\n\n";
        std::cout << "input: " << cfg.input_file << "\n";
        std::cout << "format: " << to_string(cfg.output_format) << "\n";
        std::cout << "output_sig: "
                  << (cfg.output_file_sig.empty() ? "(none)"
                                                  : cfg.output_file_sig)
                  << "\n";
        std::cout << "output_pcap: "
                  << (cfg.output_file_pcap.empty() ? "(none)"
                                                   : cfg.output_file_pcap)
                  << "\n";
        std::cout << "chunk: " << cfg.read_chunk_size << "b\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    hdlc_parser::App app;

    std::cout << "\n----- Execution started ---------\n" << std::endl;
    auto start = high_resolution_clock::now();
    app.Start(cfg);
    auto stop = high_resolution_clock::now();
    std::cout << "\n\n----- Execution ended -----------" << std::endl;

    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "\n\nExecution time: " << std::dec << duration.count()
              << "ms\n"
              << std::endl;

    return 0;
}