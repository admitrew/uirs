#include "EditorScene.h"
#include "tools/Tool.h"

void EditorScene::setTool(Tool* t) { tool = t; }

void EditorScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    if (tool) tool->mousePress(this, e);
}

void EditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (tool) tool->mouseMove(this, e);
}

void EditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (tool) tool->mouseRelease(this, e);
}
