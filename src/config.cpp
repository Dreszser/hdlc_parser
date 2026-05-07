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

    if (suffix == "b") {
        return num;
    }
    if (suffix == "kb") {
        return num * 1024;
    }

    return num;
}

const char* to_string(Config::OutputFormat f) {
    switch (f) {
        case Config::OutputFormat::NONE:
            return "none";
        case Config::OutputFormat::SIG:
            return "sig";
        case Config::OutputFormat::PCAP:
            return "pcap";
        case Config::OutputFormat::BOTH:
            return "both";
    }
    return "unknown";
}

static void get_value(int argc, char** argv, int& i, const std::string& arg,
                      std::string& out) {
    if (i + 1 >= argc) {
        throw std::runtime_error("Missing value for " + arg);
    }
    out = argv[++i];
}

Config::OutputFormat compute_format(const Config& cfg) {
    bool sig = !cfg.output_file_sig.empty();
    bool pcap = !cfg.output_file_pcap.empty();

    if (sig && pcap) {
        return Config::OutputFormat::BOTH;
    }
    if (sig) {
        return Config::OutputFormat::SIG;
    }
    if (pcap) {
        return Config::OutputFormat::PCAP;
    }
    return Config::OutputFormat::NONE;
}

Config parse_args(int argc, char** argv) {
    Config cfg;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--input-file") {
            get_value(argc, argv, i, arg, cfg.input_file);
        } else if (arg == "--output-sig") {
            if (cfg.output_file_sig.empty()) {
                get_value(argc, argv, i, arg, cfg.output_file_sig);
            } else {
                throw std::runtime_error("SIG output file already declared");
            }
        } else if (arg == "--output-pcap") {
            if (cfg.output_file_pcap.empty()) {
                get_value(argc, argv, i, arg, cfg.output_file_pcap);
            } else {
                throw std::runtime_error("PCAP output file already declared");
            }
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