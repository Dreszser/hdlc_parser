#include <chrono>
#include <hdlc_parser/app.hpp>
#include <iostream>
#include <stdexcept>

using namespace std::chrono;

int main(int argc, char** argv) {
    hdlc_parser::Config cfg;
    try {
        cfg = hdlc_parser::parse_args(argc, argv);

        if (cfg.input_file.empty()) {
            cfg.input_file = "input.bin";
        }
        if (cfg.output_file.empty()) {
            cfg.output_file = "output.bin";
        }
        if (cfg.read_chunk_size == 0) {
            cfg.read_chunk_size = 1024;
        }

        std::cout << "------- Parser configuration -------\n\n";
        std::cout << "input: " << cfg.input_file << "\n";
        std::cout << "output: " << cfg.output_file << "\n";
        std::cout << "format: " << to_string(cfg.output_format) << "\n";
        std::cout << "chunk: " << cfg.read_chunk_size << "kb\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    hdlc_parser::App app;

    std::cout << "\n------- Execution started -------\n" << std::endl;
    auto start = high_resolution_clock::now();
    app.start(cfg);
    auto stop = high_resolution_clock::now();
    std::cout << "\n\n------- Execution ended -------" << std::endl;

    auto duration = duration_cast<milliseconds>(stop - start);

    std::cout << "\n\nExecution time: " << std::dec << duration.count() << "ms\n"
              << std::endl;

    return 0;
}