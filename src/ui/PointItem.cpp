#include "PointItem.h"
#include "StyleManager.h"

#include <QPen>

PointItem::PointItem(const QPointF& position,
                     const QString& therionType,
                     const QString& options,
                     const QString& rawText)
    : m_type(therionType),
      m_options(options),
      m_rawText(rawText)
{
    setRect(-3, -3, 6, 6);
    setPos(position);

    setBrush(StyleManager::pointBrush(m_type));
    setPen(QPen(Qt::black, 1));

    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

QString PointItem::therionType() const
{
    return m_type;
}

void PointItem::setTherionType(const QString& therionType)
{
    m_type = therionType;
    m_modified = true;
    setBrush(StyleManager::pointBrush(m_type));
}

QString PointItem::options() const
{
    return m_options;
}

void PointItem::setOptions(const QString& options)
{
    m_options = options;
    m_modified = true;
}

QString PointItem::rawText() const
{
    return m_rawText;
}

void PointItem::setRawText(const QString& rawText)
{
    m_rawText = rawText;
    m_modified = false;
}

QVariant PointItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        m_modified = true;
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

QString PointItem::toTh2() const
{
    if (!m_modified && !m_rawText.trimmed().isEmpty()) {
        return m_rawText.trimmed();
    }

    QString result = QString("point %1 %2 %3")
        .arg(pos().x(), 0, 'g', 15)
        .arg(-pos().y(), 0, 'g', 15)
        .arg(m_type);

    if (!m_options.trimmed().isEmpty()) {
        result += " " + m_options.trimmed();
    }

    return result;
}