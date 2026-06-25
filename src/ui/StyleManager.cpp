#include "StyleManager.h"

#include <Qt>

QPen StyleManager::linePen(const QString& type)
{
    QPen pen;
    pen.setWidthF(1.5);
    pen.setColor(Qt::darkGray);

    if (type == "wall") {
        pen.setColor(Qt::black);
        pen.setWidthF(2.5);
    } else if (type == "border") {
        pen.setColor(Qt::gray);
        pen.setWidthF(1.5);
        pen.setStyle(Qt::DashLine);
    } else if (type == "pit") {
        pen.setColor(Qt::black);
        pen.setWidthF(2.0);
        pen.setStyle(Qt::DotLine);
    } else if (type == "floor-step") {
        pen.setColor(Qt::darkYellow);
        pen.setWidthF(1.8);
        pen.setStyle(Qt::DashDotLine);
    } else if (type == "rock-border") {
        pen.setColor(Qt::darkGray);
        pen.setWidthF(1.3);
        pen.setStyle(Qt::DashLine);
    } else if (type == "water-flow") {
        pen.setColor(Qt::blue);
        pen.setWidthF(1.8);
    } else if (type == "gradient") {
        pen.setColor(QColor(180, 90, 0));
        pen.setWidthF(1.5);
        pen.setStyle(Qt::DashDotLine);
    }

    pen.setCosmetic(true);
    return pen;
}

QBrush StyleManager::pointBrush(const QString& type)
{
    if (type == "station") {
        return QBrush(Qt::red);
    }

    if (type == "label") {
        return QBrush(Qt::darkGreen);
    }

    if (type == "entrance") {
        return QBrush(Qt::blue);
    }

    if (type == "gradient") {
        return QBrush(QColor(180, 90, 0));
    }

    if (type == "water-flow") {
        return QBrush(Qt::cyan);
    }

    if (type == "stalagmite") {
        return QBrush(Qt::darkGray);
    }

    return QBrush(Qt::gray);
}