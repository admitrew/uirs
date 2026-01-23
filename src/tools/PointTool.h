#pragma once
#include "Tool.h"
#include "core/TherionTypes.h"

class PointTool : public Tool {
public:
    explicit PointTool(PointType type);

    void mousePress(EditorScene*, QGraphicsSceneMouseEvent*) override;

    void setType(PointType type);

private:
    PointType m_type;
};
