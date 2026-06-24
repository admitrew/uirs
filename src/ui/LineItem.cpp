#include "LineItem.h"
#include "StyleManager.h"

#include <QPainterPath>

LineItem::LineItem(const QString& therionType,
                   const QString& options)
    : m_type(therionType),
      m_options(options)
{
    setPen(StyleManager::linePen(m_type));
}

LineItem::LineItem(const QVector<QPointF>& points,
                   const QString& therionType,
                   const QString& options)
    : m_type(therionType),
      m_options(options),
      m_points(points)
{
    setPen(StyleManager::linePen(m_type));
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

QString LineItem::therionType() const
{
    return m_type;
}

void LineItem::setTherionType(const QString& therionType)
{
    m_type = therionType;
    setPen(StyleManager::linePen(m_type));
}

QString LineItem::options() const
{
    return m_options;
}

void LineItem::setOptions(const QString& options)
{
    m_options = options;
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
    QString result = "line " + m_type;

    if (!m_options.trimmed().isEmpty()) {
        result += " " + m_options.trimmed();
    }

    result += "\n";

    for (const QPointF& p : m_points) {
        result += QString("  %1 %2\n").arg(p.x()).arg(-p.y());
    }

    result += "endline\n";

    return result;
}