#pragma once

#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QString>

#include "../core/Th2Serializable.h"

class PointItem : public QGraphicsEllipseItem, public Th2Serializable
{
public:
    PointItem(const QPointF& position, const QString& therionType);

    QString therionType() const;
    void setTherionType(const QString& therionType);

    QString toTh2() const override;

private:
    QString m_type;
};