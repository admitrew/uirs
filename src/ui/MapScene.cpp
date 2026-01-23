#include "MapScene.h"

MapScene::MapScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 1000, 1000);
}
