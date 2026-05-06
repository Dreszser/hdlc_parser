#include <hdlc_parser/queue.hpp>

namespace hdlc_parser {
void FrameQueue::push(frames_t&& frames) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(frames));
}

bool FrameQueue::pop(frames_t& frames) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return false;
    }
    frames = std::move(queue_.front());
    queue_.pop();
    return true;
}

}  // namespace hdlc_parser