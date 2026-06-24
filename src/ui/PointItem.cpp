#include "PointItem.h"
#include "StyleManager.h"

#include <QPen>

PointItem::PointItem(const QPointF& position,
                     const QString& therionType,
                     const QString& options)
    : m_type(therionType),
      m_options(options)
{
    setRect(-3, -3, 6, 6);
    setPos(position);

    setBrush(StyleManager::pointBrush(m_type));
    setPen(QPen(Qt::black, 1));

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
}

QString PointItem::therionType() const
{
    return m_type;
}

void PointItem::setTherionType(const QString& therionType)
{
    m_type = therionType;
    setBrush(StyleManager::pointBrush(m_type));
}

QString PointItem::options() const
{
    return m_options;
}

void PointItem::setOptions(const QString& options)
{
    m_options = options;
}

QString PointItem::toTh2() const
{
    QString result = QString("point %1 %2 %3")
        .arg(pos().x())
        .arg(-pos().y())
        .arg(m_type);

    if (!m_options.trimmed().isEmpty()) {
        result += " " + m_options.trimmed();
    }

    return result;
}