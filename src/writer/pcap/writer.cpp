#include <chrono>
#include <fstream>
#include <hdlc_parser/writer/pcap/header.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>

namespace hdlc_parser {

PcapWriter::PcapWriter(const char* filename)
    : file_(filename, std::ios::binary) {
    WriteglobalHeader();
}

void PcapWriter::WriteglobalHeader() {
    PcapGlobalHeader header;
    file_.write(reinterpret_cast<const char*>(&header), sizeof(header));
}

void PcapWriter::WriteFrame(const frame_t& frame) {
    auto now = std::chrono::system_clock::now();

    PcapPacketHeader ph{};
    ph.ts_sec = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
            .count());
    ph.ts_usec = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch())
            .count() %
        1000000);
    ph.incl_len = frame.size() - 2;
    ph.orig_len = frame.size() - 2;

    file_.write(reinterpret_cast<const char*>(&ph), sizeof(ph));
    file_.write(reinterpret_cast<const char*>(frame.data()), frame.size() - 2);
}

void PcapWriter::write(const frames_t& frames) {
    for (const auto& frame : frames) {
        WriteFrame(frame);
    }
}

bool PcapWriter::is_open() const { return file_.is_open(); }
}  // namespace hdlc_parser