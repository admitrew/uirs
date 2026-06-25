#include "LineItem.h"
#include "StyleManager.h"

#include <QPainterPath>

LineItem::LineItem(const QString& therionType,
                   const QString& options)
    : m_type(therionType),
      m_options(options)
{
    setPen(StyleManager::linePen(m_type));
    setAcceptHoverEvents(true);
    updateToolTip();
}

LineItem::LineItem(const QVector<QPointF>& points,
                   const QString& therionType,
                   const QString& options)
    : m_type(therionType),
      m_options(options)
{
    for (const QPointF& point : points) {
        LineNode node;
        node.kind = LineNode::Kind::Point;
        node.points.append(point);
        m_nodes.append(node);
    }

    setPen(StyleManager::linePen(m_type));
    setAcceptHoverEvents(true);
    updateToolTip();
    rebuildPath();
}

LineItem::LineItem(const QVector<LineNode>& nodes,
                   const QString& therionType,
                   const QString& options)
    : m_type(therionType),
      m_options(options),
      m_nodes(nodes)
{
    setPen(StyleManager::linePen(m_type));
    setAcceptHoverEvents(true);
    updateToolTip();
    rebuildPath();
}

void LineItem::addPoint(const QPointF& p)
{
    LineNode node;
    node.kind = LineNode::Kind::Point;
    node.points.append(p);

    m_nodes.append(node);
    rebuildPath();
}

void LineItem::updateLastPoint(const QPointF& p)
{
    rebuildPath(&p);
}

QVector<QPointF> LineItem::points() const
{
    QVector<QPointF> result;

    for (const LineNode& node : m_nodes) {
        if (node.kind == LineNode::Kind::Point && !node.points.isEmpty()) {
            result.append(node.points.first());
        } else if (node.kind == LineNode::Kind::Bezier && node.points.size() == 3) {
            result.append(node.points.last());
        }
    }

    return result;
}

QString LineItem::therionType() const
{
    return m_type;
}

void LineItem::setTherionType(const QString& therionType)
{
    m_type = therionType;
    setPen(StyleManager::linePen(m_type));
    updateToolTip();
}

void LineItem::setOptions(const QString& options)
{
    m_options = options;
    updateToolTip();
}

QString LineItem::options() const
{
    return m_options;
}

void LineItem::rebuildPath(const QPointF* previewPoint)
{
    QPainterPath path;
    bool hasCurrentPoint = false;

    for (const LineNode& node : m_nodes) {
        if (node.kind == LineNode::Kind::Point && !node.points.isEmpty()) {
            QPointF p = node.points.first();

            if (!hasCurrentPoint) {
                path.moveTo(p);
                hasCurrentPoint = true;
            } else {
                path.lineTo(p);
            }
        } else if (node.kind == LineNode::Kind::Bezier && node.points.size() == 3) {
            QPointF c1 = node.points[0];
            QPointF c2 = node.points[1];
            QPointF end = node.points[2];

            if (!hasCurrentPoint) {
                path.moveTo(end);
                hasCurrentPoint = true;
            } else {
                path.cubicTo(c1, c2, end);
            }
        }
    }

    if (previewPoint) {
        if (!hasCurrentPoint) {
            path.moveTo(*previewPoint);
        } else {
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

    for (const LineNode& node : m_nodes) {
        if (!node.rawText.trimmed().isEmpty()) {
            result += "  " + node.rawText.trimmed() + "\n";
            continue;
        }

        if (node.kind == LineNode::Kind::Point && !node.points.isEmpty()) {
            QPointF p = node.points.first();

            result += QString("  %1 %2\n")
                .arg(p.x())
                .arg(-p.y());
        } else if (node.kind == LineNode::Kind::Bezier && node.points.size() == 3) {
            QPointF c1 = node.points[0];
            QPointF c2 = node.points[1];
            QPointF end = node.points[2];

            result += QString("  %1 %2 %3 %4 %5 %6\n")
                .arg(c1.x())
                .arg(-c1.y())
                .arg(c2.x())
                .arg(-c2.y())
                .arg(end.x())
                .arg(-end.y());
        }
    }

    result += "endline\n";

    return result;
}

void LineItem::updateToolTip()
{
    QString text = "line " + m_type;

    if (!m_options.trimmed().isEmpty()) {
        text += " " + m_options.trimmed();
    }

    setToolTip(text);
}