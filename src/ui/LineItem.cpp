#include "LineItem.h"

#include <QPainterPath>
#include <QPen>

LineItem::LineItem(const QString& therionType)
    : m_type(therionType)
{
    setPen(QPen(Qt::black, 1));
}

LineItem::LineItem(const QVector<QPointF>& points, const QString& therionType)
    : m_type(therionType), m_points(points)
{
    setPen(QPen(Qt::black, 1));
    rebuildPath();
}

void LineItem::addPoint(const QPointF& p)
{
    m_points.append(p);
    rebuildPath();
}

void LineItem::updateLastPoint(const QPointF& p)
{
    rebuildPath(&p);
}

QVector<QPointF> LineItem::points() const
{
    return m_points;
}

void LineItem::rebuildPath(const QPointF* previewPoint)
{
    QPainterPath path;

    if (!m_points.isEmpty()) {
        path.moveTo(m_points.first());

        for (int i = 1; i < m_points.size(); ++i) {
            path.lineTo(m_points[i]);
        }

        if (previewPoint) {
            path.lineTo(*previewPoint);
        }
    }

    setPath(path);
}

QString LineItem::toTh2() const
{
    QString result = "line " + m_type + "\n";

    for (const QPointF& p : m_points) {
        result += QString("  %1 %2\n").arg(p.x()).arg(-p.y());
    }

    result += "endline\n";
    return result;
}