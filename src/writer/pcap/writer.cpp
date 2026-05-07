#include <chrono>
#include <fstream>
#include <hdlc_parser/writer/pcap/header.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>

namespace hdlc_parser {

PcapWriter::PcapWriter(const char* filename)
    : file_(filename, std::ios::binary) {
    write_global_header();
}

void PcapWriter::write_global_header() {
    PcapGlobalHeader header;
    file_.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

void PcapWriter::write_frame(const frame_t& frame) {
    auto now = std::chrono::system_clock::now();
    auto sec = std::chrono::time_point_cast<std::chrono::seconds>(now);
    auto usec =
        std::chrono::duration_cast<std::chrono::microseconds>(now - sec);

    PcapPacketHeader ph{};
    ph.ts_sec = sec.time_since_epoch().count();
    ph.ts_usec = usec.count();
    ph.incl_len = frame.size();
    ph.orig_len = frame.size();

    file_.write(reinterpret_cast<char*>(&ph), sizeof(ph));
    file_.write(reinterpret_cast<const char*>(frame.data()), frame.size());
}

void PcapWriter::write(const frames_t& frames) {
    for (const auto& frame : frames) {
        write_frame(frame);
    }
}

bool PcapWriter::is_open() const { return file_.is_open(); }
}  // namespace hdlc_parser