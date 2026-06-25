#pragma once

#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QPainter>

#include "../core/Th2Serializable.h"

class PointItem : public QGraphicsEllipseItem, public Th2Serializable
{
public:
    PointItem(const QPointF& position,
              const QString& therionType,
              const QString& options = QString(),
              const QString& rawText = QString());

    QString therionType() const;
    void setTherionType(const QString& therionType);

    QString options() const;
    void setOptions(const QString& options);

    QString rawText() const;
    void setRawText(const QString& rawText);

    QString toTh2() const override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;
private:
    QString m_type;
    QString m_options;
    QString m_rawText;

    bool m_modified = false;
};