#pragma once
#include "Tool.h"
#include "core/TherionTypes.h"
#include <QPointF>
#include <vector>

class LineItem;

class LineTool : public Tool {
public:
    explicit LineTool(LineType type);

    void mousePress(EditorScene*, QGraphicsSceneMouseEvent*) override;
    void mouseMove(EditorScene*, QGraphicsSceneMouseEvent*) override;
    void mouseRelease(EditorScene*, QGraphicsSceneMouseEvent*) override;

    void setType(LineType type);

private:
    LineType m_type;
    LineItem* currentLine = nullptr;
};
