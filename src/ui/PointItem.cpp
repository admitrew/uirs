#include "PointItem.h"
#include "StyleManager.h"

#include <QPen>

PointItem::PointItem(const QPointF& position, const QString& therionType)
    : m_type(therionType)
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

QString PointItem::toTh2() const
{
    return QString("point %1 %2 %3")
        .arg(pos().x())
        .arg(-pos().y())
        .arg(m_type);
}