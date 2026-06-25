#include "MapScene.h"
#include "PointItem.h"

#include <QKeyEvent>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QList>

MapScene::MapScene(QObject* parent)
    : QGraphicsScene(parent)
{
}

void MapScene::setSelectMode()
{
    m_mode = Mode::Select;
}

void MapScene::setAddPointMode(const QString& pointType)
{
    m_mode = Mode::AddPoint;
    m_currentPointType = pointType;
}

MapScene::Mode MapScene::mode() const
{
    return m_mode;
}

QString MapScene::currentPointType() const
{
    return m_currentPointType;
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

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_mode == Mode::AddPoint && event->button() == Qt::LeftButton) {
        clearSelection();

        auto* point = new PointItem(event->scenePos(), m_currentPointType);
        addItem(point);

        point->setSelected(true);

        event->accept();
        return;
    }

    QGraphicsScene::mousePressEvent(event);
}