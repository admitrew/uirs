#include "LineTool.h"
#include "ui/EditorScene.h"
#include "ui/LineItem.h"

LineTool::LineTool(LineType type) : m_type(type) {}

void LineTool::setType(LineType type) {
    m_type = type;
}

void LineTool::mousePress(EditorScene* scene, QGraphicsSceneMouseEvent* e) {
    if (!currentLine) {
        currentLine = new LineItem(m_type);
        currentLine->addPoint(e->scenePos());
        scene->addItem(currentLine);
    }
}

void LineTool::mouseMove(EditorScene*, QGraphicsSceneMouseEvent* e) {
    if (currentLine)
        currentLine->updateLastPoint(e->scenePos());
}

void LineTool::mouseRelease(EditorScene*, QGraphicsSceneMouseEvent* e) {
    if (currentLine) {
        currentLine->addPoint(e->scenePos());
        currentLine = nullptr;
    }
}
