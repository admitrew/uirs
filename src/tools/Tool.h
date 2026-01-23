#pragma once
#include <QGraphicsSceneMouseEvent>

class EditorScene;

class Tool {
public:
    virtual ~Tool() = default;

    virtual void mousePress(EditorScene*, QGraphicsSceneMouseEvent*) {}
    virtual void mouseMove(EditorScene*, QGraphicsSceneMouseEvent*) {}
    virtual void mouseRelease(EditorScene*, QGraphicsSceneMouseEvent*) {}
};
