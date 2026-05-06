#include <hdlc_parser/app.hpp>
#include <hdlc_parser/reader.hpp>
#include <hdlc_parser/writer/pcap/writer.hpp>
#include <hdlc_parser/writer/sig/writer.hpp>
#include <hdlc_parser/utility/crc_calculator.hpp>
#include <thread>

namespace hdlc_parser {

void App::start() {
    CRCalculator::init();

    Reader reader(1024);
    reader.read("hdlc_test.bit");
}
}  // namespace hdlc_parser