#ifndef INCLUDE_RING_BUFFER
#define INCLUDE_RING_BUFFER

#include <optional>
#include <stddef.h>
#include <stdint.h>
#include <vector>
namespace ring_buffer {
template<typename T>
class SimpleRingBuffer
{
public:
  explicit SimpleRingBuffer(size_t buffer_size)
    : buffer_(buffer_size)
  {
  }

  bool enqueue(T item)
  {
    if (is_full()) {
      return false;
    }
    buffer_[true_idx(write_idx_)] = item;
    move_next(write_idx);
    return true;
  }

  std::optional<T> dequeue()
  {
    if (is_empty()) {
      return std::nullopt;
    }
    auto v = buffer_[true_idx(read_idx_)];
    move_next(read_idx);
    return v;
  }

private:
  std::vector<T> buffer_;
  uint64_t read_idx_{ 0 };
  uint64_t write_idx_{ 0 };

  bool is_full() { return write_idx_ - read_idx_ == buffer_.size(); }
  bool is_empty() { return write_idx_ == read_idx_; }

  bool true_idx(const uint64_t idx) const { return idx % buffer_.size(); }

  void move_next(int& idx) { idx++; }
};
}
#endif
