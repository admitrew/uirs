#include "Th2Parser.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QPointF>

bool Th2Parser::parseFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    m_lines.clear();
    m_points.clear();

    QTextStream in(&file);

    bool inLine = false;
    QString currentLineType;
    QVector<QPointF> currentLinePoints;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);

        if (parts.isEmpty()) {
            continue;
        }

        if (parts[0] == "point") {
            if (parts.size() >= 4) {
                QString pointType;
                double x = 0;
                double y = 0;
                bool okX = false;
                bool okY = false;

                // Основной формат Therion:
                // point x y type
                x = parts[1].toDouble(&okX);
                y = parts[2].toDouble(&okY);

                if (okX && okY) {
                    pointType = parts[3];
                } else {
                    // Старый тестовый вариант:
                    // point type x y
                    pointType = parts[1];
                    x = parts[2].toDouble(&okX);
                    y = parts[3].toDouble(&okY);
                }

                if (okX && okY) {
                    m_points.append(new PointItem(QPointF(x, -y), pointType));
                }
            }

            continue;
        }

        if (parts[0] == "line") {
            if (parts.size() >= 2) {
                currentLineType = parts[1];
                currentLinePoints.clear();
                inLine = true;
            }

            continue;
        }

        if (parts[0] == "endline") {
            if (inLine && currentLinePoints.size() >= 2) {
                m_lines.append(new LineItem(currentLinePoints, currentLineType));
            }

            currentLinePoints.clear();
            currentLineType.clear();
            inLine = false;

            continue;
        }

        if (inLine && parts.size() >= 2) {
            bool okX = false;
            bool okY = false;

            double x = parts[0].toDouble(&okX);
            double y = parts[1].toDouble(&okY);

            if (okX && okY) {
                currentLinePoints.append(QPointF(x, -y));
            }
        }
    }

    return true;
}

const QVector<LineItem*>& Th2Parser::lines() const
{
    return m_lines;
}

const QVector<PointItem*>& Th2Parser::points() const
{
    return m_points;
}