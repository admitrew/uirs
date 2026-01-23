#include "PointItem.h"

PointItem::PointItem(QPointF p, PointType t): type(t){
    setRect(-3,-3,6,6);
    setPos(p);
}

QString PointItem::toTh2() const {
    return QString("point %1 %2 %3")
        .arg(pos().x()).arg(pos().y()).arg(toTherion(type));
}
