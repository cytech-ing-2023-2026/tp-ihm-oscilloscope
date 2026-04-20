/**
 * @file OscilloscopeItem.cpp
 * @brief Implements rendering and sampling behavior for OscilloscopeItem.
 */

#include "OscilloscopeItem.hpp"

#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QPainterPath>

/** @copydoc OscilloscopeItem::OscilloscopeItem(QQuickItem*) */
OscilloscopeItem::OscilloscopeItem(QQuickItem* parent) : QQuickPaintedItem(parent) {
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
    setAntialiasing(true);

    connect(&timer_, &QTimer::timeout, this, &OscilloscopeItem::tick);

    timer_.start(1000 / 30.0);
}

/** @copydoc OscilloscopeItem::model */
AppSensorList* OscilloscopeItem::model() const { return model_; }

/** @copydoc OscilloscopeItem::setModel */
void OscilloscopeItem::setModel(AppSensorList* model) {
    if (model_ == model) return;

    model_ = model;
    emit modelChanged();
    update();
}

/** @copydoc OscilloscopeItem::tick */
void OscilloscopeItem::tick() {
    if (model_) {
        auto& sensors = model_->sensors();

        for (auto& sensor : sensors) {
            sensor.sample();
        }
    }

    update();  // triggers repaint
}

/** @copydoc OscilloscopeItem::paint */
void OscilloscopeItem::paint(QPainter* painter) {
    if (!model_)
        return;

    // Background
    painter->fillRect(boundingRect(), QColor(10, 10, 10));

    const auto& sensors = model_->sensors();

    const float w = width();
    const float h = height();

    // Grid + labels
    const QPen linePen = QPen(QColor(50, 50, 50), 1);
    const QPen importantLinePen = QPen(QColor(100, 100, 100), 1);

    const int gridX = 10;
    const int gridY = 8;
    const int rowsTo2 = 4;
    const float diffByRow = h * 0.4 / rowsTo2;
    const int rowsToEdge = h * 0.5 / diffByRow;

    painter->setPen(linePen);

    for (int i = 1; i < gridX; ++i) {
        float x = (w / gridX) * i;
        painter->drawLine(QPointF(x, 0), QPointF(x, h));
    }

    for (int i = 1; i < rowsTo2; ++i) {
        float absDiff = diffByRow * i;
        float posY = h * 0.5 - absDiff;
        float negY = h * 0.5 + absDiff;
        painter->drawLine(QPointF(0, posY), QPointF(w, posY));
        painter->drawLine(QPointF(0, negY), QPointF(w, negY));
    }

    painter->setPen(importantLinePen);
    painter->drawLine(QPointF(0, h * 0.5), QPointF(w, h * 0.5));
    painter->drawLine(QPointF(0, h * 0.1), QPointF(w, h * 0.1));
    painter->drawLine(QPointF(0, h * 0.9), QPointF(w, h * 0.9));

    float padding = 10;

    painter->drawText(padding, h * 0.1 - padding, "1");
    painter->drawText(padding, h * 0.5 - padding, "0");
    painter->drawText(padding, h * 0.9 - padding, "-1");

    // Data
    for (const auto& sensor : sensors) {

        const auto& hist = sensor.history;
        const int n = hist.size();

        if (n < 2)
            continue;

        QPen pen(sensor.color);
        pen.setWidth(2);
        painter->setPen(pen);

        QPainterPath path;

        int xi = 0;

        for (int i = sensor.history.lastIndex(); i >= sensor.history.firstIndex(); i--) {
            float x = xi / 59.0 * w;
            float y = h * 0.5f - hist[i] * (h * 0.4f / 2.0f);

            if (xi == 0)
                path.moveTo(x, y);
            else
                path.lineTo(x, y);

            xi++;
        }

        painter->drawPath(path);
    }
}