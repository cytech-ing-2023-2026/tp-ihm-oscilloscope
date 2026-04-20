/**
 * @file AppState.cpp
 * @brief Implements the application state wrapper.
 */

#include "AppState.hpp"

/** @copydoc AppState::AppState(QObject*) */
AppState::AppState(QObject* parent) : QObject(parent) {}

/** @copydoc AppState::sensorsModel */
AppSensorList* AppState::sensorsModel() { return &sensors_; }

/** @copydoc AppState::addSensor */
void AppState::addSensor(uint8_t id, const QColor& color) { sensors_.addSensor(id, color); }

/** @copydoc AppState::setSensorValue */
void AppState::setSensorValue(uint8_t id, float value) { sensors_.setSensorValue(id, value); }