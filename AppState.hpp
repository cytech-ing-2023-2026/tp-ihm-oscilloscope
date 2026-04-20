/**
 * @file AppState.hpp
 * @brief Declares the application state object exposed to QML.
 */

#pragma once

#include <QObject>

#include "AppSensorList.hpp"

/**
 * @class AppState
 * @brief Owns and exposes shared state used by the UI.
 */
class AppState : public QObject {
    Q_OBJECT

    /**
     * @brief Sensor model exposed as a read-only QML property.
     */
    Q_PROPERTY(AppSensorList* sensorsModel READ sensorsModel CONSTANT)

public:
    /**
     * @brief Creates the application state container.
     * @param parent Optional QObject parent.
     */
    explicit AppState(QObject* parent = nullptr);

    /**
     * @brief Returns the sensor model instance.
     * @return Pointer to the owned sensor model.
     */
    AppSensorList* sensorsModel();

    /**
     * @brief Adds a new sensor to the shared model.
     * @param id Sensor identifier.
     * @param color Sensor display color.
     */
    void addSensor(uint8_t id, const QColor& color);

    /**
     * @brief Updates a sensor value in the shared model.
     * @param id Sensor identifier.
     * @param value New value.
     */
    void setSensorValue(uint8_t id, float value);

private:
    /** @brief Sensor model owned by the application state. */
    AppSensorList sensors_;
};