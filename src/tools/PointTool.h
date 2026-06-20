#pragma once

#include "Tool.h"

#include <QString>

class PointTool : public Tool
{
public:
    explicit PointTool(const QString& type = "station");

    void mousePress(EditorScene* scene, QGraphicsSceneMouseEvent* event) override;

    void setType(const QString& type);

private:
    QString m_type;
};