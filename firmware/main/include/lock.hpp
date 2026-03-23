#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace Lock {

struct NoLock {
  void acquire() {}
  void release() {}
};

struct FreeRTOSMutex {
  SemaphoreHandle_t handle;

  FreeRTOSMutex() { handle = xSemaphoreCreateMutex(); }
  ~FreeRTOSMutex() { vSemaphoreDelete(handle); }

  void acquire() { xSemaphoreTake(handle, portMAX_DELAY); }
  void release() { xSemaphoreGive(handle); }
};

template <typename T> struct Guard {
  T &lock;

  Guard(T &l) : lock(l) { lock.acquire(); }
  ~Guard() { lock.release(); }
};

} // namespace Lock