/**
 * @file OscilloscopeItem.hpp
 * @brief Declares the custom QQuickPaintedItem used to render sensor curves.
 */

#pragma once

#include <QQuickPaintedItem>
#include <QTimer>
#include "AppSensorList.hpp"

/**
 * @class OscilloscopeItem
 * @brief Custom painted item that draws scrolling sensor waveforms.
 */
class OscilloscopeItem : public QQuickPaintedItem {
    Q_OBJECT

    /**
     * @brief Sensor model consumed by this visual component.
     */
    Q_PROPERTY(AppSensorList* model READ model WRITE setModel NOTIFY modelChanged)
public:
    /**
     * @brief Constructs the oscilloscope item.
     * @param parent Optional QQuickItem parent.
     */
    explicit OscilloscopeItem(QQuickItem* parent = nullptr);

    /**
     * @brief Returns the currently assigned model.
     * @return Pointer to the sensor model.
     */
    AppSensorList* model() const;

    /**
     * @brief Assigns the model used for rendering.
     * @param model Pointer to the sensor model.
     */
    void setModel(AppSensorList* model);

    /**
     * @brief Paints the oscilloscope grid and curves.
     * @param painter Active painter supplied by Qt.
     */
    void paint(QPainter* painter) override;

    /**
     * @brief Samples current sensor values and schedules a repaint.
     */
    void tick();

signals:
    /**
     * @brief Emitted when the model property changes.
     */
    void modelChanged();

private:
    /** @brief Model pointer set from QML. */
    AppSensorList* model_ = nullptr;
    /** @brief Timer driving periodic sampling and redraw. */
    QTimer timer_; 
};