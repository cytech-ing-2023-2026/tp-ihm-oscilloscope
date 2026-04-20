#include "AppState.hpp"

AppState::AppState(QObject* parent) : QObject(parent) {}

AppSensorList* AppState::sensorsModel() { return &sensors_; }

void AppState::addSensor(uint8_t id, const QColor& color) { sensors_.addSensor(id, color); }

void AppState::setSensorValue(uint8_t id, float value) { sensors_.setSensorValue(id, value); }