#ifndef INCLUDE_RING_BUFFER
#define INCLUDE_RING_BUFFER

#include <stddef.h>
#include <stdint.h>
#include <vector>

class RingBuffer
{
public:
  explicit RingBuffer(size_t size)
    : buffer_(size)
  {
  }

  // Returns true on success. Fails if the buffer is full.
  bool enqueue(int item)
  {
    if (write_idx_ - read_idx_ == buffer_.size()) {
      return false;
    }
    buffer_[write_idx_ % buffer_.size()] = item;
    write_idx_++;
    return true;
  }

  // Returns true on success. Fails if the buffer is empty.
  bool dequeue(int* dest)
  {
    if (write_idx_ == read_idx_) {
      return false;
    }
    *dest = buffer_[read_idx_ % buffer_.size()];
    read_idx_++;
    return true;
  }

private:
  std::vector<int> buffer_;
  uint64_t read_idx_{ 0 };
  uint64_t write_idx_{ 0 };
};
#endif
