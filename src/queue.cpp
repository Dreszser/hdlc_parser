#include <hdlc_parser/queue.hpp>

namespace hdlc_parser {
void FrameQueue::push(frames_t&& frame) {
  std::lock_guard<std::mutex> lock(mutex_);
  queue_.push(std::move(frame));
}

frames_t FrameQueue::pop() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.empty()) {
    return {};
  }
  frames_t frame = queue_.front();
  queue_.pop();
  return frame;
}

}  // namespace hdlc_parser