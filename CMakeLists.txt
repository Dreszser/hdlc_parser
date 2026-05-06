cmake_minimum_required(VERSION 4.1)
project(hdlc_parser)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -std=c++20")

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/app.cpp
        src/bit_reader.cpp
        src/reader.cpp
        src/writer/sig/writer.cpp
        src/writer/pcap/writer.cpp
        src/utility/crc_calculator.cpp
        src/queue.cpp
        src/config.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)