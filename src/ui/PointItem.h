#pragma once

#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QString>

#include "../core/Th2Serializable.h"

class PointItem : public QGraphicsEllipseItem, public Th2Serializable
{
public:
    PointItem(const QPointF& position,
              const QString& therionType,
              const QString& options = QString());

    QString therionType() const;
    void setTherionType(const QString& therionType);

    QString options() const;
    void setOptions(const QString& options);

    QString toTh2() const override;

private:
    QString m_type;
    QString m_options;
};