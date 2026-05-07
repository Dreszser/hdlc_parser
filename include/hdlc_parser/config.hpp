#pragma once

#include <string>

namespace hdlc_parser {

struct Config {
    std::string input_file;
    std::string output_file_sig;
    std::string output_file_pcap;

    enum class OutputFormat { NONE, SIG, PCAP, BOTH } output_format;

    uint32_t read_chunk_size = 0;
};

size_t parse_size(const std::string& s);

Config::OutputFormat compute_format(const Config& cfg);

const char* to_string(Config::OutputFormat f);

Config parse_args(int argc, char** argv);

}  // namespace hdlc_parser