#pragma once
#include <hdlc_parser/config.hpp>
#include <hdlc_parser/writer/writer.hpp>
#include <vector>

namespace hdlc_parser {
class App {
   public:
    void Start(const Config& pcfg);

   private:
    static void FrameExtractorCb(const std::string& input_file,
                                 size_t chunk_size);
    static void WriterCb(const Config& cfg);

    static bool AddWriter(std::vector<std::unique_ptr<Writer>>& writers,
                          std::unique_ptr<Writer> writer,
                          const std::string& filename);
};
}  // namespace hdlc_parser
