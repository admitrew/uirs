#pragma once

#include <QGraphicsPathItem>
#include <QVector>
#include <QPointF>
#include <QString>

#include "../core/Th2Serializable.h"

struct LineNode
{
    enum class Kind {
        Point,
        Bezier,
        Command
    };

    Kind kind;
    QVector<QPointF> points;
    QString rawText;
};

class LineItem : public QGraphicsPathItem, public Th2Serializable
{
public:
    explicit LineItem(const QString& therionType,
                      const QString& options = QString());

    LineItem(const QVector<QPointF>& points,
             const QString& therionType,
             const QString& options = QString());

    LineItem(const QVector<LineNode>& nodes,
             const QString& therionType,
             const QString& options = QString());

    void addPoint(const QPointF& p);
    void updateLastPoint(const QPointF& p);

    QVector<QPointF> points() const;

    QString therionType() const;
    void setTherionType(const QString& therionType);

    QString options() const;
    void setOptions(const QString& options);

    QString toTh2() const override;

private:
    void rebuildPath(const QPointF* previewPoint = nullptr);
    void updateToolTip();

    QString m_type;
    QString m_options;
    QVector<LineNode> m_nodes;
};