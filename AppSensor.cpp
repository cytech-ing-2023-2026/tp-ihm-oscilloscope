/**
 * @file AppSensor.cpp
 * @brief Implements helper logic for AppSensor.
 */

#include "AppSensor.hpp"

/** @copydoc AppSensor::sample */
void AppSensor::sample() {
  history.append(value);
  history.normalizeIndexes();
}