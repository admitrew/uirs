#include "PointTool.h"

#include "ui/EditorScene.h"
#include "ui/PointItem.h"

PointTool::PointTool(const QString& type)
    : m_type(type)
{
}

void PointTool::setType(const QString& type)
{
    m_type = type;
}

void PointTool::mousePress(EditorScene* scene, QGraphicsSceneMouseEvent* event)
{
    auto* point = new PointItem(event->scenePos(), m_type);
    scene->addItem(point);
}