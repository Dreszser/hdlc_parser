#pragma once
#include <vector>

namespace hdlc_parser {
using frame_t = std::vector<uint8_t>;
using frames_t = std::vector<frame_t>;

}  // namespace hdlc_parser