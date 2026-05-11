#pragma once
#include <condition_variable>
#include <hdlc_parser/singleton.hpp>
#include <hdlc_parser/types.hpp>
#include <mutex>
#include <queue>

namespace hdlc_parser {

class FrameQueue : public Singleton<FrameQueue> {
   public:
    void push(frames_t&& frames);

    bool pop(frames_t& frames);
    void notify_done();

   private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<frames_t> queue_;
    bool done_ = false;
};
}  // namespace hdlc_parser