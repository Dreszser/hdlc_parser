#include <hdlc_parser/singleton.hpp>
#include <hdlc_parser/types.hpp>
#include <mutex>
#include <queue>

namespace hdlc_parser {

class FrameQueue : public Singleton<FrameQueue> {
 public:
  void push(frames_t&& frame);

  frames_t pop();

 private:
  std::mutex mutex_;
  std::queue<frames_t> queue_;
};
}  // namespace hdlc_parser