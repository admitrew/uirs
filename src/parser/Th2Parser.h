#pragma once

#include <QString>
#include <QStringList>
#include <QVector>

#include "../ui/LineItem.h"
#include "../ui/PointItem.h"

class Th2Parser
{
public:
    bool parseFile(const QString& filePath);

    const QVector<LineItem*>& lines() const;
    const QVector<PointItem*>& points() const;

    const QStringList& headerLines() const;
    QString scrapLine() const;
    QString endScrapLine() const;

private:
    QVector<LineItem*> m_lines;
    QVector<PointItem*> m_points;

    QStringList m_headerLines;
    QString m_scrapLine;
    QString m_endScrapLine;
};