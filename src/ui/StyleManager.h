#pragma once

#include <QPen>
#include <QBrush>

enum class LineType;
enum class PointType;

class StyleManager
{
public:
    static QPen linePen(LineType type);
    static QBrush pointBrush(PointType type);
};
