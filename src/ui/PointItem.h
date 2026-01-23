#pragma once
#include <QGraphicsEllipseItem>
#include "../core/TherionTypes.h"
#include "../core/Th2Serializable.h"

class PointItem : public QGraphicsEllipseItem, public Th2Serializable {
public:
    PointItem(QPointF, PointType);
    QString toTh2() const override;
private:
    PointType type;
};
