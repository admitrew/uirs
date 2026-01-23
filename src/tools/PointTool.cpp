#include "PointTool.h"
#include "ui/EditorScene.h"
#include "ui/PointItem.h"

PointTool::PointTool(PointType type) : m_type(type) {}

void PointTool::setType(PointType type) {
    m_type = type;
}

void PointTool::mousePress(EditorScene* scene, QGraphicsSceneMouseEvent* e) {
    auto* p = new PointItem(e->scenePos(), m_type);
    scene->addItem(p);
}
