#pragma once

#include <QColor>
#include <QContiguousCache>

struct AppSensor {
    uint8_t id;
    QColor color;
    float value;
    QContiguousCache<float> history;

    void sample();
};