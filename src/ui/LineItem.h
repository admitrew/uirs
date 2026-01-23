#pragma once
#include <QGraphicsPathItem>
#include "../core/TherionTypes.h"
#include "../core/Th2Serializable.h"

class LineItem : public QGraphicsPathItem {
public:
    explicit LineItem(const QString& therionType);

    void addPoint(const QPointF& p);
    QVector<QPointF> points() const;

private:
    QString m_type;
    QVector<QPointF> m_points;
};
