/**
 * @file AppSensorList.cpp
 * @brief Implements the AppSensorList Qt model.
 */

#include "AppSensorList.hpp"

/** @copydoc AppSensorList::AppSensorList(QObject*) */
AppSensorList::AppSensorList(QObject* parent)
    : QAbstractListModel(parent) {}

/** @copydoc AppSensorList::rowCount */
int AppSensorList::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;

    return sensors_.size();
}

/** @copydoc AppSensorList::data */
QVariant AppSensorList::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= sensors_.size())
        return {};

    const AppSensor& sensor = sensors_.at(index.row());

    switch (role) {
    case IdRole:
        return sensor.id;
    case ColorRole:
        return sensor.color;
    case ValueRole:
        return sensor.value;
    default:
        return {};
    }
}

/** @copydoc AppSensorList::roleNames */
QHash<int, QByteArray> AppSensorList::roleNames() const {
    return {
        { IdRole, "sensorId" },
        { ColorRole, "color" },
        { ValueRole, "value" }
    };
}

/** @copydoc AppSensorList::sensors() */
QVector<AppSensor>& AppSensorList::sensors() {
    return sensors_;
}

/** @copydoc AppSensorList::sensors() const */
const QVector<AppSensor>& AppSensorList::sensors() const {
    return sensors_;
}

/** @copydoc AppSensorList::addSensor */
void AppSensorList::addSensor(uint8_t id, const QColor& color) {
    // prevent duplicates (linear search as requested)
    for (const auto& s : sensors_) {
        if (s.id == id)
            return;
    }

    const int row = sensors_.size();

    beginInsertRows(QModelIndex(), row, row);
    sensors_.push_back({ id, color, 0.0f, QContiguousCache<float>(60)});
    endInsertRows();
}

/** @copydoc AppSensorList::setSensorValue */
void AppSensorList::setSensorValue(uint8_t id, float value) {
    for (int row = 0; row < sensors_.size(); ++row) {
        if (sensors_[row].id == id) {
            sensors_[row].value = value;

            QModelIndex idx = createIndex(row, 0);
            emit dataChanged(idx, idx, { ValueRole });
            return;
        }
    }
}