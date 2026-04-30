#include <hdlc_parser/app.hpp>
#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>
#include <hdlc_parser/writer/sig/writer.hpp>
#include <thread>

namespace hdlc_parser {

void App::start() {
    Reader reader(1024);
    auto result = reader.read("hdlc_test.bit");
}
}  // namespace hdlc_parser