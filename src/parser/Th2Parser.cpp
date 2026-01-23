#include "Th2Parser.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>

bool Th2Parser::parseFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    bool inLine = false;
    LineType currentType = LineType::Other;
    QVector<QPointF> points;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith("point")) {
            // point station x y
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 4) {
                bool okX, okY;
                double x = parts[2].toDouble(&okX);
                double y = parts[3].toDouble(&okY);

                if (okX && okY) {
                    auto type = parsePointType(parts[1]);
                    m_points.append(new PointItem(QPointF(x, -y), type));
                }
            }
            continue;
        }

        if (line.startsWith("line")) {
            // line wall / line water
            QStringList parts = line.split(' ');
            if (parts.size() >= 2) {
                currentType = parseLineType(parts[1]);
                points.clear();
                inLine = true;
            }
            continue;
        }

        if (line == "endline" && inLine) {
            if (points.size() >= 2) {
                m_lines.append(new LineItem(points, currentType));
            }
            inLine = false;
            continue;
        }

        if (inLine) {
            // координаты: x y
            QStringList coords = line.split(' ', Qt::SkipEmptyParts);
            if (coords.size() >= 2) {
                bool okX, okY;
                double x = coords[0].toDouble(&okX);
                double y = coords[1].toDouble(&okY);

                if (okX && okY)
                    points.append(QPointF(x, -y)); // инверсия Y для сцены
            }
        }
    }

    return true;
}

const QVector<LineItem*>& Th2Parser::lines() const
{
    return m_lines;
}

LineType Th2Parser::parseLineType(const QString& typeStr) const
{
    if (typeStr == "wall")
        return LineType::Wall;
    if (typeStr == "water")
        return LineType::Water;
    return LineType::Other;
}

const QVector<PointItem*>& Th2Parser::points() const
{
    return m_points;
}

PointType Th2Parser::parsePointType(const QString& typeStr) const
{
    if (typeStr == "station")
        return PointType::Station;
    if (typeStr == "label")
        return PointType::Label;
    return PointType::Other;
}
