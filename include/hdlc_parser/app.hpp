#pragma once
#include <hdlc_parser/config.hpp>
#include <hdlc_parser/writer/writer.hpp>
#include <memory>
#include <thread>
#include <vector>

namespace hdlc_parser {

class ThreadGuard {
   public:
    explicit ThreadGuard(std::thread&& t) : thread_(std::move(t)) {}

    ~ThreadGuard() {
        if (thread_.joinable()) {
            thread_.join();
        }
    }

    ThreadGuard(const ThreadGuard&) = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;

   private:
    std::thread thread_;
};

class App {
   public:
    void Start(const Config& pcfg);

   private:
    static std::vector<std::unique_ptr<Writer>> InitializeWriters(
        const Config& cfg);

    static void FrameExtractorCb(const std::string& input_file,
                                 size_t chunk_size);
    static void WriterCb(const Config& cfg);

    static bool AddWriter(std::vector<std::unique_ptr<Writer>>& writers,
                          std::unique_ptr<Writer> writer,
                          const std::string& filename);
};

}  // namespace hdlc_parser
