#include "StyleManager.h"
#include "LineItem.h"
#include "PointItem.h"
#include <Qt>

QPen StyleManager::linePen(LineType type)
{
    QPen pen;
    pen.setWidthF(1.5);

    switch (type) {
    case LineType::Wall:
        pen.setColor(Qt::black);
        pen.setWidthF(2.5);
        break;

    case LineType::Water:
        pen.setColor(Qt::blue);
        pen.setStyle(Qt::DashLine);
        break;

    default:
        pen.setColor(Qt::darkGray);
        break;
    }

    return pen;
}

QBrush StyleManager::pointBrush(PointType type)
{
    switch (type) {
    case PointType::Station:
        return QBrush(Qt::red);

    case PointType::Label:
        return QBrush(Qt::darkGreen);

    default:
        return QBrush(Qt::gray);
    }
}
