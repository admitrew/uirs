#pragma once

#include <QString>
#include <QStringList>

class QGraphicsScene;

class Th2Writer
{
public:
    static void write(QGraphicsScene* scene, const QString& filePath);

    static void write(QGraphicsScene* scene,
                      const QString& filePath,
                      const QStringList& headerLines,
                      const QString& scrapLine,
                      const QString& endScrapLine,
                      const QStringList& areaBlocks = QStringList());
};