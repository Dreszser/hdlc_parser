#include <hdlc_parser/app.hpp>
#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>
#include <hdlc_parser/writer/sig/writer.hpp>
#include <thread>

namespace hdlc_parser {

void reader_cb(const char* filename, uint32_t chunk_size) {
    Reader reader(chunk_size);
    reader.read(filename);
}

void writer_cb(const char* filename) { SigWriter sig_writer(filename); }

void App::start() {
    /* std::jthread reader_thread(reader_cb, "hdlc_test.bit", 1024);
    std::jthread writer_thread(writer_cb, "output.sig"); */
}
}  // namespace hdlc_parser