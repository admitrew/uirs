#pragma once
#include <QGraphicsScene>

class Tool;

class EditorScene : public QGraphicsScene {
public:
    void setTool(Tool*);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent*) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
private:
    Tool* tool = nullptr;
};
