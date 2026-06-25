#include "MapScene.h"

#include "PointItem.h"
#include "LineItem.h"

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
    finishCurrentLine(true);
    m_mode = Mode::Select;
}

void MapScene::setAddPointMode(const QString& pointType)
{
    finishCurrentLine(true);
    m_mode = Mode::AddPoint;
    m_currentPointType = pointType;
}

void MapScene::setAddLineMode(const QString& lineType)
{
    finishCurrentLine(true);
    m_mode = Mode::AddLine;
    m_currentLineType = lineType;
}

MapScene::Mode MapScene::mode() const
{
    return m_mode;
}

QString MapScene::currentPointType() const
{
    return m_currentPointType;
}

QString MapScene::currentLineType() const
{
    return m_currentLineType;
}

void MapScene::cancelCurrentLine()
{
    finishCurrentLine(false);
}

void MapScene::finishCurrentLine(bool keepLine)
{
    if (!m_currentLine) {
        return;
    }

    const bool hasEnoughPoints = m_currentLine->points().size() >= 2;

    if (keepLine && hasEnoughPoints) {
        m_currentLine->setSelected(false);
    } else {
        removeItem(m_currentLine);
        delete m_currentLine;
    }

    m_currentLine = nullptr;
}

void MapScene::keyPressEvent(QKeyEvent* event)
{
    if (m_mode == Mode::AddLine && m_currentLine) {
        if (event->key() == Qt::Key_Escape) {
            finishCurrentLine(false);
            event->accept();
            return;
        }

        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            finishCurrentLine(true);
            event->accept();
            return;
        }
    }

    if (event->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> selected = selectedItems();

        for (QGraphicsItem* item : selected) {
            if (item == m_currentLine) {
                m_currentLine = nullptr;
            }

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

    if (m_mode == Mode::AddLine) {
        if (event->button() == Qt::LeftButton) {
            if (!m_currentLine) {
                clearSelection();

                m_currentLine = new LineItem(m_currentLineType);
                addItem(m_currentLine);

                m_currentLine->setSelected(true);
            }

            m_currentLine->addPoint(event->scenePos());

            event->accept();
            return;
        }

        if (event->button() == Qt::RightButton) {
            finishCurrentLine(true);

            event->accept();
            return;
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_mode == Mode::AddLine && m_currentLine && !m_currentLine->points().isEmpty()) {
        m_currentLine->updateLastPoint(event->scenePos());

        event->accept();
        return;
    }

    QGraphicsScene::mouseMoveEvent(event);
}