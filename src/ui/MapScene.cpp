#include "MapScene.h"

#include <QKeyEvent>
#include <QGraphicsItem>
#include <QList>

MapScene::MapScene(QObject* parent)
    : QGraphicsScene(parent)
{
}

void MapScene::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> selected = selectedItems();

        for (QGraphicsItem* item : selected) {
            removeItem(item);
            delete item;
        }

        event->accept();
        return;
    }

    QGraphicsScene::keyPressEvent(event);
}