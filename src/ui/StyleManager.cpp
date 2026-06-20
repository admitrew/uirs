#include "StyleManager.h"

#include <Qt>

QPen StyleManager::linePen(const QString& type)
{
    QPen pen;
    pen.setWidthF(1.5);

    if (type == "wall") {
        pen.setColor(Qt::black);
        pen.setWidthF(2.5);
    } else if (type == "water") {
        pen.setColor(Qt::blue);
        pen.setStyle(Qt::DashLine);
    } else {
        pen.setColor(Qt::darkGray);
    }

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

    return QBrush(Qt::gray);
}