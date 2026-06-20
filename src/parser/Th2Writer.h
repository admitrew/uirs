#pragma once

#include <QString>

class QGraphicsScene;

class Th2Writer
{
public:
    static void write(QGraphicsScene* scene, const QString& filePath);
};