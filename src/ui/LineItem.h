#pragma once

#include <QGraphicsPathItem>
#include <QVector>
#include <QPointF>
#include <QString>

#include "../core/Th2Serializable.h"

class LineItem : public QGraphicsPathItem, public Th2Serializable
{
public:
    explicit LineItem(const QString& therionType);
    LineItem(const QVector<QPointF>& points, const QString& therionType);

    void addPoint(const QPointF& p);
    void updateLastPoint(const QPointF& p);

    QVector<QPointF> points() const;

    QString toTh2() const override;

private:
    void rebuildPath(const QPointF* previewPoint = nullptr);

    QString m_type;
    QVector<QPointF> m_points;
};