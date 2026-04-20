#include "AppSensor.hpp"

void AppSensor::sample() {
  history.append(value);
  history.normalizeIndexes();
}