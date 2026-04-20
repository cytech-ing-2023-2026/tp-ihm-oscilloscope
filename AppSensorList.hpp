#pragma once

#include <QAbstractListModel>
#include <QColor>
#include <QVector>

#include "AppSensor.hpp"

class AppSensorList : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        ColorRole,
        ValueRole
    };

    explicit AppSensorList(QObject* parent = nullptr);

    // QAbstractListModel overrides
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    
    QVector<AppSensor>& sensors();
    const QVector<AppSensor>& sensors() const;

    // API
    Q_INVOKABLE void addSensor(uint8_t id, const QColor& color);
    Q_INVOKABLE void setSensorValue(uint8_t id, float value);

private:
    QVector<AppSensor> sensors_;
};