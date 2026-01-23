#pragma once

#include <QObject>

class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class Tool;

class EditorController : public QObject
{
    Q_OBJECT
public:
    explicit EditorController(QGraphicsScene* scene);

    void setTool(Tool* tool);

    void mousePress(QGraphicsSceneMouseEvent* event);
    void mouseMove(QGraphicsSceneMouseEvent* event);
    void mouseRelease(QGraphicsSceneMouseEvent* event);

private:
    QGraphicsScene* m_scene;
    Tool* m_currentTool = nullptr;
};
