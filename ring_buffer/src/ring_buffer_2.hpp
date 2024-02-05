#ifndef INCLUDE_RING_BUFFER2
#define INCLUDE_RING_BUFFER2

#include <atomic>
#include <stddef.h>
#include <stdint.h>
#include <vector>

class RingBuffer2
{
public:
  explicit RingBuffer2(size_t size)
    : buffer_(size)
  {
  }

  // Returns true on success. Fails if the buffer is full.
  bool enqueue(int item)
  {
    uint64_t write_idx = write_idx_.load(std::memory_order_relaxed);
    uint64_t read_idx = read_idx_.load(std::memory_order_acquire);
    if (write_idx - read_idx == buffer_.size()) {
      return false;
    }
    buffer_[write_idx & (buffer_.size() - 1)] = item;
    write_idx_.store(write_idx + 1, std::memory_order_release);
    return true;
  }

  // Returns true on success. Fails if the buffer is empty.
  bool dequeue(int* dest)
  {
    uint64_t read_idx = read_idx_.load(std::memory_order_relaxed);
    uint64_t write_idx = write_idx_.load(std::memory_order_acquire);
    if (write_idx == read_idx) {
      return false;
    }
    *dest = buffer_[read_idx & (buffer_.size() - 1)];
    read_idx_.store(read_idx + 1, std::memory_order_release);
    return true;
  }

private:
  std::vector<int> buffer_;
  std::atomic<uint64_t> read_idx_{ 0 };
  std::atomic<uint64_t> write_idx_{ 0 };
};
#endif
