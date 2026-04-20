#pragma once

#include <QObject>

#include "AppSensorList.hpp"

class AppState : public QObject {
    Q_OBJECT

    // Expose model to QML
    Q_PROPERTY(AppSensorList* sensorsModel READ sensorsModel CONSTANT)

public:
    explicit AppState(QObject* parent = nullptr);

    AppSensorList* sensorsModel();

    void addSensor(uint8_t id, const QColor& color);
    void setSensorValue(uint8_t id, float value);

private:
    AppSensorList sensors_;
};