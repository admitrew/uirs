#pragma once

#include <QString>
#include <QVector>

#include "../ui/LineItem.h"
#include "../ui/PointItem.h"

class Th2Parser
{
public:
    bool parseFile(const QString& filePath);

    const QVector<LineItem*>& lines() const;
    const QVector<PointItem*>& points() const;

private:
    QVector<LineItem*> m_lines;
    QVector<PointItem*> m_points;
};