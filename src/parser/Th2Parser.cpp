#include "Th2Parser.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QPointF>

static QString collectOptions(const QStringList& parts, int startIndex)
{
    if (startIndex >= parts.size()) {
        return QString();
    }

    return parts.mid(startIndex).join(' ');
}

static bool parseDoubleList(const QStringList& parts, QVector<double>& values)
{
    values.clear();

    for (const QString& part : parts) {
        bool ok = false;
        double value = part.toDouble(&ok);

        if (!ok) {
            return false;
        }

        values.append(value);
    }

    return true;
}

bool Th2Parser::parseFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    m_lines.clear();
    m_points.clear();

    m_headerLines.clear();
    m_scrapLine.clear();
    m_endScrapLine.clear();
    m_areaBlocks.clear();

    QTextStream in(&file);

    bool inScrap = false;
    bool inLine = false;
    bool inArea = false;

    QString currentLineType;
    QString currentLineOptions;
    QVector<LineNode> currentLineNodes;

    QStringList currentAreaBlock;

    while (!in.atEnd()) {
        QString rawLine = in.readLine();
        QString line = rawLine.trimmed();

        // Всё до scrap сохраняем как заголовок файла:
        // encoding, ##XTHERION## и другие служебные строки.
        if (!inScrap) {
            if (line.startsWith("scrap ")) {
                m_scrapLine = line;
                inScrap = true;
                continue;
            }

            m_headerLines.append(rawLine);
            continue;
        }

        // Сохраняем area ... endarea как сырой блок.
        if (inArea) {
            currentAreaBlock.append(rawLine);

            if (line == "endarea") {
                m_areaBlocks.append(currentAreaBlock.join("\n"));
                currentAreaBlock.clear();
                inArea = false;
            }

            continue;
        }

        if (line.startsWith("area ")) {
            currentAreaBlock.clear();
            currentAreaBlock.append(rawLine);
            inArea = true;
            continue;
        }

        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);

        if (parts.isEmpty()) {
            continue;
        }

        if (parts[0] == "endscrap") {
            m_endScrapLine = line;
            continue;
        }

        if (parts[0] == "point") {
            if (parts.size() >= 4) {
                QString pointType;
                QString pointOptions;

                double x = 0;
                double y = 0;

                bool okX = false;
                bool okY = false;

                // Основной формат Therion:
                // point x y type [options]
                x = parts[1].toDouble(&okX);
                y = parts[2].toDouble(&okY);

                if (okX && okY) {
                    pointType = parts[3];
                    pointOptions = collectOptions(parts, 4);
                } else {
                    // Поддержка старого тестового формата:
                    // point type x y [options]
                    pointType = parts[1];

                    x = parts[2].toDouble(&okX);
                    y = parts[3].toDouble(&okY);

                    if (okX && okY) {
                        pointOptions = collectOptions(parts, 4);
                    }
                }

                if (okX && okY) {
                    m_points.append(new PointItem(QPointF(x, -y), pointType, pointOptions, line));
                }
            }

            continue;
        }

        if (parts[0] == "line") {
            if (parts.size() >= 2) {
                currentLineType = parts[1];
                currentLineOptions = collectOptions(parts, 2);
                currentLineNodes.clear();
                inLine = true;
            }

            continue;
        }

        if (parts[0] == "endline") {
            if (inLine) {
                m_lines.append(new LineItem(currentLineNodes, currentLineType, currentLineOptions));
            }

            currentLineNodes.clear();
            currentLineType.clear();
            currentLineOptions.clear();
            inLine = false;

            continue;
        }

        if (inLine) {
            QVector<double> values;

            if (parseDoubleList(parts, values)) {
                if (values.size() == 2) {
                    LineNode node;
                    node.kind = LineNode::Kind::Point;
                    node.points.append(QPointF(values[0], -values[1]));
                    node.rawText = line;

                    currentLineNodes.append(node);
                } else if (values.size() == 6) {
                    LineNode node;
                    node.kind = LineNode::Kind::Bezier;
                    node.points.append(QPointF(values[0], -values[1]));
                    node.points.append(QPointF(values[2], -values[3]));
                    node.points.append(QPointF(values[4], -values[5]));
                    node.rawText = line;

                    currentLineNodes.append(node);
                } else {
                    LineNode node;
                    node.kind = LineNode::Kind::Command;
                    node.rawText = line;

                    currentLineNodes.append(node);
                }
            } else {
                LineNode node;
                node.kind = LineNode::Kind::Command;
                node.rawText = line;

                currentLineNodes.append(node);
            }

            continue;
        }
    }

    if (m_scrapLine.isEmpty()) {
        m_scrapLine = "scrap scrap1";
    }

    if (m_endScrapLine.isEmpty()) {
        m_endScrapLine = "endscrap";
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

const QStringList& Th2Parser::headerLines() const
{
    return m_headerLines;
}

QString Th2Parser::scrapLine() const
{
    return m_scrapLine;
}

QString Th2Parser::endScrapLine() const
{
    return m_endScrapLine;
}

const QStringList& Th2Parser::areaBlocks() const
{
    return m_areaBlocks;
}