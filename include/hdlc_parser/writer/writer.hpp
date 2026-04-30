#pragma once
#include <hdlc_parser/types.hpp>

namespace hdlc_parser {
class Writer {
   public:
    virtual void write(const frames_t& frames) = 0;
};
}  // namespace hdlc_parser