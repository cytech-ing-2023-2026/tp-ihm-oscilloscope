/**
 * @file AppSensor.hpp
 * @brief Declares the data container used to represent a single sensor.
 */

#pragma once

#include <QColor>
#include <QContiguousCache>

/**
 * @struct AppSensor
 * @brief Stores state and history for one sensor stream.
 */
struct AppSensor {
    /** @brief Unique sensor identifier received from the device. */
    uint8_t id;
    /** @brief Display color used to render this sensor curve. */
    QColor color;
    /** @brief Most recent value received over UDP. */
    float value;
    /** @brief Rolling history used by the oscilloscope widget. */
    QContiguousCache<float> history;

    /**
     * @brief Pushes the current value into the history buffer.
     *
     * @details The cache indexes are normalized after insertion so the
     * implementation can iterate history using contiguous indexes.
     */
    void sample();
};