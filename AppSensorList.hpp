/**
 * @file AppSensorList.hpp
 * @brief Declares the list model exposing sensors to Qt views and QML.
 */

#pragma once

#include <QAbstractListModel>
#include <QColor>
#include <QVector>

#include "AppSensor.hpp"

/**
 * @class AppSensorList
 * @brief List model storing sensors and exposing their fields through roles.
 */
class AppSensorList : public QAbstractListModel {
    Q_OBJECT

public:
    /**
     * @enum Roles
     * @brief Custom item roles exposed by the model.
     */
    enum Roles {
        /** @brief Sensor identifier role. */
        IdRole = Qt::UserRole + 1,
        /** @brief Sensor display color role. */
        ColorRole,
        /** @brief Current sensor value role. */
        ValueRole
    };

    /**
     * @brief Builds an empty sensor model.
     * @param parent Optional QObject parent.
     */
    explicit AppSensorList(QObject* parent = nullptr);

    /**
     * @brief Returns the number of model rows.
     * @param parent Parent index used by Qt's model API.
     * @return Number of top-level rows.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    /**
     * @brief Returns data for one model index and role.
     * @param index Item index in the model.
     * @param role Role identifier to resolve.
     * @return Value mapped to the requested role, or an invalid QVariant.
     */
    QVariant data(const QModelIndex& index, int role) const override;

    /**
     * @brief Provides the role-name mapping used by QML.
     * @return Hash mapping role integers to role names.
     */
    QHash<int, QByteArray> roleNames() const override;
    
    /**
     * @brief Returns mutable access to the backing sensor container.
     * @return Reference to the internal sensor vector.
     */
    QVector<AppSensor>& sensors();

    /**
     * @brief Returns read-only access to the backing sensor container.
     * @return Const reference to the internal sensor vector.
     */
    const QVector<AppSensor>& sensors() const;

    /**
     * @brief Adds a sensor if its identifier is not already present.
     * @param id Sensor identifier.
     * @param color Sensor display color.
     */
    Q_INVOKABLE void addSensor(uint8_t id, const QColor& color);

    /**
     * @brief Updates the current value of an existing sensor.
     * @param id Sensor identifier.
     * @param value New sample value.
     */
    Q_INVOKABLE void setSensorValue(uint8_t id, float value);

private:
    /** @brief Backing storage used by the list model. */
    QVector<AppSensor> sensors_;
};