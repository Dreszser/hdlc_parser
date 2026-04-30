#include <hdlc_parser/app.hpp>
#include <hdlc_parser/bit_reader.hpp>
#include <iostream>

int main() {
    /* hdlc_parser::App app;
    app.start();
    return 0; */
    hdlc_parser::BitReader br;
    br.append("\xF0", 1);
    br.readBit();
    br.append("\x0F", 1);
    uint8_t bits[15];
    for (int i = 0; i < 15; ++i) {
        bits[i] = br.readBit();
    }
    for (int i = 0; i < 15; ++i) {
        std::cout << static_cast<int>(bits[i]);
    }
    std::cout << "\n";
}