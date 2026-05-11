#include <hdlc_parser/queue.hpp>

namespace hdlc_parser {
void FrameQueue::push(frames_t&& frames) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(frames));
    }
    cv_.notify_one();
}

bool FrameQueue::pop(frames_t& frames) {
    std::unique_lock<std::mutex> lock(mutex_);

    cv_.wait(lock, [this] { return !queue_.empty() || done_; });

    if (queue_.empty()) {
        return false;
    }
    frames = std::move(queue_.front());
    queue_.pop();
    return true;
}

void FrameQueue::notify_done() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        done_ = true;
    }
    cv_.notify_one();
}

}  // namespace hdlc_parser