#include "AppSensorList.hpp"

AppSensorList::AppSensorList(QObject* parent)
    : QAbstractListModel(parent) {}

int AppSensorList::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;

    return sensors_.size();
}

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

QHash<int, QByteArray> AppSensorList::roleNames() const {
    return {
        { IdRole, "sensorId" },
        { ColorRole, "color" },
        { ValueRole, "value" }
    };
}

QVector<AppSensor>& AppSensorList::sensors() {
    return sensors_;
}

const QVector<AppSensor>& AppSensorList::sensors() const {
    return sensors_;
}

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