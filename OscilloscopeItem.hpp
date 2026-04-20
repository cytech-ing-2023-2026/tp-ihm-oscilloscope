#pragma once

#include <QQuickPaintedItem>
#include <QTimer>
#include "AppSensorList.hpp"

class OscilloscopeItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(AppSensorList* model READ model WRITE setModel NOTIFY modelChanged)
public:
    explicit OscilloscopeItem(QQuickItem* parent = nullptr);

    AppSensorList* model() const;
    void setModel(AppSensorList* model);

    void paint(QPainter* painter) override;

    void tick();

signals:
    void modelChanged();

private:
    AppSensorList* model_ = nullptr;
    QTimer timer_; 
};