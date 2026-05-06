#pragma once

#include <string>

namespace hdlc_parser {

struct Config {
    std::string input_file;
    std::string output_file;

    enum class OutputFormat { SIG, PCAP, BOTH } output_format;

    uint32_t read_chunk_size = 0;
};

size_t parse_size(const std::string& s);

Config::OutputFormat parse_format(const std::string& s);

const char* to_string(Config::OutputFormat f);

Config parse_args(int argc, char** argv);

}  // namespace hdlc_parser