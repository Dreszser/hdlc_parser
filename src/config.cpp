#include <hdlc_parser/config.hpp>

namespace hdlc_parser {

size_t parse_size(const std::string& s) {
    size_t num = 0;
    size_t i = 0;

    while (i < s.size() && isdigit(s[i])) {
        num = num * 10 + (s[i] - '0');
        ++i;
    }

    std::string suffix = s.substr(i);

    if (suffix == "kb") {
        return num;
    }
    if (suffix == "mb") {
        return num * 1024;
    }

    return num;
}

Config::OutputFormat parse_format(const std::string& s) {
    if (s == "sig") {
        return Config::OutputFormat::SIG;
    } else if (s == "pcap") {
        return Config::OutputFormat::PCAP;
    } else if (s == "both") {
        return Config::OutputFormat::BOTH;
    }

    throw std::runtime_error("Invalid output_format: " + s);
}

const char* to_string(Config::OutputFormat f) {
    switch (f) {
        case Config::OutputFormat::SIG:
            return "sig";
        case Config::OutputFormat::PCAP:
            return "pcap";
        case Config::OutputFormat::BOTH:
            return "both";
    }
    return "unknown";
}

static void get_value(int argc, char** argv, int i, const std::string& arg,
                      std::string& out) {
    if (i + 1 >= argc) {
        throw std::runtime_error("Missing value for " + arg);
    }
    out = argv[++i];
}

Config parse_args(int argc, char** argv) {
    Config cfg{};

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--input-file") {
            get_value(argc, argv, i, arg, cfg.input_file);
        } else if (arg == "--output-file") {
            get_value(argc, argv, i, arg, cfg.output_file);
        } else if (arg == "--output_format") {
            std::string val;
            get_value(argc, argv, i, arg, val);
            cfg.output_format = parse_format(val);
        } else if (arg == "--read-chunk-size") {
            std::string val;
            get_value(argc, argv, i, arg, val);
            cfg.read_chunk_size = parse_size(val);
        } else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }

    return cfg;
}

}  // namespace hdlc_parser