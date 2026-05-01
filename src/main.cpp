#include <chrono>
#include <hdlc_parser/app.hpp>
#include <iostream>

using namespace std::chrono;

int main() {
    auto start = high_resolution_clock::now();
    hdlc_parser::App app;
    app.start();
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    std::cout << duration.count() << std::endl;

    return 0;
}