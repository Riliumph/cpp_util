#ifndef INCLUDE_RING_BUFFER2
#define INCLUDE_RING_BUFFER2

#include <atomic>
#include <stddef.h>
#include <stdint.h>
#include <vector>
namespace ring_buffer {

template<typename T>
class CustomRingBuffer
{
public:
  /// @brief コンストラクタ
  /// @param buffer_size 2のべき乗である必要がある
  explicit CustomRingBuffer(size_t buffer_size)
    : buffer_(buffer_size)
  {
  }

  bool enqueue(T item)
  {
    auto write_idx = write_idx_.load(std::memory_order_relaxed);
    auto read_idx = read_idx_.load(std::memory_order_acquire);
    if (is_full(write_idx, read_idx)) {
      return false;
    }
    buffer_[true_idx(write_idx)] = item;
    move_next_write_idx(write_idx);
    return true;
  }

  bool dequeue(T* dest)
  {
    auto read_idx = read_idx_.load(std::memory_order_relaxed);
    auto write_idx = write_idx_.load(std::memory_order_acquire);
    if (is_empty(write_idx, read_idx)) {
      return false;
    }
    *dest = buffer_[true_idx(read_idx)];
    move_next_read_idx(read_idx);
    return true;
  }

private:
  std::vector<T> buffer_;
  std::atomic<uint64_t> read_idx_{ 0 };
  std::atomic<uint64_t> write_idx_{ 0 };

  bool is_full(const uint64_t write_idx, const uint64_t read_idx) const
  {
    write_idx - read_idx == buffer_.size();
  }

  bool is_empty(const uint64_t write_idx, const uint64_t read_idx) const
  {
    return write_idx == read_idx;
  }

  bool true_idx(const uint64_t idx) const { return idx & (buffer_.size() - 1); }

  void move_next_read_idx(const uint64_t read_idx)
  {
    read_idx_.store(read_idx + 1, std::memory_order_release);
  }

  void move_next_write_idx(const uint64_t write_idx)
  {
    write_idx_.store(write_idx + 1, std::memory_order_release);
  }
};
}
#endif
