#pragma once

namespace Lock {

struct NoLock {
  void acquire() {}
  void release() {}
};

template <typename T> struct Guard {
  T &lock;

  Guard(T &l) : lock(l) { lock.acquire(); }
  ~Guard() { lock.release(); }
};

} // namespace Lock