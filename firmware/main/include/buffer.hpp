#pragma once

#include "lock.hpp"
#include <stddef.h>

template <typename T, size_t N, typename TLock = Lock::NoLock> class CircularBuffer {
  T data[N];
  size_t head = 0;
  size_t tail = 0;
  size_t count = 0;
  TLock lock;

public:
  /**
   * Pushes item into the buffer. If full, overwrites the oldest item.
   * @param item Item to push
   */
  void push(const T &item) {
    Lock::Guard<TLock> guard(lock);

    data[head] = item;
    head = (head + 1) % N;

    if (count == N) {
      tail = (tail + 1) % N;
    } else {
      count++;
    }
  }

  /**
   * Pops item from the buffer.
   * @param out Reference to store the popped item
   * @return true  If item was popped successfully
   * @return false If buffer is empty
   */
  [[nodiscard]] bool pop(T &out) {
    Lock::Guard<TLock> guard(lock);

    if (count == 0) return false;

    out = data[tail];
    tail = (tail + 1) % N;
    count--;

    return true;
  }

  /**
   * Peeks at the oldest item without removing it.
   * @param out Reference to store the peeked item
   * @return true  If item was peeked successfully
   * @return false If buffer is empty
   */
  [[nodiscard]] bool peek(T &out) {
    Lock::Guard<TLock> guard(lock);

    if (count == 0) return false;

    out = data[tail];

    return true;
  }

  [[nodiscard]] bool is_empty() {
    Lock::Guard<TLock> guard(lock);
    return count == 0;
  }

  [[nodiscard]] bool is_full() {
    Lock::Guard<TLock> guard(lock);
    return count == N;
  }

  [[nodiscard]] size_t size() {
    Lock::Guard<TLock> guard(lock);
    return count;
  }

  [[nodiscard]] size_t capacity() const { return N; }
};