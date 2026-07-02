#pragma once

template <typename T> class ISensor {
public:
  /**
   * Initializes the sensor.
   * @return true  If initialization was successful
   * @return false Otherwise
   */
  [[nodiscard]] virtual bool init() = 0;

  /**
   * Deinitializes the sensor.
   * Prepares it for deep sleep.
   */
  virtual void deinit() = 0;

  /**
   * Reads sensor value into out.
   * @param out Reference to store the sensor reading
   * @return true  If reading was successful
   * @return false Otherwise
   */
  [[nodiscard]] virtual bool read(T &out) = 0;

  virtual ~ISensor() = default;
};